// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "MonsterAIData.generated.h"

class AMonsterAIBase;
class UMonsterAITemplate;

UCLASS(BlueprintType, Const)
class EMBER_API UMonsterAIData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	static const UMonsterAIData& Get();

public:
#if WITH_EDITORONLY_DATA
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
#endif // WITH_EDITORONLY_DATA

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif // WITH_EDITOR

public:
	const UMonsterAITemplate& FindMonsterAITemplateByClass(const TSubclassOf<AMonsterAIBase>& MonsterAIBaseClass) const;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TMap<int32, TSubclassOf<UMonsterAITemplate>> MonsterAITemplateIDToClass;

	UPROPERTY()
	TMap<TSubclassOf<AMonsterAIBase>, TSubclassOf<UMonsterAITemplate>> MonsterAIBaseClassToTemplateClass;
};
