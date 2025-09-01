// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTT_Base.h"
#include "BTT_ActivateRandomAbilityByTags.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API UBTT_ActivateRandomAbilityByTags : public UBTT_Base
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Ability", Meta = (Categories = "Ability.AI"))
	TArray<FGameplayTag> GameplayTags;
};
