#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillCheckComponent.generated.h"

class UInputAction;
class UUserWidget;
class USkillCheckWidget;

UENUM(BlueprintType)
enum class ESkillCheckResult : uint8
{
    Great UMETA(DisplayName = "Great"),
    Good  UMETA(DisplayName = "Good"),
    Miss  UMETA(DisplayName = "Miss")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillCheckResult, ESkillCheckResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillCheckStarted);

UCLASS(ClassGroup = (G2I), meta = (BlueprintSpawnableComponent))
class WATERWORLDINSPACE_API USkillCheckComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USkillCheckComponent();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    UFUNCTION(BlueprintCallable, Category = "Skill Check")
    void StartSkillCheck();

    UFUNCTION(BlueprintCallable, Category = "Skill Check")
    void StopSkillCheck();

    UFUNCTION(BlueprintPure, Category = "Skill Check")
    bool IsSkillCheckActive() const { return bIsActive; }

    UFUNCTION(BlueprintPure, Category = "Skill Check")
    float GetCurrentAngle() const { return CurrentAngle; }

    UFUNCTION(BlueprintPure, Category = "Skill Check")
    float GetGreatZoneCenterAngle() const { return GreatZoneCenterAngle; }

    UFUNCTION(BlueprintPure, Category = "Skill Check")
    float GetGoodZoneCenterAngle() const { return GoodZoneCenterAngle; }

    UFUNCTION(BlueprintPure, Category = "Skill Check")
    float GetGreatZoneSize() const { return GreatZoneSize; }

    UFUNCTION(BlueprintPure, Category = "Skill Check")
    float GetGoodZoneSize() const { return GoodZoneSize; }

    UFUNCTION(BlueprintPure, Category = "Skill Check")
    float GetPointerSpeed() const { return PointerSpeed; }

public:
    UPROPERTY(BlueprintAssignable, Category = "Skill Check")
    FOnSkillCheckStarted OnSkillCheckStarted;

    UPROPERTY(BlueprintAssignable, Category = "Skill Check")
    FOnSkillCheckResult OnSkillCheckResult;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Check|Input")
    TObjectPtr<UInputAction> SkillCheckInputAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Check|UI")
    TSubclassOf<UUserWidget> SkillCheckWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Check|Settings", meta = (ClampMin = "0.1"))
    float CheckDuration = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Check|Zones", meta = (ClampMin = "0.0", ClampMax = "360.0"))
    float GreatZoneSize = 12.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Check|Zones", meta = (ClampMin = "0.0", ClampMax = "360.0"))
    float GoodZoneSize = 40.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Check|Zones", meta = (ClampMin = "0.0", ClampMax = "360.0"))
    float RandomAngleMin = 30.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Check|Zones", meta = (ClampMin = "0.0", ClampMax = "360.0"))
    float RandomAngleMax = 330.0f;

private:
    UPROPERTY()
    TObjectPtr<USkillCheckWidget> SkillCheckWidget;

    float CurrentAngle = 0.0f;
    float GreatZoneCenterAngle = 0.0f;
    float GoodZoneCenterAngle = 0.0f;
    float PointerSpeed = 0.0f;
    bool bIsActive = false;
    FTimerHandle TimerHandle_AutoComplete;
    bool bInputBound = false;

    void OnSkillCheckInput();
    ESkillCheckResult CalculateResult() const;
    bool IsAngleInsideZone(float Angle, float ZoneCenter, float ZoneSize) const;
    void CreateSkillCheckWidget();
    void RemoveSkillCheckWidget();
    void GenerateZones();
    void BindInput();
    void OnAutoCompleteTimeout();
    void ShowScreenResult(ESkillCheckResult Result);
};