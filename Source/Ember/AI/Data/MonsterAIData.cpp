// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIData.h"

#include "MonsterAITemplate.h"
#include "AI/Monster/MonsterAIBase.h"
#include "Misc/DataValidation.h"
#include "System/EmberAssetManager.h"
#include "UObject/ObjectSaveContext.h"

const UMonsterAIData& UMonsterAIData::Get()
{
	return UEmberAssetManager::Get().GetMonsterAIData();
}

#if WITH_EDITORONLY_DATA
void UMonsterAIData::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	MonsterAIBaseClassToTemplateClass.Empty();

	for (const auto& Pair : MonsterAITemplateIDToClass)
	{
		if (const TSubclassOf<UMonsterAITemplate> MonsterAITemplateClass = Pair.Value)
		{
			if (const UMonsterAITemplate* DefaultObject = MonsterAITemplateClass->GetDefaultObject<UMonsterAITemplate>())
			{
				MonsterAIBaseClassToTemplateClass.Emplace(DefaultObject->MonsterClass, MonsterAITemplateClass);
			}
		}
	}
}
#endif

#if WITH_EDITOR
EDataValidationResult UMonsterAIData::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	TSet<int32> MonsterAITemplateIDSet;
	TSet<TSubclassOf<UMonsterAITemplate>> MonsterAITemplateClassSet;

	for (const auto& Pair : MonsterAITemplateIDToClass)
	{
		// ID Check
		const int32 MonsterAITemplateID = Pair.Key;

		if (MonsterAITemplateID <= 0)
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid ID : [ID : %d]\n"), MonsterAITemplateID)));
			Result = EDataValidationResult::Invalid;
		}

		if (MonsterAITemplateIDSet.Contains(MonsterAITemplateID))
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Duplicated ID : [ID : %d]\n"), MonsterAITemplateID)));
			Result = EDataValidationResult::Invalid;
		}

		MonsterAITemplateIDSet.Add(MonsterAITemplateID);

		const TSubclassOf<UMonsterAITemplate> MonsterAITemplateClass = Pair.Value;
		
		if (MonsterAITemplateClass == nullptr)
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid Class : [ID : %d]\n"), MonsterAITemplateID)));
			Result = EDataValidationResult::Invalid;
		}

		if (MonsterAITemplateClassSet.Contains(MonsterAITemplateClass))
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Duplicated Class : [ID : %d]\n"), MonsterAITemplateID)));
			Result = EDataValidationResult::Invalid;
		}
		
		MonsterAITemplateClassSet.Add(MonsterAITemplateClass);
	}
	
	return Result;
}
#endif

const UMonsterAITemplate& UMonsterAIData::FindMonsterAITemplateByClass(const TSubclassOf<AMonsterAIBase>& MonsterAIBaseClass) const
{
	const TSubclassOf<UMonsterAITemplate>* MonsterAITemplateClass = MonsterAIBaseClassToTemplateClass.Find(MonsterAIBaseClass);
	ensureAlwaysMsgf(MonsterAITemplateClass, TEXT("Can't find MonsterAITemplateClass"));
	return *(MonsterAITemplateClass->GetDefaultObject());
}
