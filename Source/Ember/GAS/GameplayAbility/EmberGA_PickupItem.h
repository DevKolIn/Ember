#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EmberGA_PickupItem.generated.h"

UCLASS()
class EMBER_API UEmberGA_PickupItem : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UEmberGA_PickupItem();

protected:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;
};
