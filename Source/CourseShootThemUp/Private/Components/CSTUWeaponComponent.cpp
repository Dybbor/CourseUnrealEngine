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
    InitAnimation();
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
    EquipAnimInProgress = true;
    PlayAnimMontage(EquipAnimMonatege);
}

void UCSTUWeaponComponent::StartFire() {
    if (!CanFire()) return;
    CurrentWeapon->StartFire();
}

void UCSTUWeaponComponent::StopFire() {
    if (!CanFire()) return;
    CurrentWeapon->StopFire();
}

void UCSTUWeaponComponent::NextWeapon() {
    if (!CanEquip()) return;
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

void UCSTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation) {
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;
    Character->PlayAnimMontage(Animation);
}

void UCSTUWeaponComponent::InitAnimation() {
    if (!EquipAnimMonatege) return;
    const auto NotifyEvents = EquipAnimMonatege->Notifies;
    for (auto NotifyEvent : NotifyEvents) {
        auto EquipFinishNotify = Cast<UCSTUEquipFinishAnimNotify>(NotifyEvent.Notify);
        if (EquipFinishNotify) {
            EquipFinishNotify->OnNotified.AddUObject(this, &UCSTUWeaponComponent::OnEquipFinished);
            break;
        }
    }
}
void UCSTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent) {
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || MeshComponent != Character->GetMesh()) return;
    EquipAnimInProgress = false;
}

bool UCSTUWeaponComponent::CanFire() const {
    return CurrentWeapon && !EquipAnimInProgress;
}

bool UCSTUWeaponComponent::CanEquip() const {
    return !EquipAnimInProgress;
}
