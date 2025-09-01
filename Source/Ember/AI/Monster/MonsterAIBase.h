#pragma once
#include "EmberBaseCharacter.h"
#include "MonsterAIBase.generated.h"

class AMonsterAIWeapon;
class UEmberAS_Player;
class UHealthComponent;
class UMonsterGameplayEffectComponent;

UCLASS(Abstract, Blueprintable)
class EMBER_API AMonsterAIBase : public AEmberBaseCharacter
{
	GENERATED_BODY()

public:
	AMonsterAIBase();

public:
	AMonsterAIWeapon* GetWeapon() { return WeaponActor; }

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	virtual void HandleHealthChanged(UHealthComponent* InHealthComponent, float OldValue, float NewValue, AActor* DamageInstigator);

private:
	//~ AEmberBaseCharacter Overrides
	virtual void OnDeath() override;
	//~ End of AEmberBaseCharacter Overrides
	
private:
	void InitializeMonsterAI();
	void DisableMovementAndCollision();

protected:
	UPROPERTY()
	TObjectPtr<UEmberAS_Player> AttributeSet;

private:
	UPROPERTY()
	TObjectPtr<AMonsterAIWeapon> WeaponActor;
};
