// CourseShootThemUp


#include "Weapon/CSTULauncherWeapon.h"

void ACSTULauncherWeapon::StartFire() {
    MakeShot();
}

void ACSTULauncherWeapon::MakeShot() {
    if (!GetWorld() ||  IsAmmoEmpty()) return;

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd)) return;

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    const auto EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
    const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();
    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
    ACSTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ACSTUProjectile>(ProjectileClass, SpawnTransform);
    if (Projectile) {
        Projectile->SetShotDirection(Direction);
        Projectile->SetOwner(GetOwner());
        Projectile->FinishSpawning(SpawnTransform);
    }
    DecreaseAmmo();
}
