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

void ACSTUBaseWeapon::StartFire() {}

void ACSTUBaseWeapon::StopFire() {}

APlayerController* ACSTUBaseWeapon::GetPlayerController() const {

    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player) return nullptr;
    return Player->GetController<APlayerController>();
}

bool ACSTUBaseWeapon::GetPlayerCameraPoint(FVector& CameraLocation, FRotator& CameraRotation) const {
    const APlayerController* Controller = GetPlayerController();
    if (!Controller) return false;
    Controller->GetPlayerViewPoint(CameraLocation, CameraRotation);
    return true;
}

FVector ACSTUBaseWeapon::GetMuzzleWorldLocation() const {
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ACSTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const {
    FVector CameraLocation;
    FRotator CameraRotation;
    if (!GetPlayerCameraPoint(CameraLocation, CameraRotation)) return false;

    TraceStart = CameraLocation;
    const FVector ShootDirection = CameraRotation.Vector();
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

void ACSTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const {
    if (!GetWorld()) return;
    FCollisionQueryParams CollisionParams;

    CollisionParams.AddIgnoredActor(GetOwner());
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

bool ACSTUBaseWeapon::CheckAngleHit(const FVector& First, const FVector& Second) const {
    const float Degrees = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(First, Second)));
    return (Degrees > 90.0f) ? false : true;
}

bool ACSTUBaseWeapon::IsEnemy(const AActor* Actor) const {
    const ACharacter* DamagedActor = Cast<ACharacter>(Actor);
    return (!DamagedActor) ? false : DamagedActor->GetName().Contains("BaseCharacter");
}

void ACSTUBaseWeapon::MakeDamage(AActor* Enemy) {
    Enemy->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}

void ACSTUBaseWeapon::MakeShot() {
    if (!GetWorld()) return;

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd)) return;

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    UE_LOG(LogBaseWeapon, Display, TEXT("Make shot with check angle"));

    if (HitResult.bBlockingHit) {
        const FVector MuzzleForwardVector = WeaponMesh->GetSocketTransform(MuzzleSocketName).GetRotation().GetForwardVector();
        const FVector FromMuzzleToHitPointNormal = (HitResult.ImpactPoint - GetMuzzleWorldLocation()).GetSafeNormal();
        /** You can hit enemy behind u.
         * It is bug, so we need to check angle between MeshGun.MuzzleForwardVector and vector direction from muzzle gun & ImpactPoint
         * if it is bigger then 30 degrees (can be changed), shoot not register
         **/
        if (CheckAngleHit(MuzzleForwardVector, FromMuzzleToHitPointNormal)) {
            UE_LOG(LogBaseWeapon, Warning, TEXT("Shoot is succeed (No logic, need to add functionality)"));
            const auto DamagedActor = HitResult.GetActor();
            if (IsEnemy(DamagedActor)) {
                MakeDamage(DamagedActor);
            } else {
                DebugPrint("it is not enemy", FColor::Red);
            }
        } else {
            UE_LOG(LogBaseWeapon, Error, TEXT("Shoot incorrect (No logic, need to add functionality)"));
        }
        DebugLine(TraceStart, TraceEnd, FColor::Orange);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0);
    } else {
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    }
}

/**
 * Just another idea how to shoot, dont use in project(only for learning)
 * TraceStart move on projection CameraLocation + CameraRotation * TraceMaxDistance
 */
void ACSTUBaseWeapon::MakeShot1() {

    if (!GetWorld()) return;

    FVector CameraLocation;
    FRotator CameraRotation;
    GetPlayerCameraPoint(CameraLocation, CameraRotation);

    const FVector VectorCameraToMuzzle = GetMuzzleWorldLocation() - CameraLocation;
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
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Orange, false, 3.0f, 0, 3);
    }
    UE_LOG(LogBaseWeapon, Display, TEXT("Make shot with moved TraceStart"));
}