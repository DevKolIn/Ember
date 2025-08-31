// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/AssetManager.h"
#include "AI/Data/MonsterAIData.h"
#include "EmberAssetManager.generated.h"

UCLASS(Config=Game)
class EMBER_API UEmberAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UEmberAssetManager& Get();
	
	const UMonsterAIData& GetMonsterAIData();
	
protected:
	//~ UAssetManager interface
	virtual void StartInitialLoading() override;
	//~ End of ~UAssetManager interface

protected:
	template <typename GameDataClass>
	const GameDataClass& GetOrLoadTypedGameData(const TSoftObjectPtr<GameDataClass>& DataPath)
	{
		if (TObjectPtr<UPrimaryDataAsset> const * pResult = GameDataMap.Find(GameDataClass::StaticClass()))
		{
			return *CastChecked<GameDataClass>(*pResult);
		}

		return *CastChecked<const GameDataClass>(LoadGameDataOfClass(GameDataClass::StaticClass(), DataPath, GameDataClass::StaticClass()->GetFName()));
	}

	UPrimaryDataAsset* LoadGameDataOfClass(TSubclassOf<UPrimaryDataAsset> DataClass, const TSoftObjectPtr<UPrimaryDataAsset>& DataClassPath, FPrimaryAssetType PrimaryAssetType);


protected:
	UPROPERTY(Config)
	TSoftObjectPtr<UMonsterAIData> MonsterAIData;
	
	UPROPERTY(Transient)
	TMap<TObjectPtr<UClass>, TObjectPtr<UPrimaryDataAsset>> GameDataMap;
};
