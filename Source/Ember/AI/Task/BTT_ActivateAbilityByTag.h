// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BTT_Base.h"
#include "BTT_ActivateAbilityByTag.generated.h"

struct FAbilityEndedData;
class UEmberAbilitySystemComponent;

UCLASS()
class EMBER_API UBTT_ActivateAbilityByTag : public UBTT_Base
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	UFUNCTION()
	void OnAbilityEnd(const FAbilityEndedData& AbilityEndedData);
	
private:
	UPROPERTY(EditAnywhere, Category = "Ability", Meta = (Categories = "Ability.AI"))
	FGameplayTag GameplayTag;
	
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> CachedOwnerComp = nullptr;
	
	TWeakObjectPtr<UEmberAbilitySystemComponent> CachedASC;
	
	FDelegateHandle AbilityEndHandle;
};
