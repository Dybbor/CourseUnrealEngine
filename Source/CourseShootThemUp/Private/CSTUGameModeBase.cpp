// CourseShootThemUp

#include "CSTUGameModeBase.h"

ACSTUGameModeBase::ACSTUGameModeBase() {
    DefaultPawnClass = ACSTUBaseCharacter::StaticClass();
    PlayerControllerClass = ACSTUBasePlayerController::StaticClass();
    HUDClass = ACSTUGameHUD::StaticClass();
}
