// CourseShootThemUp

#include "Weapon/CSTUBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)
#define DebugPrint(text, color) GEngine->AddOnScreenDebugMessage(-1, 5.0f, color, text, true, FVector2D(1.5f, 1.5f));
#define DebugLine(first, second, color)                                                                                                    \
    if (GetWorld()) DrawDebugLine(GetWorld(), first, second, color, false, 3.0f, 0, 3);

// Sets default values
ACSTUBaseWeapon::ACSTUBaseWeapon() {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

// Called when the game starts or when spawned
void ACSTUBaseWeapon::BeginPlay() {
    Super::BeginPlay();
    check(WeaponMesh);
}

void ACSTUBaseWeapon::Fire() {
    UE_LOG(LogBaseWeapon, Display, TEXT("FIRE!"));
    MakeShot();
}

void ACSTUBaseWeapon::MakeShot() {
    if (!GetWorld()) return;

    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player) return;

    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller) return;

    FVector ViewLocation;
    FRotator ViewRotation;
    /**
     * GetCamera's ViewPoint
     */
    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);

    const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);

    const FVector TraceStart = ViewLocation;
    const FVector ShootDirection = ViewRotation.Vector();
    const FVector TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    //DrawDebugLine(GetWorld(), ViewLocation, FVector::ZeroVector, FColor::Red, false, 5.0f, 0, 3);
    //DrawDebugLine(GetWorld(), ShootDirection * TraceMaxDistance, FVector::ZeroVector, FColor::Green, false, 5.0f, 0, 3);
    //DebugLine(TraceStart, TraceStart + ShootDirection * TraceMaxDistance, FColor::Orange);
     FHitResult HitResult;
     FCollisionQueryParams CollisionParams;
     CollisionParams.AddIgnoredActor(GetOwner());
     GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
     if (HitResult.bBlockingHit) {
         DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
         DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0);
         UE_LOG(LogBaseWeapon, Display, TEXT("Bone %s "), *HitResult.BoneName.ToString());
     } else {
         DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
     }
}
