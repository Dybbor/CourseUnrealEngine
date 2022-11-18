// CourseShootThemUp

#include "Components/CSTUWeaponComponent.h"

// Sets default values for this component's properties
UCSTUWeaponComponent::UCSTUWeaponComponent() {
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;

    // ...
}

// Called when the game starts
void UCSTUWeaponComponent::BeginPlay() {
    Super::BeginPlay();
    CurrentWeaponIndex = 0;
    SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex);
}

void UCSTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    CurrentWeapon = nullptr;
    for (auto Weapon : Weapons) {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }
    Weapons.Empty();

    Super::EndPlay(EndPlayReason);
}

void UCSTUWeaponComponent::SpawnWeapons() {

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || !GetWorld()) return;
    for (auto WeaponClass : WeaponClasses) {
        auto Weapon = GetWorld()->SpawnActor<ACSTUBaseWeapon>(WeaponClass);
        if (!Weapon) continue;

        Weapon->SetOwner(Character);
        Weapons.Add(Weapon);

        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
    }
    // if weapon spawned, spawn return pointer on weapon. If pointer is null, weapon not spawned
    if (!CurrentWeapon) return;
}

void UCSTUWeaponComponent::AttachWeaponToSocket(ACSTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName) {
    if (!Weapon || !SceneComponent) return;
    UE_LOG(BaseWeaponComponent, Display, TEXT("Weapon %s spawned!!!"), *(Weapon->GetName()));
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void UCSTUWeaponComponent::EquipWeapon(int32 WeaponIndex) {
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    if (CurrentWeapon) {
        CurrentWeapon->StopFire();
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }

    CurrentWeapon = Weapons[WeaponIndex];
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
}

void UCSTUWeaponComponent::StartFire() {
    if (!CurrentWeapon) return;
    CurrentWeapon->StartFire();
}

void UCSTUWeaponComponent::StopFire() {
    if (!CurrentWeapon) return;
    CurrentWeapon->StopFire();
}

void UCSTUWeaponComponent::NextWeapon() {
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}