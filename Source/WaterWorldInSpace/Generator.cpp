#include "Generator.h"
#include "Components/StaticMeshComponent.h"
#include "SkillCheckComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

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

    if (!SkillCheckComponent) return;

    SkillCheckComponent->OnSkillCheckResult.AddDynamic(this, &AGenerator::OnSkillCheckResult);

    CurrentHP = 0.0f;
    bIsRepairing = false;
}

void AGenerator::Interact()
{
    if (CurrentHP >= MaxHP) return;

    if (bIsInteracting)
    {
        CancelInteraction();
        return;
    }

    bIsInteracting = true;
    bIsRepairing = true;
    BlockPlayerInput(true);

    ShowHPOnScreen();

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

    if (!SkillCheckComponent) return;

    if (SkillCheckComponent->IsSkillCheckActive()) return;

       
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
        CompleteRepair();
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
    AddHP(30.0f);
}

void AGenerator::HandleGood()
{
    AddHP(20.0f);
}

void AGenerator::HandleMiss()
{
    AddHP(-10.0f);
}

void AGenerator::AddHP(float Delta)
{
    CurrentHP = FMath::Clamp(CurrentHP + Delta, 0.0f, MaxHP);
    ShowHPOnScreen();
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

    bIsInteracting = false;
    bIsRepairing = false;
    BlockPlayerInput(false);

    if (GEngine)
    {
        GEngine->ClearOnScreenDebugMessages();
    }
}

void AGenerator::CompleteRepair()
{
    bIsInteracting = false;
    bIsRepairing = false;
    BlockPlayerInput(false);

    if (GEngine)
    {
        GEngine->ClearOnScreenDebugMessages();
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Generator fully repaired!"));
    }
}

void AGenerator::ShowHPOnScreen()
{
    if (!GEngine) return;

    FString HPMessage = FString::Printf(TEXT("Generator HP: %.0f / %.0f"), CurrentHP, MaxHP);
    GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::White, HPMessage);
}