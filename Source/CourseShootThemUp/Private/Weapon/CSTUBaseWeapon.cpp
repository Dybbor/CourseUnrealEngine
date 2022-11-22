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
    CurrentAmmo = DefaultAmmo;
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

bool ACSTUBaseWeapon::CheckAngleHit(const FVector& First, const FVector& Second) const {
    const float Degrees = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(First, Second)));
    return (Degrees > 90.0f) ? false : true;
}

bool ACSTUBaseWeapon::IsEnemy(const AActor* Actor) const {
    const ACharacter* DamagedActor = Cast<ACharacter>(Actor);
    return (!DamagedActor) ? false : DamagedActor->GetName().Contains("BaseCharacter");
}    

void ACSTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const {
    if (!GetWorld()) return;
    FCollisionQueryParams CollisionParams;

    CollisionParams.AddIgnoredActor(GetOwner());
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

void ACSTUBaseWeapon::DecreaseAmmo() {
    if (CurrentAmmo.Bullets == 0) {
        UE_LOG(LogBaseWeapon, Warning, TEXT("Clip is empty"));
    }
    CurrentAmmo.Bullets--;
    LogAmmo();
    if (IsClipEmpty() && !IsAmmoEmpty()) {
        StopFire();
        OnClipEmpty.Broadcast();
    }
}

bool ACSTUBaseWeapon::IsAmmoEmpty() const {
    return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ACSTUBaseWeapon::IsClipEmpty() const {
    return CurrentAmmo.Bullets == 0;
}

void ACSTUBaseWeapon::ChangeClip() {
    if (!CurrentAmmo.Infinite) {
        if (CurrentAmmo.Clips == 0) {
            UE_LOG(LogBaseWeapon, Warning, TEXT("No more clips"));
        }
        CurrentAmmo.Clips--;
        UE_LOG(LogBaseWeapon, Display, TEXT("ChangeClip"));
    }
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
}

void ACSTUBaseWeapon::LogAmmo() const {
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + "  ";
    AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
    UE_LOG(LogBaseWeapon, Display, TEXT("%s"),*AmmoInfo);
}

bool ACSTUBaseWeapon::CanReload() const {
    return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}