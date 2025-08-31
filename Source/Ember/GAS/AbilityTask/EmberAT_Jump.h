// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "EmberAT_Jump.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEmberJump);

UCLASS()
class EMBER_API UEmberAT_Jump : public UAbilityTask
{
	GENERATED_BODY()
public:
	UEmberAT_Jump();
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "Jump", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UEmberAT_Jump* CreateTask(UGameplayAbility* OwningAbility);

protected:
	virtual void Activate() override;

public:
	virtual void OnDestroy(bool bInOwnerFinished) override;
	UPROPERTY(BlueprintAssignable)
	FEmberJump OnComplete;

protected:
	UFUNCTION()
	void OnLandedCallback(const FHitResult& Hit);
};
