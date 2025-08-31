// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Base.h"

#include "AIController.h"
#include "AI/Monster/MonsterAIBase.h"

UBTT_Base::UBTT_Base(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
}

void UBTT_Base::SetOwner(AActor* InActorOwner)
{
	AIOwner = Cast<AAIController>(InActorOwner);
	ActorOwner = Cast<AMonsterAIBase>(AIOwner->GetPawn());
}
