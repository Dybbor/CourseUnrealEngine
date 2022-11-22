// CourseShootThemUp

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"
#include "CSTUGameHUD.generated.h"

/**
 *
 */
UCLASS()
class COURSESHOOTTHEMUP_API ACSTUGameHUD : public AHUD {
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PlayerHUDWidgetClass;
    
    virtual void BeginPlay() override;

private:
    void DrawCrosshair();
};
