// CourseShootThemUp

#include "Weapon/CSTUBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)
#define DebugPrint(text, color) GEngine->AddOnScreenDebugMessage(-1, 5.0f, color, text, true, FVector2D(1.5f, 1.5f));
#define DebugLine(first, second, color)                                                                                                    \
    if (GetWorld()) DrawDebugLine(GetWorld(), first, second, color, false, 10.0f, 0, 3);

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

    FVector CameraLocation;
    FRotator CameraRotation;
    /**
     * GetCamera's ViewPoint
     */
    Controller->GetPlayerViewPoint(CameraLocation, CameraRotation);

    const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);

    const FVector TraceStart = CameraLocation;
    const FVector ShootDirection = CameraRotation.Vector();
    const FVector TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
    if (HitResult.bBlockingHit) {
        /** You can hit enemy behind u.
         * It is bug, so we need to check angle between MeshGun.ForwardVector and vector derection from muzzle gun & ImpactPoint
         * if it is bigger then 30 degrees (can be changed), shoot not register
         **/
        const FVector ActualTraceEnd = HitResult.ImpactPoint;
        const FVector HitDirectionFromMuzzle = (ActualTraceEnd - SocketTransform.GetLocation()).GetSafeNormal();
        const float Degrees = FMath::RadiansToDegrees(
            FMath::Acos(FVector::DotProduct(SocketTransform.GetRotation().GetForwardVector(), HitDirectionFromMuzzle)));
        UE_LOG(LogBaseWeapon, Display, TEXT("Degrees %f"), Degrees);
        if (Degrees > 90.0f) {
            UE_LOG(LogBaseWeapon, Display, TEXT("You cant shoot like this"));
        }
        DebugLine(TraceStart, TraceEnd, FColor::Orange);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0);
    } else {
        DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    }
    UE_LOG(LogBaseWeapon, Display, TEXT("Make shot with check angle"));
}

void ACSTUBaseWeapon::MakeShot1() {

    if (!GetWorld()) return;

    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player) return;

    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller) return;

    FVector CameraLocation;
    FRotator CameraRotation;
    Controller->GetPlayerViewPoint(CameraLocation, CameraRotation);

    const FVector MuzzleLocation = WeaponMesh->GetSocketTransform(MuzzleSocketName).GetLocation();
    const FVector VectorCameraToMuzzle = MuzzleLocation - CameraLocation;
    const FVector ProjectedVector = VectorCameraToMuzzle.ProjectOnToNormal(CameraRotation.Vector());
    const FVector TraceStart = CameraLocation + ProjectedVector;
    const FVector TraceEnd = CameraLocation + CameraRotation.Vector() * TraceMaxDistance;

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
    if (HitResult.bBlockingHit) {
        DrawDebugLine(GetWorld(), CameraLocation, TraceEnd, FColor::Orange, false, 3.0f, 0, 3);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 24, FColor::Red, false, 3.0f);
    } else {
        DrawDebugLine(GetWorld(), MuzzleLocation, TraceEnd, FColor::Orange, false, 3.0f, 0, 3);
    }
    UE_LOG(LogBaseWeapon, Display, TEXT("Make shot with moved TraceStart"));
}
