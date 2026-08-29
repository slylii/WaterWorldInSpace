#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Generator.generated.h"

class UStaticMeshComponent;
class USkillCheckComponent;
class USoundBase;
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
    float GreateHP = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
    float GoodHP = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
    float MissHP = -10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
    float MaxHP = 100.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Generator")
    bool bIsRepairing = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator|Sound")
    USoundBase* SkillCheckStartSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator|Sound")
    USoundBase* GoodHitSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator|Sound")
    USoundBase* GreatHitSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator|Sound")
    USoundBase* MissSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator|Sound")
    USoundBase* RepairCompleteSound;

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
    void FinishRepair(bool bSuccess);

    void PlaySound(USoundBase* Sound);
};