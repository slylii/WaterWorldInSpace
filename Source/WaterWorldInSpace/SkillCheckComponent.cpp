#include "SkillCheckComponent.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "SkillCheckWidget.h"
#include "Engine/Engine.h"

USkillCheckComponent::USkillCheckComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void USkillCheckComponent::BeginPlay()
{
    Super::BeginPlay();
    BindInput();
}

void USkillCheckComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (!bIsActive) return;

    CurrentAngle += PointerSpeed * DeltaTime;
    if (CurrentAngle >= 360.0f) CurrentAngle -= 360.0f;
}

void USkillCheckComponent::StartSkillCheck()
{
    if (bIsActive) return;
    if (!GetWorld()) return;
    if (!SkillCheckWidgetClass) return;

    GoodZoneSize = FMath::Max(GoodZoneSize, GreatZoneSize);
    PointerSpeed = 360.0f / CheckDuration;
    CurrentAngle = 0.0f;
    GenerateZones();

    bIsActive = true;
    CreateSkillCheckWidget();

    GetWorld()->GetTimerManager().SetTimer(TimerHandle_AutoComplete, this, &USkillCheckComponent::OnAutoCompleteTimeout, CheckDuration, false);

    OnSkillCheckStarted.Broadcast();
}

void USkillCheckComponent::StopSkillCheck()
{
    if (!bIsActive) return;

    bIsActive = false;
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle_AutoComplete);
    RemoveSkillCheckWidget();
}

void USkillCheckComponent::OnSkillCheckInput()
{
    if (!bIsActive) return;

    const ESkillCheckResult Result = CalculateResult();

    ShowScreenResult(Result);

    bIsActive = false;
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle_AutoComplete);
    RemoveSkillCheckWidget();

    OnSkillCheckResult.Broadcast(Result);
}

ESkillCheckResult USkillCheckComponent::CalculateResult() const
{
    if (IsAngleInsideZone(CurrentAngle, GreatZoneCenterAngle, GreatZoneSize))
        return ESkillCheckResult::Great;

    if (IsAngleInsideZone(CurrentAngle, GoodZoneCenterAngle, GoodZoneSize))
        return ESkillCheckResult::Good;

    return ESkillCheckResult::Miss;
}

bool USkillCheckComponent::IsAngleInsideZone(float Angle, float ZoneCenter, float ZoneSize) const
{
    const float Difference = FMath::Abs(FMath::FindDeltaAngleDegrees(Angle, ZoneCenter));
    return Difference <= ZoneSize * 0.5f;
}

void USkillCheckComponent::GenerateZones()
{
    const float MinAngle = FMath::Min(RandomAngleMin, RandomAngleMax);
    const float MaxAngle = FMath::Max(RandomAngleMin, RandomAngleMax);
    GreatZoneCenterAngle = FMath::RandRange(MinAngle, MaxAngle);
    GoodZoneCenterAngle = GreatZoneCenterAngle;
}

void USkillCheckComponent::CreateSkillCheckWidget()
{
    if (SkillCheckWidget) return;

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;

    SkillCheckWidget = Cast<USkillCheckWidget>(CreateWidget<UUserWidget>(PC, SkillCheckWidgetClass));
    if (SkillCheckWidget)
    {
        SkillCheckWidget->Init(this);
        SkillCheckWidget->AddToViewport(1000);
    }
}

void USkillCheckComponent::RemoveSkillCheckWidget()
{
    if (SkillCheckWidget)
    {
        SkillCheckWidget->RemoveFromParent();
        SkillCheckWidget = nullptr;
    }
}

void USkillCheckComponent::BindInput()
{
    if (bInputBound) return;
    if (!SkillCheckInputAction) return;

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;

    UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PC->InputComponent);
    if (!EIC) return;

    EIC->BindAction(SkillCheckInputAction, ETriggerEvent::Started, this, &USkillCheckComponent::OnSkillCheckInput);
    bInputBound = true;
}

void USkillCheckComponent::OnAutoCompleteTimeout()
{
    if (!bIsActive) return;

    const ESkillCheckResult Result = ESkillCheckResult::Miss;

    ShowScreenResult(Result);

    bIsActive = false;
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle_AutoComplete);
    RemoveSkillCheckWidget();

    OnSkillCheckResult.Broadcast(Result);
}

void USkillCheckComponent::ShowScreenResult(ESkillCheckResult Result)
{
    if (!GEngine) return;

    FString Message = FString::Printf(
        TEXT("Result: %s\n")
        TEXT("Current Angle: %.1f\n")
        TEXT("Great Center: %.1f (Size: %.1f)\n")
        TEXT("Good Center:  %.1f (Size: %.1f)"),
        Result == ESkillCheckResult::Great ? TEXT("GREAT") :
        Result == ESkillCheckResult::Good ? TEXT("GOOD") : TEXT("MISS"),
        CurrentAngle,
        GreatZoneCenterAngle, GreatZoneSize,
        GoodZoneCenterAngle, GoodZoneSize
    );

    FColor Color = Result == ESkillCheckResult::Great ? FColor::Green :
        Result == ESkillCheckResult::Good ? FColor::Yellow : FColor::Red;

    GEngine->AddOnScreenDebugMessage(-1, 3.0f, Color, Message);
}