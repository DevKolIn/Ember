// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SwitchBehaviorTree.h"

#include "AIController.h"

EBTNodeResult::Type UBTT_SwitchBehaviorTree::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	if (!AIOwner)
	{
		return EBTNodeResult::Failed;
	}

	// 2. 현재 실행 중인 BehaviorTreeComponent 확인
	UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(AIOwner->GetBrainComponent());
	if (!BTComp)
	{
		return EBTNodeResult::Failed;
	}

	// 3. 기존 트리 중단 및 새 트리 실행
	BTComp->StopTree(EBTStopMode::Safe);
	AIOwner->RunBehaviorTree(NewBehaviorTree);

	return EBTNodeResult::Succeeded;
}
