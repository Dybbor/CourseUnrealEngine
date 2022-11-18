// CourseShootThemUp

#pragma once

#include "CoreMinimal.h"
#include "Weapon/CSTUBaseWeapon.h"
#include "CSTURifleWeapon.generated.h"

/**
 * Riffle weapon
 */
DEFINE_LOG_CATEGORY_STATIC(LogRiffleWeapon, All, All)

UCLASS()
class COURSESHOOTTHEMUP_API ACSTURifleWeapon : public ACSTUBaseWeapon {
    GENERATED_BODY()
    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TimeBetweenShots = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BulletSpread = 1.5f;

    virtual void MakeShot() override;
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

private:
    FTimerHandle ShotTimerHandle;
};
