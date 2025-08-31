#include "Core/RaidGameMode.h"
#include "Core/RaidGameState.h"
#include "Character/EmberCharacter.h"
#include "Character/EmberPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/CLog.h"
#include "EngineUtils.h"


ARaidGameMode::ARaidGameMode()
{
	IsOnFX = false;
	CurrentWeather = EWeatherType::Clear;
	WeatherTerm = 5.0f;
}

void ARaidGameMode::BeginPlay()
{
	Super::BeginPlay();
	RaidGameState = GetGameState<ARaidGameState>();
	GetWorld()->GetTimerManager().SetTimer(GameTimer, this, &ARaidGameMode::UpdateWeather, WeatherTerm, true);
}

void ARaidGameMode::UpdateWeather()
{
	switch (RaidGameState->CurrentWeather)
	{
	case EWeatherType::Clear:
		RaidGameState->CurrentWeather = EWeatherType::Snow;
		UE_LOG(LogTemp, Warning, TEXT("Weather: %s"), *UEnum::GetValueAsString(RaidGameState->CurrentWeather));
		RaidGameState->OnReplicatedUse();
		break;
	case EWeatherType::Snow:
		RaidGameState->CurrentWeather = EWeatherType::Storm;
		UE_LOG(LogTemp, Warning, TEXT("Weather: %s"), *UEnum::GetValueAsString(RaidGameState->CurrentWeather));
		RaidGameState->OnReplicatedUse();
		break;
	case EWeatherType::Storm:
		RaidGameState->CurrentWeather = EWeatherType::Clear;
		UE_LOG(LogTemp, Warning, TEXT("Weather: %s"), *UEnum::GetValueAsString(RaidGameState->CurrentWeather));
		RaidGameState->OnReplicatedUse();
		break;
	default: break;
	}
}

void ARaidGameMode::PlayerDamage() const
{
	// 월드에서 모든 EmberCharacter 찾기
	for (TActorIterator<AEmberCharacter> It(GetWorld()); It; ++It)
	{
		AEmberCharacter* Player = *It;
		if (!Player) continue;
	}
}