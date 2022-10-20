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
    virtual void Fire();
    void MakeShot();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapons")
    FName MuzzleSocketName = "MazzleSocket";

    /**
    * 1 unit = 1 cm, 1500 = 15m
    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
    float TraceMaxDistance = 1500.0f;
};
