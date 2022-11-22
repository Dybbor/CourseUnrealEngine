// CourseShootThemUp


#include "UI/CSTUPlayerWidget.h"

float UCSTUPlayerWidget::GetHealthPercent() const {
    const auto Player = GetOwningPlayerPawn();
    if (!Player) return 0.0;
    const auto Component = Player->GetComponentByClass(UCSTUHealthComponent::StaticClass());
    const auto HealthComponent = Cast<UCSTUHealthComponent>(Component);
    if (!HealthComponent) return 0;
    return HealthComponent->GetHealthPercent();
}
