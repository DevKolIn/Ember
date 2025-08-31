// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberAssetManager.h"

UEmberAssetManager& UEmberAssetManager::Get()
{
	if (UEmberAssetManager* Singleton = Cast<UEmberAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	return *NewObject<UEmberAssetManager>();
}

const UMonsterAIData& UEmberAssetManager::GetMonsterAIData()
{
	return GetOrLoadTypedGameData(MonsterAIData);
}

void UEmberAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
}

UPrimaryDataAsset* UEmberAssetManager::LoadGameDataOfClass(TSubclassOf<UPrimaryDataAsset> DataClass,
	const TSoftObjectPtr<UPrimaryDataAsset>& DataClassPath, FPrimaryAssetType PrimaryAssetType)
{
	UPrimaryDataAsset* Asset = nullptr;

	if (!DataClassPath.IsNull())
	{
		if (GIsEditor)
		{
			Asset = DataClassPath.LoadSynchronous();
			LoadPrimaryAssetsWithType(PrimaryAssetType);
		}
		else
		{
			TSharedPtr<FStreamableHandle> Handle = LoadPrimaryAssetsWithType(PrimaryAssetType);
			if (Handle.IsValid())
			{
				Asset = Cast<UPrimaryDataAsset>(Handle->GetLoadedAsset());
			}
		}
	}

	if (Asset)
	{
		GameDataMap.Add(DataClass, Asset);
	}
	
	return Asset;
}
