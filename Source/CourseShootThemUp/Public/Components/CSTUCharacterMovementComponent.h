// CourseShootThemUp

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CSTUCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class COURSESHOOTTHEMUP_API UCSTUCharacterMovementComponent : public UCharacterMovementComponent
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "1.5", ClampMax = "10"))
    float RunModifier = 2.0f;
    virtual float GetMaxSpeed() const override;
};
