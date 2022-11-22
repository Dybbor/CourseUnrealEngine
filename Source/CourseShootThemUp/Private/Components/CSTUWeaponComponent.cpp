// CourseShootThemUp

#include "Components/CSTUWeaponComponent.h"

const static int32 WeaponNum = 2;
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

    checkf(WeaponData.Num() == WeaponNum, TEXT("Our character can hold only %i weapon items"), WeaponNum);
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
    for (auto OneWeaponData : WeaponData) {
        auto Weapon = GetWorld()->SpawnActor<ACSTUBaseWeapon>(OneWeaponData.WeaponClass);
        if (!Weapon) continue;

        Weapon->OnClipEmpty.AddUObject(this, &UCSTUWeaponComponent::OnClipEmpty);
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
    if (WeaponIndex < 0 || WeaponIndex >= WeaponData.Num()) {
        UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid weapon index"));
        return;
    }
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    if (CurrentWeapon) {
        CurrentWeapon->StopFire();
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }

    CurrentWeapon = Weapons[WeaponIndex];
    // CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;
    const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) {  //
        return Data.WeaponClass == CurrentWeapon->GetClass();                                 //
    });
    CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

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

void UCSTUWeaponComponent::Reload() {
    ChangeClip();
}

void UCSTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation) {
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;
    Character->PlayAnimMontage(Animation);
}

void UCSTUWeaponComponent::InitAnimation() {
    auto EquipFinishAnimNotify = AnimUtils::FindAnimNotifyByClass<UCSTUEquipFinishAnimNotify>(EquipAnimMonatege);
    if (EquipFinishAnimNotify) {
        EquipFinishAnimNotify->OnNotified.AddUObject(this, &UCSTUWeaponComponent::OnEquipFinished);
    } else {
        UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));
        checkNoEntry();
    }
    for (auto OneWeaponData : WeaponData) {
        auto ReloadFinishAnimNotify = AnimUtils::FindAnimNotifyByClass<UCSTUReloadFinishAnimNotify>(OneWeaponData.ReloadAnimMontage);
        if (!ReloadFinishAnimNotify) {
            UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim notify is forgotten to set"));
            checkNoEntry();
        };
        ReloadFinishAnimNotify->OnNotified.AddUObject(this, &UCSTUWeaponComponent::OnReloadFinished);
    }
}
void UCSTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent) {
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || MeshComponent != Character->GetMesh()) return;
    EquipAnimInProgress = false;
}

void UCSTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent) {
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || MeshComponent != Character->GetMesh()) return;
    ReloadAnimInProgress = false;
}

bool UCSTUWeaponComponent::CanFire() const {
    return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool UCSTUWeaponComponent::CanEquip() const {
    return !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool UCSTUWeaponComponent::CanReload() const {
    return CurrentWeapon             //
           && !EquipAnimInProgress   //
           && !ReloadAnimInProgress  //
           && CurrentWeapon->CanReload();
}

void UCSTUWeaponComponent::OnClipEmpty() {
    ChangeClip();
}
void UCSTUWeaponComponent::ChangeClip() {
    if (!CanReload()) return;
    CurrentWeapon->StopFire();
    CurrentWeapon->ChangeClip();
    ReloadAnimInProgress = true;
    PlayAnimMontage(CurrentReloadAnimMontage);
}