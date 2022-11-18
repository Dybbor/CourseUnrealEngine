// CourseShootThemUp

#pragma once

#include "CoreMinimal.h"
#include "Weapon/CSTUBaseWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Weapon/CSTUProjectile.h"
#include "CSTULauncherWeapon.generated.h"

/**
 * 
 */
class ACSTUProjectile;

UCLASS()
class COURSESHOOTTHEMUP_API ACSTULauncherWeapon : public ACSTUBaseWeapon
{
    GENERATED_BODY()
public:
    virtual void StartFire() override;
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapons")
    TSubclassOf<ACSTUProjectile> ProjectileClass; 

    virtual void MakeShot() override;
};
