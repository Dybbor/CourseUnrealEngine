// CourseShootThemUp

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Player/CSTUBaseCharacter.h"
#include "Player/CSTUBasePlayerController.h"
#include "UI/CSTUGameHUD.h"
#include "CSTUGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class COURSESHOOTTHEMUP_API ACSTUGameModeBase : public AGameModeBase {
    GENERATED_BODY()
public:
    ACSTUGameModeBase();
};
