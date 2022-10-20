// CourseShootThemUp


#include "DevelopmentContent/CSTUDevDamageActor.h"
#include "DrawDebugHelpers.h"

// Sets default values
ACSTUDevDamageActor::ACSTUDevDamageActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SetRootComponent(SceneComponent);

}

// Called when the game starts or when spawned 
void ACSTUDevDamageActor::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called every frame
void ACSTUDevDamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 24, SphereColor);
    /* {}  - list of enemy, which can be damage, empty list - all actors get the damage */
    UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radius, DamageType, {}, this, nullptr, DoFullDamage);
}

