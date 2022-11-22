// CourseShootThemUp

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CSTUEquipFinishAnimNotify.generated.h"

/**
 *
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifySignature, USkeletalMeshComponent*)
UCLASS()
class COURSESHOOTTHEMUP_API UCSTUEquipFinishAnimNotify : public UAnimNotify {
    GENERATED_BODY()
public:
    FOnNotifySignature OnNotified;
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
