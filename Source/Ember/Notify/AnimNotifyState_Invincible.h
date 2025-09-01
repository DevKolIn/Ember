// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_Invincible.generated.h"

UCLASS()
class EMBER_API UAnimNotifyState_Invincible : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual ::FString GetNotifyName_Implementation() const override;
	virtual void NotifyBegin(::USkeletalMeshComponent* MeshComp, ::UAnimSequenceBase* Animation, float TotalDuration,
		const ::FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(::USkeletalMeshComponent* MeshComp, ::UAnimSequenceBase* Animation,
		const ::FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditAnywhere)
	FGameplayTag EnableInvincibleTag;
	UPROPERTY(EditAnywhere)
	FGameplayTag DisableInvincibleTag;
};
