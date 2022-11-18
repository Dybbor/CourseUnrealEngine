// CourseShootThemUp

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/CSTUBaseWeapon.h"
#include "CSTUWeaponComponent.generated.h"

DEFINE_LOG_CATEGORY_STATIC(BaseWeaponComponent, All, All)

class ACSTUBaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COURSESHOOTTHEMUP_API UCSTUWeaponComponent : public UActorComponent {
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UCSTUWeaponComponent();
    void StartFire();
    void StopFire();
    void NextWeapon();

protected:
    // Called when the game starts
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<TSubclassOf<ACSTUBaseWeapon>> WeaponClasses;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponPoint";

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponArmorySocketName = "ArmorySocket";

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    UPROPERTY()
    ACSTUBaseWeapon* CurrentWeapon = nullptr;

    UPROPERTY()
    TArray<ACSTUBaseWeapon*> Weapons;

    int32 CurrentWeaponIndex = 0;

    void SpawnWeapons();
    void AttachWeaponToSocket(ACSTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
    void EquipWeapon(int32 WeaponIndex);
};
