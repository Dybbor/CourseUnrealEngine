// CourseShootThemUp

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "CSTUCoreTypes.h"
#include "CSTUHealthComponent.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COURSESHOOTTHEMUP_API UCSTUHealthComponent : public UActorComponent {
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UCSTUHealthComponent();
    float GetHealth() const { return Health; }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
    bool AutoHeal = true;

    /**
     * The frequancy, which add healt point
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal", ClampMin = "0", ClampMax = "60"))
    float HealUpdateTime = 0.3f;

    /**
     * Time, after it autohealth on
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal", ClampMin = "0", ClampMax = "10"))
    float HealDelay = 3.0f;

    /**
     * Coeff health
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealModifier = 1.0;

    UFUNCTION(BlueprintCallable)
    bool IsDead() const { return FMath::IsNearlyZero(Health); };

    UFUNCTION(BlueprintCallable)
    float GetHealthPercent() const { return Health / MaxHealth; }

    // declare Delegate
    FOnDeath OnDeath;
    FOnHealthChanged OnHealthChanged;

protected:
    // Called when the game starts
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0", ClampMax = "1000"))
    float MaxHealth = 100.0f;

    virtual void BeginPlay() override;

private:
    float Health = 0.0f;
    FTimerHandle HealTimerHandle;
    UFUNCTION()
    void OnTakeAnyDamageHandle(
        AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
    void HealUpdate();
    void SetHealth(float NewHealth);
};
