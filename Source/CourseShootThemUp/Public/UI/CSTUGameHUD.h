// CourseShootThemUp

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "CSTUGameHUD.generated.h"

/**
 *
 */
UCLASS()
class COURSESHOOTTHEMUP_API ACSTUGameHUD : public AHUD {
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;

private:
    void DrawCrosshair();
};
