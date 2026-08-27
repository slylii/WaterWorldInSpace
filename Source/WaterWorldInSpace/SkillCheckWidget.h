#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillCheckWidget.generated.h"

class USkillCheckComponent;
class UImage;

UCLASS()
class WATERWORLDINSPACE_API USkillCheckWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void Init(USkillCheckComponent* InComponent);

protected:
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    UPROPERTY()
    TObjectPtr<USkillCheckComponent> SkillCheckComponent;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> Pointer;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> ZonesImage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    float ZoneAngleOffset = 0.0f;
};