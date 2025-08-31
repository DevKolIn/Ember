// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberWeaponBase.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Item/RuneItem.h"
#include "Utility/CHelpers.h"
#include "Utility/CLog.h"

// Sets default values
AEmberWeaponBase::AEmberWeaponBase()
{
 	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent(this,&MeshComp,TEXT("Mesh"));
	CHelpers::CreateComponent(this, &Collision, TEXT("Collision"), MeshComp);
	Cooldown = 1.0f;
	Collision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	//Durability = 100.0f;
}

void AEmberWeaponBase::ApplyRune(ARuneItem* Rune)
{
	if (!Rune) return;
	if (EquippedRunes.Num() >= MaxRuneSlots)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("더 이상 룬을 장착할 수 없습니다. (Max slots: %d)"),
			MaxRuneSlots);
		return;
	}
	EquippedRunes.Add(Rune);
	// CurrentDamage += Rune->DamageBonus;
	//UE_LOG(LogTemp, Log, TEXT("Rune Class: %s"), Rune->GetName() );
}

void AEmberWeaponBase::BeginPlay()
{
	OwnerCharacter = CastChecked<ACharacter>(GetOwner());
	Super::BeginPlay();
}

void AEmberWeaponBase::AttachTo(FName InSocketName)
{
	if (OwnerCharacter == nullptr)
	{
		DebugLogE("Owner Character is null");
		return;
	}
	
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
}