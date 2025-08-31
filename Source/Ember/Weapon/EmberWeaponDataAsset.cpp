// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberWeaponDataAsset.h"

#include "EmberWeaponBase.h"
#include "WeaponData.h"
#include "GameFramework/Character.h"
#include "Utility/CLog.h"

UWeaponDataAsset::UWeaponDataAsset()
{
	WeaponClass = AEmberWeaponBase::StaticClass();
}

void UWeaponDataAsset::Beginplay(ACharacter* InOwner, UWeaponData** OutWeaponData)
{
	AEmberWeaponBase* weapon{};

	if (WeaponClass == nullptr)
	{
		DebugLogE("WeaponClass is null");
		return;
	}

	FActorSpawnParameters parames;
	parames.Owner = InOwner;
	weapon = InOwner->GetWorld()->SpawnActor<AEmberWeaponBase>(WeaponClass, parames);

	*OutWeaponData = NewObject<UWeaponData>();
	(*OutWeaponData)->WeaponBase = weapon;
}
