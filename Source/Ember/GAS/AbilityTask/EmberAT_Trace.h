// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "EmberAT_Trace.generated.h"

class AEmberTA_Trace;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTraceResultDelegate,const FGameplayAbilityTargetDataHandle&,TargetDataHandle);

UCLASS()
class EMBER_API UEmberAT_Trace : public UAbilityTask
{
	GENERATED_BODY()
public:
	UEmberAT_Trace();

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "Jump", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UEmberAT_Trace* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<AEmberTA_Trace> InTargetActorClass);
	void SpawnAndInitializeTargetActor();
	void FinallzeTargetActor();
protected:
	virtual void Activate() override;
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);
public:
	virtual void OnDestroy(bool bInOwnerFinished) override;

public:
	UPROPERTY(BlueprintAssignable)
	FTraceResultDelegate OnComplete;
protected:
	UPROPERTY()
	TSubclassOf<AEmberTA_Trace> TargetActorClass;
	UPROPERTY()
	AEmberTA_Trace* SpawnedTargetActor;
	UPROPERTY(EditAnywhere)
	bool bShowDebug;
};
