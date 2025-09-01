#include "RaidGameState.h"
#include "Character/EmberCharacter.h"
#include "Character/EmberPlayerController.h"
#include "kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ARaidGameState::ARaidGameState()
{
	bReplicates = true;
	CurrentWeather = EWeatherType::Clear;
}

void ARaidGameState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CurrentWeather, COND_None, REPNOTIFY_Always);
}

void ARaidGameState::OnReplicatedUse() const
{
	AEmberPlayerController* Controller = Cast<AEmberPlayerController>(GetWorld()->GetFirstPlayerController());
	Controller->ChangeFX();
}
