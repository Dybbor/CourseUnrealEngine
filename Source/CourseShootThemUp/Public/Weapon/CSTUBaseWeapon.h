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

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapons")
    FName MuzzleSocketName = "MuzzleSocket";

    /**
     * 1 unit = 1 cm, 1500 = 15m
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapons")
    float TraceMaxDistance = 1500.0f;

    virtual void BeginPlay() override;
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
    virtual void MakeShot(){};

    APlayerController* GetPlayerController() const;
    bool GetPlayerCameraPoint(FVector& CameraLocation, FRotator& CameraRotation) const;
    FVector GetMuzzleWorldLocation() const;
    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;
    bool CheckAngleHit(const FVector& First, const FVector& Second) const;
    bool IsEnemy(const AActor* Actor) const;
};
