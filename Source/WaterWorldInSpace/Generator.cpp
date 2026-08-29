#include "Generator.h"
#include "Components/StaticMeshComponent.h"
#include "SkillCheckComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

AGenerator::AGenerator()
{
    PrimaryActorTick.bCanEverTick = false;

    GeneratorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GeneratorMesh"));
    RootComponent = GeneratorMesh;

    SkillCheckComponent = CreateDefaultSubobject<USkillCheckComponent>(TEXT("SkillCheckComponent"));
}

void AGenerator::BeginPlay()
{
    Super::BeginPlay();

    if (!SkillCheckComponent)
    {
        return;
    }

    SkillCheckComponent->OnSkillCheckResult.AddDynamic(this, &AGenerator::OnSkillCheckResult);

    CurrentHP = 0.0f;
    bIsRepairing = false;
    bIsInteracting = false;
}

void AGenerator::Interact()
{
    if (CurrentHP >= MaxHP)
    {
        return;
    }

    if (bIsInteracting)
    {
        CancelInteraction();
        return;
    }

    bIsInteracting = true;
    bIsRepairing = true;
    BlockPlayerInput(true);

    float Delay = FMath::RandRange(0.5f, 2.0f);
    GetWorld()->GetTimerManager().SetTimer(
        SkillCheckDelayTimer,
        this,
        &AGenerator::StartSkillCheckDelayed,
        Delay,
        false
    );
}

void AGenerator::StartSkillCheckDelayed()
{
    if (!bIsInteracting) return;

    if (!SkillCheckComponent)
    {
        return;
    }

    if (SkillCheckComponent->IsSkillCheckActive())
        return;

    PlaySound(SkillCheckStartSound);

    SkillCheckComponent->StartSkillCheck();
}

void AGenerator::OnSkillCheckResult(ESkillCheckResult Result)
{
    switch (Result)
    {
    case ESkillCheckResult::Great: HandleGreat(); break;
    case ESkillCheckResult::Good:  HandleGood();  break;
    case ESkillCheckResult::Miss:  HandleMiss();  break;
    default: break;
    }

    if (CurrentHP >= MaxHP)
    {
        FinishRepair(true);
        return;
    }

    float Delay = FMath::RandRange(0.5f, 2.0f);
    GetWorld()->GetTimerManager().SetTimer(
        SkillCheckDelayTimer,
        this,
        &AGenerator::StartSkillCheckDelayed,
        Delay,
        false
    );
}

void AGenerator::HandleGreat()
{
    AddHP(GreateHP);
    PlaySound(GreatHitSound);
}

void AGenerator::HandleGood()
{
    AddHP(GoodHP);
    PlaySound(GoodHitSound);
}

void AGenerator::HandleMiss()
{
    AddHP(MissHP);
    PlaySound(MissSound);
}

void AGenerator::AddHP(float Delta)
{
    CurrentHP = FMath::Clamp(CurrentHP + Delta, 0.0f, MaxHP);

    if (GEngine)
    {
        FString HPMessage = FString::Printf(TEXT("Generator HP: %.1f / %.1f"), CurrentHP, MaxHP);
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, HPMessage);
    }
}

void AGenerator::BlockPlayerInput(bool bBlock)
{
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;

    PC->SetIgnoreMoveInput(bBlock);
    PC->SetIgnoreLookInput(bBlock);
}

void AGenerator::CancelInteraction()
{
    if (!bIsInteracting) return;

    GetWorld()->GetTimerManager().ClearTimer(SkillCheckDelayTimer);

    if (SkillCheckComponent && SkillCheckComponent->IsSkillCheckActive())
    {
        SkillCheckComponent->StopSkillCheck();
    }

    FinishRepair(false);
}

void AGenerator::FinishRepair(bool bSuccess)
{
    if (!bIsInteracting) return;

    bIsInteracting = false;
    bIsRepairing = false;

    BlockPlayerInput(false);
    GetWorld()->GetTimerManager().ClearTimer(SkillCheckDelayTimer);

    if (bSuccess)
    {
        if (RepairCompleteSound)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), RepairCompleteSound, GetActorLocation());
        }
    }
}

void AGenerator::PlaySound(USoundBase* Sound)
{
    if (Sound && GetWorld())
    {
        UGameplayStatics::PlaySound2D(GetWorld(), Sound);
    }
}