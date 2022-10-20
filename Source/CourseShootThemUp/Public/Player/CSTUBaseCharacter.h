// CourseShootThemUp

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/TextRenderComponent.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Controller.h"
#include "Components/CSTUCharacterMovementComponent.h"
#include "Components/CSTUHealthComponent.h"
#include "Components/CSTUWeaponComponent.h"
#include "CSTUBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UCSTUHealthComponent;
class UTextRenderComponent;
class UAnimMontage;
class ACSTUBaseWeapon;
class UCSTUWeaponComponent;

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All)

UCLASS()
class COURSESHOOTTHEMUP_API ACSTUBaseCharacter : public ACharacter {
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ACSTUBaseCharacter(const FObjectInitializer& ObjectInitializer);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCSTUHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTextRenderComponent* HealthTextComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCSTUWeaponComponent* WeaponComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Animations")
    UAnimMontage* DeathAnimMontage;



    /**
    * Limited speed velocity z
    * When character fall with 900 cm/s, he dont take damage
    * When character fall with 900 - 1200 cm/s, damage will be scaled by Velocity.Z(Height)
    * When character fall with 1200 cm/s, he give maximum damage(character die)
    */
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsRunning() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

private:
    bool WantToRun = false;
    bool IsMovingForward = false;
    void MoveForward(float Scale);
    void MoveRight(float Scale);
    void OnStartRunning();
    void OnEndRunning();
    void OnDeath();
    void OnHealthChanged(float Health);

    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);

    
};
