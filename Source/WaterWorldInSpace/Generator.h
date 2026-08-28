#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Generator.generated.h"

class UStaticMeshComponent;
class USkillCheckComponent;
enum class ESkillCheckResult : uint8;

UCLASS()
class WATERWORLDINSPACE_API AGenerator : public AActor
{
    GENERATED_BODY()

public:
    AGenerator();

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION(BlueprintCallable, Category = "Generator|Interaction")
    void Interact();

    UFUNCTION()
    void StartSkillCheckDelayed();

    UFUNCTION()
    void OnSkillCheckResult(ESkillCheckResult Result);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generator")
    float CurrentHP = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
    float MaxHP = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
    bool bIsRepairing = false;

    UFUNCTION(BlueprintCallable, Category = "Generator|Interaction")
    void CompleteRepair();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UStaticMeshComponent> GeneratorMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<USkillCheckComponent> SkillCheckComponent;

    bool bIsInteracting = false;

    FTimerHandle SkillCheckDelayTimer;

    void HandleGreat();
    void HandleGood();
    void HandleMiss();
    void AddHP(float Delta);
    void BlockPlayerInput(bool bBlock);
    void CancelInteraction();
    void ShowHPOnScreen();
};