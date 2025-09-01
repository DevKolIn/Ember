// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EmberWeaponBase.generated.h"

class UCapsuleComponent;

UCLASS()
class EMBER_API AEmberWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEmberWeaponBase();

	/* 공격 */
	/*UFUNCTION(BlueprintCallable, Category="Weapon")
	void Attack();*/
	UFUNCTION(BlueprintCallable)
	void AttachTo(FName InSocketName);
	/* 공격 쿨다운 (초) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon|Stats")
	float Cooldown;
	/* 룬관련 */
	UPROPERTY(VisibleAnywhere, Category="Weapon|Rune")
	TArray<TObjectPtr<ARuneItem>> EquippedRunes;
	UPROPERTY(EditAnywhere, Category="Weapon|Rune")
	int MaxRuneSlots = 3;
	UFUNCTION(BlueprintCallable, Category="Weapon|Rune")
	void ApplyRune(ARuneItem* Rune);
	
	/*  내구도 -> 필요했던가?? */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon|Stats")
	//float Durability;

	/* 애님몽타주 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon|Animation")
	UAnimMontage* EquipMontage;
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	ACharacter* OwnerCharacter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	float LastAttackTime; /* 쿨타임 체크용 */
	/*bool CanAttack() const;
	virtual void OnAttack() PURE_VIRTUAL(AEmberWeaponBase::OnAttack, );*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon|Mesh")
	TObjectPtr<USkeletalMeshComponent> MeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon|Mesh")
	TObjectPtr<UCapsuleComponent> Collision;
};
