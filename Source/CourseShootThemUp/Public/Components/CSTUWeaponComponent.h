// CourseShootThemUp

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/CSTUBaseWeapon.h"
#include "CSTUWeaponComponent.generated.h"

DEFINE_LOG_CATEGORY_STATIC(BaseWeaponComponent, All, All)

class ACSTUBaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COURSESHOOTTHEMUP_API UCSTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCSTUWeaponComponent();
    void StartFire();
    void StopFire();

protected:
	// Called when the game starts
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<ACSTUBaseWeapon> WeaponClass;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponAttachPointName = "WeaponPoint";

	virtual void BeginPlay() override;

private:
    UPROPERTY()
    ACSTUBaseWeapon* CurrentWeapon = nullptr;

    void SpawnWeapon();

		
};
