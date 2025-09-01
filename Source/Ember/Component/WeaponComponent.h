#pragma once

#include "CoreMinimal.h"
#include "Weapon/EmberWeaponDataAsset.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMBER_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	FORCEINLINE UWeaponDataAsset* GetWeaponAsset() { return WeaponAsset; }

private:
	UPROPERTY(EditAnywhere, Category="DataAsset")
	UWeaponDataAsset* WeaponAsset;
	UPROPERTY()
	UWeaponData* WeaponData;

public:	
	UWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	AEmberWeaponBase* GetWeapon();

private:
	ACharacter* OwnerCharacter;
};
