// CourseShootThemUp

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CSTUHealthComponent.h"
#include "CSTUPlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class COURSESHOOTTHEMUP_API UCSTUPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetHealthPercent() const;
};
