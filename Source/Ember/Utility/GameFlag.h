#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EWeatherType : uint8
{
	Clear UMETA(DisplayName = "CLEAR"),
	Snow UMETA(DisplayName = "SNOW"),
	Storm UMETA(DisplayName = "STORM"),
};

enum class EWorldTimeState
{
	Morning UMETA(DisplayName = "MORNING"),
	Afternoon UMETA(DisplayName = "AFTERNOON"),
	Evening UMETA(DisplayName = "EVENING"),
	Night UMETA(DisplayName = "NIGHT"),
};

UENUM(BlueprintType)
enum class ESpeedType : uint8
{
	Walk UMETA(DisplayName = "Walk"),
	Run UMETA(DisplayName = "Run"),
	Sprint UMETA(DisplayName = "Sprint"),
	Max UMETA(DisplayName = "Max")
};