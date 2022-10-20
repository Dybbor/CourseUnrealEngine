// CourseShootThemUp

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CSTUDevDamageActor.generated.h"

class USceneComponent;

UCLASS()
class COURSESHOOTTHEMUP_API ACSTUDevDamageActor : public AActor {
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ACSTUDevDamageActor();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USceneComponent* SceneComponent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FColor SphereColor = FColor::Red;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Radius = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Damage = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool DoFullDamage = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UDamageType> DamageType;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
   
};
