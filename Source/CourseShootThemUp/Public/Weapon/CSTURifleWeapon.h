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
public:
    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Riffle Weapon")
    float TimeBetweenShots = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Riffle Weapon")
    float BulletSpread = 1.5f;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Riffle Weapon")
    float DamageAmount = 10.0f;

    virtual void MakeShot() override;  // Make shot through angles between Muzzle.ForwardVector and VectorMuzzleToImpactPoint
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;
    void MakeShot1();  // Make shot by moved start point (projected VectorCameraToMuzzle to CameraRotation)

private:
    FTimerHandle ShotTimerHandle;
    void MakeDamage(AActor* Enemy);
};
