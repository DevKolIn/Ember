#include "EmberGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemUtils.h"
#include "Kismet/GameplayStatics.h"

UEmberGameInstance::UEmberGameInstance()
{
}

void UEmberGameInstance::Init()
{
	if (UWorld* World = GetWorld())
	{
		if (IOnlineSubsystem* SubSystem = Online::GetSubsystem(World))
		{
			SessionInterface = SubSystem->GetSessionInterface();
			if (SessionInterface.IsValid())
			{
				SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UEmberGameInstance::OnCreateSessionComplete);
			}
		}
	}
}

void UEmberGameInstance::OnCreateSessionComplete(FName ServerName, bool Succeeded)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete, Succeeded: %d"), Succeeded);
	if (Succeeded)
	{
		//play level reference
		GetWorld()->ServerTravel("/Game/Maps/TestLevel?listen");
	}
}

void UEmberGameInstance::OnFindSessionComplete(bool Succeeded)
{
	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete, Succeeded: %d"), Succeeded);
	
	if (Succeeded)
	{
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;

		for (FOnlineSessionSearchResult Result : SearchResults)
		{
			if (!Result.IsValid()) continue;

			FServerInfo Info;
			FString ServerName = "Empty Server Name";
			FString HostName = "Empty Host Name";

			Result.Session.SessionSettings.Get(FName("SERVER_NAME_KEY"), ServerName);
			Result.Session.SessionSettings.Get(FName("SERVER_HOSTNAME_KEY"), HostName);

			Info.ServerName = ServerName;
			Info.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
			Info.CurrentPlayers = Info.MaxPlayers - Result.Session.NumOpenPublicConnections;
			Info.SetPlayerCount();

			ServerListDel.Broadcast(Info);
		}

		UE_LOG(LogTemp, Warning, TEXT("SearchResults, Server Count: %d"), SearchResults.Num());

		if (SearchResults.Num())
		{
			// UE_LOG(LogTemp, Warning, TEXT("Joining Server"));
			// SessionInterface->JoinSession(0, "My Session", SearchResults[0]);
		}
	}
}

void UEmberGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete, SessionName: %s"), *SessionName.ToString());
	if (APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString JoinAddress = "";
		SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
		if (JoinAddress != "")
		{
			PlayerController->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
		}
	}
}

void UEmberGameInstance::CreateServer(FString ServerName, FString HostName)
{
	UE_LOG(LogTemp, Warning, TEXT("CreateServer"));
	
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	if (Subsystem && Subsystem->GetSubsystemName() != NAME_None)
	{
		SessionSettings.bIsLANMatch = false;
	}
	else
	{
		SessionSettings.bIsLANMatch = true; //IS LAN
	}
	
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.NumPublicConnections = 5;
	SessionSettings.BuildUniqueId = 0x12345678;

	SessionSettings.Set(FName("SERVER_NAME_KEY"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings.Set(FName("SERVER_HOSTNAME_KEY"), HostName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
	SessionInterface->CreateSession(0, FName("My Session"), SessionSettings);
}

void UEmberGameInstance::FindServer()
{
	UE_LOG(LogTemp, Warning, TEXT("FindServers"));

	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	if (Subsystem && Subsystem->GetSubsystemName() != NAME_None)
	{
		SessionSearch->bIsLanQuery = false;
	}
	else
	{
		SessionSearch->bIsLanQuery = true; //IS LAN
	}

	SessionSearch->MaxSearchResults = 10;
	SessionSearch->QuerySettings.Set(FName(TEXT("PRESENCE")), true, EOnlineComparisonOp::Equals);
	SessionSearch->QuerySettings.Set(FName(TEXT("SEARCH_BUILD_UNIQUE_ID")), 0x12345678, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}
