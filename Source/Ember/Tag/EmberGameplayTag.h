#pragma once

#include "GameplayTagContainer.h"

#define ABTAG_DATA_DAMAGE FGameplayTag::RequestGameplayTag(FName("Data.Damage"))
#define ABTAG_DATA_HIT FGameplayTag::RequestGameplayTag(FName("Character.State.IsHit"))
#define ABTAG_DATA_Dead FGameplayTag::RequestGameplayTag(FName("Character.State.IsDead"))
#define ABTAG_DATA_Avoid FGameplayTag::RequestGameplayTag(FName("Character.State.IsAvoid"))