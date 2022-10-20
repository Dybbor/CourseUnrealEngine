// CourseShootThemUp

#include "Components/CSTUHealthComponent.h"

#include "GameFramework/Actor.h"
// Sets default values for this component's properties
UCSTUHealthComponent::UCSTUHealthComponent() {
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;

    // ...
}

// Called when the game starts
void UCSTUHealthComponent::BeginPlay() {
    Super::BeginPlay();

    SetHealth(MaxHealth);
    AActor* ComponentOwner = GetOwner();
    if (ComponentOwner) {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UCSTUHealthComponent::OnTakeAnyDamageHandle);
    }
}

void UCSTUHealthComponent::OnTakeAnyDamageHandle(
    AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) {
    if (Damage <= 0.f || IsDead() || !GetWorld()) return;
    SetHealth(Health - Damage);
    GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    if (IsDead()) {
        OnDeath.Broadcast();
    } else if (AutoHeal) {
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UCSTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
    }
}

void UCSTUHealthComponent::HealUpdate() {
    SetHealth(Health + HealModifier);
    if (FMath::IsNearlyEqual(Health,MaxHealth) && GetWorld()) {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    }
}

void UCSTUHealthComponent::SetHealth(float NewHealth) {
    Health = FMath::Clamp(NewHealth, 0 , MaxHealth);
    OnHealthChanged.Broadcast(Health);
}
