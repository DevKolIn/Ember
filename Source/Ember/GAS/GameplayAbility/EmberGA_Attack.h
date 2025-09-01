#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EmberGA_Attack.generated.h"

UCLASS()
class EMBER_API UEmberGA_Attack : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UEmberGA_Attack();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

public:
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
	UFUNCTION()
	void OnCompleteCallback();
	UFUNCTION()
	void OnInterruptedCallback();

protected:
	FName GetNextSection();
	void StartComboTimer();
	void CheckComboInput();

protected:
	UPROPERTY()
	TObjectPtr<class UWeaponDataAsset> CurrentComboData;

protected:
	uint8 CurrentCombo{};
	FTimerHandle ComboTimerHandle;
	bool bHasNextComboInput{};
};
