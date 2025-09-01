// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MonsterAIMovementComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class EMBER_API UMonsterAIMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UMonsterAIMovementComponent();

	virtual void InitializeComponent() override;
	
protected:
	virtual void BeginPlay() override;
};
