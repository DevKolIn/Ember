// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EmberPlayerState.h"
#include "Utility/CHelpers.h"
#include "AbilitySystemComponent.h"
#include "GAS/Attribute/EmberAS_Player.h"

AEmberPlayerState::AEmberPlayerState()
{
	CHelpers::CreateActorComponent(this, &ASC, "Ability System Component");
	ASC->SetIsReplicated(true);
	AttributeSet = CreateDefaultSubobject<UEmberAS_Player>("Attribute Set");
}

UEmberAbilitySystemComponent* AEmberPlayerState::GetAbilitySystemComponent() const
{
	return  ASC;
}
