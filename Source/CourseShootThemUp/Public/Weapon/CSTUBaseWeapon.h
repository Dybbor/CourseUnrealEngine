// CourseShootThemUp

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "CSTUBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class COURSESHOOTTHEMUP_API ACSTUBaseWeapon : public AActor {
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ACSTUBaseWeapon();
    virtual void StartFire();
    virtual void StopFire();
    virtual void MakeShot();   // Make shot through angles between Muzzle.ForwardVector and VectorMuzzleToImpactPoint
    void MakeShot1();  // Make shot by moved start point (projected VectorCameraToMuzzle to CameraRotation)
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapons")
    FName MuzzleSocketName = "MuzzleSocket";

    /**
     * 1 unit = 1 cm, 1500 = 15m
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
    float TraceMaxDistance = 1500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DamageAmount = 10.0f;

    virtual void BeginPlay() override;
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    APlayerController* GetPlayerController() const;
    bool GetPlayerCameraPoint(FVector& CameraLocation, FRotator& CameraRotation) const;
    FVector GetMuzzleWorldLocation() const;
    bool CheckAngleHit(const FVector& First, const FVector& Second) const;
    bool IsEnemy(const AActor* Actor) const;
    void MakeDamage(AActor* Enemy);
    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;
};
