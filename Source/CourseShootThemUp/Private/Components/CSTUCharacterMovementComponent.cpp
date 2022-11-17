// CourseShootThemUp

#include "Components/CSTUCharacterMovementComponent.h"
#include "Player/CSTUBaseCharacter.h"

float UCSTUCharacterMovementComponent::GetMaxSpeed() const {

    const float MaxSpeed = Super::GetMaxSpeed();
    const ACSTUBaseCharacter* Player = Cast<ACSTUBaseCharacter>(GetPawnOwner());
    return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}
