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
    SpawnWeapon();
}

void UCSTUWeaponComponent::SpawnWeapon() {
    if (!GetWorld()) return;
    CurrentWeapon = GetWorld()->SpawnActor<ACSTUBaseWeapon>(WeaponClass);
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    // if weapon spawned, spawn return pointer on weapon. If pointer is null, weapon not spawned
    if (!CurrentWeapon) return;

    UE_LOG(BaseWeaponComponent, Display, TEXT("Weapon spawned!!!"));
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttachPointName);
    CurrentWeapon->SetOwner(Character);
}

void UCSTUWeaponComponent::Fire() {
    if (!CurrentWeapon) return;
    CurrentWeapon->Fire(); 
}