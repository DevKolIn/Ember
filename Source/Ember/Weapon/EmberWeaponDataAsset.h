// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EmberWeaponDataAsset.generated.h"

class UWeaponData;
/**
 * 
 */
UCLASS()
class EMBER_API UWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	FORCEINLINE UAnimMontage* GetAnimMontage() { return Montage; }
	FORCEINLINE FString GetMontageSectionNamePrefix() {return MontageSectionNamePrefix;}
	FORCEINLINE uint8 GetMaxCount() { return MaxComboCount; }
	FORCEINLINE FString GetTaskInstanceName() { return TaskInstanceName; }
	FORCEINLINE float GetFrameRate() { return FrameRate; }
	FORCEINLINE float GetEffectiveFrameCount(int32 InIndex) { return EffectiveFrameCount[InIndex]; }
	FORCEINLINE bool GetCanMove() { return bCanMove; }
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEmberWeaponBase> WeaponClass;
	UPROPERTY(EditAnywhere, Category = "Weapon|Animation")
	UAnimMontage* Montage;
	UPROPERTY(EditAnywhere, Category = "Weapon|Animation")
	FString TaskInstanceName;
	UPROPERTY(EditAnywhere, Category = "Weapon|Animation")
	FString MontageSectionNamePrefix;
	UPROPERTY(EditAnywhere, Category = "Weapon|Animation")
	uint8 MaxComboCount;
	UPROPERTY(EditAnywhere, Category = "Weapon|Animation")
	float FrameRate;
	UPROPERTY(EditAnywhere, Category = "Weapon|Animation")
	TArray<float> EffectiveFrameCount;
	UPROPERTY(EditAnywhere)
	bool bCanMove{};

	///* 데미지 */
	//UPROPERTY(EditAnywhere, Category="Weapon|Stats", meta=(ClampMin="0.0"))
	//float Damage = 10.0f;

	///* 쿨다운 */
	//UPROPERTY(EditAnywhere, Category="Weapon|Stats", meta=(ClampMin="0.0"))
	//float Cooldown = 1.0f;

	///* 공격범위 */
	//UPROPERTY(EditAnywhere, Category="Weapon|Stats", meta=(ClampMin="0.0"))
	//float AttackRange = 200.0f;

	///* 투사체 스피드 */
	//UPROPERTY(EditAnywhere, Category="Weapon|Stats")
	//float ProjectileSpeed = 2000.0f;

	///* 최대 내구도 */
	//UPROPERTY(EditAnywhere, Category="Weapon|Stats", meta=(ClampMin="0.0"))
	//float Durability = 100.0f;

public:
	UWeaponDataAsset();
	void Beginplay(ACharacter* InOwner, UWeaponData** OutWeaponData);
};