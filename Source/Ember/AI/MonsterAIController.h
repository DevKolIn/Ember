// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/AIModule/Classes/AIController.h"
#include "MonsterAIController.generated.h"

enum EGameTeamID :uint8
{
	Monster = 0,
	Team1
};

UCLASS()
class EMBER_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMonsterAIController();

protected:
	//~ AAIController Override
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* Possessed) override;
	virtual void OnUnPossess() override;
	//~ End of AAIController Override

/* 팀 설정 */
public:
	//~ IGenericTeamAgentInterface interface
	virtual FGenericTeamId GetGenericTeamId() const override;
	//~ End of IGenericTeamAgentInterface interface
	
/* AI 감지 */
protected:
	UFUNCTION()
	virtual void OnTargetPerceptionUpdated(AActor* PerceivedActor, struct FAIStimulus Stimulus);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UAIPerceptionComponent> AIPerceptionComponent;
	
/* AI 감지 - 시야 */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UAISenseConfig_Sight> AISenseConfigSight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UAISenseConfig_Damage> AISenseConfigDamage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI|Sight")
	float DetectionRadius = 1500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI|Sight")
	float LoseInterestRadius = 2000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI|Sight")
	float SightAngleDegree = 60.f;
};
