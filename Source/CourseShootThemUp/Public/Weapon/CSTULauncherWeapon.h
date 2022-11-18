// CourseShootThemUp

#pragma once

#include "CoreMinimal.h"
#include "Weapon/CSTUBaseWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "CSTULauncherWeapon.generated.h"

/**
 * 
 */
UCLASS()
class COURSESHOOTTHEMUP_API ACSTULauncherWeapon : public ACSTUBaseWeapon
{
    GENERATED_BODY()
public:
    virtual void StartFire() override;
    virtual void StopFire() override;
};
