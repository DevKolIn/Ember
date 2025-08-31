// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Data/MonsterAIData.h"
#include "Data/MonsterAITemplate.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Runtime/AIModule/Classes/Perception/AIPerceptionComponent.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h"
#include "Utility/CHelpers.h"

AMonsterAIController::AMonsterAIController()
{
	bWantsPlayerState = true;
	// PrimaryActorTick.bCanEverTick = false;

	CHelpers::CreateActorComponent(this, &AISenseConfigSight, TEXT("AISenseConfigSight"));
	AISenseConfigSight->SightRadius = DetectionRadius;
	AISenseConfigSight->LoseSightRadius = LoseInterestRadius;
	AISenseConfigSight->PeripheralVisionAngleDegrees = SightAngleDegree;

	CHelpers::CreateActorComponent(this, &AISenseConfigDamage, TEXT("AISenseConfigDamage"));
	
	CHelpers::CreateActorComponent(this, &AIPerceptionComponent, TEXT("AIPerceptionComponent"));
	AIPerceptionComponent->ConfigureSense(*AISenseConfigSight);
	AIPerceptionComponent->ConfigureSense(*AISenseConfigDamage);
	AIPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnTargetPerceptionUpdated);
}

void AMonsterAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AMonsterAIController::OnPossess(APawn* Possessed)
{
	Super::OnPossess(Possessed);

	if (Possessed == nullptr)
		return;

	const UMonsterAITemplate& MonsterAITemplate = UMonsterAIData::Get().FindMonsterAITemplateByClass(Possessed->GetClass());
	RunBehaviorTree(MonsterAITemplate.BehaviorTree);
}

void AMonsterAIController::OnUnPossess()
{
	AIPerceptionComponent->OnTargetPerceptionUpdated.RemoveDynamic(this, &ThisClass::OnTargetPerceptionUpdated);
	
	Super::OnUnPossess();
}

FGenericTeamId AMonsterAIController::GetGenericTeamId() const
{
	return FGenericTeamId((uint8)EGameTeamID::Monster);
}

void AMonsterAIController::OnTargetPerceptionUpdated(AActor* PerceivedActor, FAIStimulus Stimulus)
 {
	if (PerceivedActor && Stimulus.WasSuccessfullySensed())
	{
		if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
		{
			BlackboardComponent->SetValueAsObject(FName("TargetActor"), PerceivedActor);
		}
	}
 }

