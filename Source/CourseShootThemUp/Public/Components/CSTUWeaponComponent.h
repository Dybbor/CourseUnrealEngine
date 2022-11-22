// CourseShootThemUp

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "Weapon/CSTUBaseWeapon.h"
#include "Animations/CSTUEquipFinishAnimNotify.h"
#include "Animations/CSTUReloadFinishAnimNotify.h"
#include "Animations/AnimUtils.h"
#include "CSTUCoreTypes.h"
#include "CSTUWeaponComponent.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All)

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
    void Reload();

protected:
    // Called when the game starts
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<FWeaponData> WeaponData;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponPoint";

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponArmorySocketName = "ArmorySocket";

    UPROPERTY(EditDefaultsonly, Category = "Animation")
    UAnimMontage* EquipAnimMonatege;

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    UPROPERTY()
    ACSTUBaseWeapon* CurrentWeapon = nullptr;

    UPROPERTY()
    TArray<ACSTUBaseWeapon*> Weapons;

    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;

    int32 CurrentWeaponIndex = 0;
    bool EquipAnimInProgress = false;
    bool ReloadAnimInProgress = false;

    void SpawnWeapons();
    void AttachWeaponToSocket(ACSTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
    void EquipWeapon(int32 WeaponIndex);
    void PlayAnimMontage(UAnimMontage* Animation);
    void InitAnimation();
    void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
    void OnReloadFinished(USkeletalMeshComponent* MeshComponent);
    bool CanFire() const;
    bool CanEquip() const;
    bool CanReload() const;
    void OnClipEmpty();
    void ChangeClip();
};
