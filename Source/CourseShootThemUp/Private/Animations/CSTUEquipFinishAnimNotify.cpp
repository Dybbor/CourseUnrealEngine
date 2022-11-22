// CourseShootThemUp

#include "Animations/CSTUEquipFinishAnimNotify.h"

void UCSTUEquipFinishAnimNotify::Notify(
    USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
    OnNotified.Broadcast(MeshComp);
    Super::Notify(MeshComp, Animation, EventReference);
}