// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Base.generated.h"

class AMonsterAIBase;

UCLASS()
class EMBER_API UBTT_Base : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTT_Base(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
protected:
	virtual void SetOwner(AActor* InActorOwner) override;

protected:
	/** Cached AIController owner of BehaviorTreeComponent. */
	UPROPERTY(Transient)
	TObjectPtr<AAIController> AIOwner;

	/** Cached actor owner of BehaviorTreeComponent. */
	UPROPERTY(Transient)
	TObjectPtr<AMonsterAIBase> ActorOwner;
};
