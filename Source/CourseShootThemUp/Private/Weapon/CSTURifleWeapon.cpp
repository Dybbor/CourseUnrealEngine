// CourseShootThemUp


#include "Weapon/CSTURifleWeapon.h"

void ACSTURifleWeapon::StartFire() {
    UE_LOG(LogRiffleWeapon,Display, TEXT("FIRE!"));
    MakeShot();
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ACSTURifleWeapon::MakeShot, TimeBetweenShots, true);
}

void ACSTURifleWeapon::StopFire() {
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

bool ACSTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const {
    FVector CameraLocation;
    FRotator CameraRotation;
    if (!GetPlayerCameraPoint(CameraLocation, CameraRotation)) return false;

    const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
    TraceStart = CameraLocation;
    const FVector ShootDirection = FMath::VRandCone(CameraRotation.Vector(), HalfRad);
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

void ACSTURifleWeapon::MakeShot() {
    if (!GetWorld()) return;

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd)) return;

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    UE_LOG(LogRiffleWeapon, Display, TEXT("Make shot with check angle"));

    if (HitResult.bBlockingHit) {
        const FVector MuzzleForwardVector = WeaponMesh->GetSocketTransform(MuzzleSocketName).GetRotation().GetForwardVector();
        const FVector FromMuzzleToHitPointNormal = (HitResult.ImpactPoint - GetMuzzleWorldLocation()).GetSafeNormal();
        /** You can hit enemy behind u.
         * It is bug, so we need to check angle between MeshGun.MuzzleForwardVector and vector direction from muzzle gun & ImpactPoint
         * if it is bigger then 30 degrees (can be changed), shoot not register
         **/
        if (CheckAngleHit(MuzzleForwardVector, FromMuzzleToHitPointNormal)) {
            UE_LOG(LogRiffleWeapon, Warning, TEXT("Shoot is succeed (No logic, need to add functionality)"));
            const auto DamagedActor = HitResult.GetActor();
            if (IsEnemy(DamagedActor)) {
                MakeDamage(DamagedActor);
            } else {
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "it is no enemy", true, FVector2D(1.5f, 1.5f));
            }
        } else {
            UE_LOG(LogRiffleWeapon, Error, TEXT("Shoot incorrect (No logic, need to add functionality)"));
        }
        DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Orange,false, 3.0f, 0.0f, 3.0f);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0);
    } else {
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    }
}