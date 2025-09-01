// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "EmberBaseCharacter.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GenericTeamAgentInterface.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Component/RuneSystemComponent.h"
#include "Template/RuneItemTemplate.h"
#include "Components/SphereComponent.h"
#include "Component/LootDropManagerComponent.h"
#include "Component/InteractionComponent.h"
#include "Component/QuickSlotComponent.h"
#include "EmberCharacter.generated.h"


class UMontageComponent;
class UGameplayEffect;
class UGameplayAbility;
class UWeaponComponent;

UCLASS()
class EMBER_API AEmberCharacter : public AEmberBaseCharacter,  public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AEmberCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* 팀 설정 */
	virtual FGenericTeamId GetGenericTeamId() const override;
	void PickupItem();

	//룬 장착용 함수
	UFUNCTION(BlueprintCallable)
	bool TryEquipRune(const URuneItemTemplate* Template, int32 PreferredSlotIndex);
	UFUNCTION(BlueprintCallable)
	bool TryEquipRuneAuto(const URuneItemTemplate* Template); // 내부에서 PreferredSlotIndex=-1 전달
	UFUNCTION(Server, Reliable)
	void Server_TryEquipRune(const URuneItemTemplate* Template, int32 PreferredSlotIndex);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnRuneEquipped(const URuneItemTemplate* Template, int32 SlotIndex);
	UQuickSlotComponent* GetQuickSlotComponent() const;
	void ShowRuneComparisonUI(const URuneItemTemplate* NewRuneTemplate);
	void EquipRune(const URuneItemTemplate* NewRuneTemplate, int32 SlotIndex);
	UFUNCTION(Server, Reliable)
	void Server_RequestInteraction(UInteractionComponent* TargetInteraction);

	UFUNCTION(Server, Reliable)
	void Server_PickupItem(APickupItemActor* TargetItem);

	UFUNCTION()
	void AddOverlappingItem(APickupItemActor* Item);
	UFUNCTION()
	void RemoveOverlappingItem(APickupItemActor* Item);
	void SetIgnoreCollision(bool bIgnore);
	// 수정: UFUNCTION 매크로 추가
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnRuneEquippedDetailed(
		const FText& RuneName, int32 SlotIndex,
		float New_AD, float dAD,
		float New_MaxAD, float dMaxAD,
		float New_Range, float dRange,
		float New_Radius, float dRadius,
		float New_Meta, float dMeta
	);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<class USpringArmComponent> SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, Category = Component)
	TObjectPtr<class UCustomMoveComponent> MoveComponent;
	UPROPERTY(VisibleAnywhere, Category = Component)
	TObjectPtr<class UCustomCameraComponent> CameraComponent;
	UPROPERTY(VisibleAnywhere, Category = Component)
	TObjectPtr<UWeaponComponent> WeaponComponent;
	UPROPERTY(VisibleAnywhere, Category = Component)
	TObjectPtr<UMontageComponent> MontageComponent;

	//GAS
	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray< TSubclassOf<UGameplayAbility >> InputAbilities;
	UPROPERTY(EditAnywhere, Category = "GAS")
	TMap<int32, TSubclassOf<class UGameplayAbility>> GameAbilities;
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayEffect> GETemperature;
	UPROPERTY(EditAnywhere, Category = "GAS|TemperaturLevel")
	float TemperatureLeve = 1.0f;
	UPROPERTY(EditAnywhere, Category = "GAS|TemperaturLevel")
	int32 MaxCount = 10.0f;
	int32 Count;

	virtual void BeginPlay() override;
	void SetupGASInputComponent();
	void GASInputPressed(int32 Input);
	void GASInputReleased(int32 Input);
	UPROPERTY()
	TArray<APickupItemActor*> OverlappingItems;
	
	APickupItemActor* GetFocusedPickupItem() const;
	// 룬 시스템 선언
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<URuneSystemComponent> RuneSystem;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	USphereComponent* PickupSphere;
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float InteractDistance = 500.0f;
	UPROPERTY(EditAnywhere, Category = "Interaction")
	bool bDrawInteractionDebug = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UQuickSlotComponent* QuickSlotComponent;
	void DamageTemperature();

	void Dead(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);
	
	UFUNCTION(Server, Reliable)
	void Server_UseQuickSlot(int32 Index);

	void UseQuickSlot1(); void UseQuickSlot2(); void UseQuickSlot3(); void UseQuickSlot4(); void UseQuickSlot5();
	void UseQuickSlot(int32 Index);
	virtual void OnRep_PlayerState() override;


private:
	TObjectPtr<class AEmberPlayerController> PlayerController;

	FTimerHandle Timer;

	UPROPERTY(EditAnywhere)
	UAnimMontage* montage;
};
