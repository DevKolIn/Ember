// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeaponData.generated.h"

class AEmberWeaponBase;
class UWeaponDataAsset;
/**
 * 
 */
UCLASS()
class EMBER_API UWeaponData : public UObject
{
	GENERATED_BODY()
private:
	friend UWeaponDataAsset;

public:
	FORCEINLINE TObjectPtr<AEmberWeaponBase> GetWeapon() { return WeaponBase; }

private:
	TObjectPtr<AEmberWeaponBase> WeaponBase;
};
