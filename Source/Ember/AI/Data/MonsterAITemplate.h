#pragma once

#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "MonsterAITemplate.generated.h"

class AAIController;
class UBehaviorTree;
class AMonsterAIBase;
class AMonsterAIWeapon;
class UMonsterGameplayAbility;

USTRUCT(BlueprintType)
struct FEmberAbilitySet_GameplayAbility	
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMonsterGameplayAbility> Ability = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "Ability.AI"))
	FGameplayTag GameplayTag;
};

USTRUCT(BlueprintType)
struct FMontage
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> Montage;
	UPROPERTY(EditAnywhere)
	float PlayRate;
};

UCLASS(Blueprintable, Const, Abstract)
class UMonsterAITemplate : public UObject
{
	GENERATED_BODY()

public:
	UMonsterAITemplate(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UFUNCTION()
	UAnimMontage* GetHitMontage(AActor* HitActor, const FVector& HitLocation) const;
	
protected:
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif // WITH_EDITOR

public:
	/* Base */
	UPROPERTY(EditDefaultsOnly, Category = "Base")
	TSubclassOf<AMonsterAIBase> MonsterClass;

	/* AIController */
	UPROPERTY(EditDefaultsOnly, Category = "AIController")
	TSubclassOf<AAIController> AIControllerClass;

	UPROPERTY(EditDefaultsOnly, Category = "AIController")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	/* Weapon */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AMonsterAIWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponAttachSocket;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FTransform WeaponAttachTransform;

	/* GAS */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta=(TitleProperty=Ability))
	TArray<FEmberAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	/* Animation */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> FrontHitMontage;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> BackHitMontage;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> LeftHitMontage;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> RightHitMontage;
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, FMontage> MontageDatas;
};
