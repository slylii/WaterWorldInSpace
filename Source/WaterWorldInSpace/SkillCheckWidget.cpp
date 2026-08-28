#include "SkillCheckWidget.h"
#include "Components/Image.h"
#include "SkillCheckComponent.h"

void USkillCheckWidget::Init(USkillCheckComponent* InComponent)
{
    SkillCheckComponent = InComponent;
}

void USkillCheckWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    if (!SkillCheckComponent) return;

    if (Pointer)
        Pointer->SetRenderTransformAngle(SkillCheckComponent->GetCurrentAngle());

    if (ZonesImage)
    {
        float Angle = SkillCheckComponent->GetGreatZoneCenterAngle() + ZoneAngleOffset;
        Angle = FMath::Fmod(Angle, 360.0f);
        if (Angle < 0.0f) Angle += 360.0f;
        ZonesImage->SetRenderTransformAngle(Angle);
    }
}