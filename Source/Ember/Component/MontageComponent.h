// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "MontageComponent.generated.h"

USTRUCT()
struct FMontageData:public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FGameplayTag StateTag;
	UPROPERTY(EditAnywhere)
	UAnimMontage* Montage{};
	UPROPERTY(EditAnywhere)
	float PlayRate = 1.0f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMBER_API UMontageComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	UMontageComponent();

protected:
	virtual void BeginPlay() override;

public:
	FMontageData* GetHitData();
	FMontageData* GetDeadData();
	FMontageData* GetAvoidData(FGameplayTag InTag);

public:
	UPROPERTY(EditAnywhere, Category="Data Table")
	UDataTable* DataTable;

private:
	TObjectPtr<ACharacter> OwnerCharacter;
	TArray<FMontageData*> MontageData;
};
