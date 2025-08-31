// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Utility/GameFlag.h"
#include "RaidGameState.generated.h"


UCLASS()
class EMBER_API ARaidGameState : public AGameState
{
	GENERATED_BODY()

public:
	ARaidGameState();

	UFUNCTION()
	void OnReplicatedUse() const;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	TSubclassOf<AActor> SnowFXClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX", ReplicatedUsing=OnReplicatedUse)
	EWeatherType CurrentWeather;
};