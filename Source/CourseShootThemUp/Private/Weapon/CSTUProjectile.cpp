// CourseShootThemUp

#include "Weapon/CSTUProjectile.h"

// Sets default values
ACSTUProjectile::ACSTUProjectile() {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    CollisionComponent->InitSphereRadius(5.0f);
    // QueryOnly only notification
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
    SetRootComponent(CollisionComponent);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
    MovementComponent->InitialSpeed = 2000;
    // Gravity scale multiply on g - free fall (physics g*t^2)
    MovementComponent->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned
void ACSTUProjectile::BeginPlay() {
    Super::BeginPlay();
    check(MovementComponent);
    check(CollisionComponent);
    MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
    CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
    CollisionComponent->OnComponentHit.AddDynamic(this, &ACSTUProjectile::OnProjectileHit);
    SetLifeSpan(LifeSeconds);
}

void ACSTUProjectile::OnProjectileHit(
    UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
    if (!GetWorld()) return;
    MovementComponent->StopMovementImmediately();
    // make damage
    /* {}  - list of enemy, which can be damage, empty list - all actors get the damage */
    UGameplayStatics::ApplyRadialDamage(GetWorld(),  //
        DamageAmount,                                //
        GetActorLocation(),                          //
        DamageRadius,                                //
        UDamageType::StaticClass(),                  //
        {GetOwner()},                                //
        this,                                        //
        GetController(),                             //
        DoFullDamage);
    DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 5.0f);
    // destroy
    Destroy();
}

AController* ACSTUProjectile::GetController() const {
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}