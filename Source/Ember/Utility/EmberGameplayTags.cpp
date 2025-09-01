#pragma once

#include "EmberGameplayTags.h"

namespace EmberGameplayTags
{
	// AI Ability
	UE_DEFINE_GAMEPLAY_TAG(AI_Ability_Death,								"AI.Ability.Death");
	UE_DEFINE_GAMEPLAY_TAG(AI_Ability_HitReact,								"AI.Ability.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(AI_Ability_Attack,								"AI.Ability.Attack");
	UE_DEFINE_GAMEPLAY_TAG(AI_Ability_Attack_Normal,						"AI.Ability.Attack.Normal");
	UE_DEFINE_GAMEPLAY_TAG(AI_Ability_Attack_Heavy,							"AI.Ability.Attack.Heavy");
	UE_DEFINE_GAMEPLAY_TAG(AI_Ability_Phase,								"AI.Ability.Phase");
	UE_DEFINE_GAMEPLAY_TAG(AI_Ability_GiantForm,							"AI.Ability.GiantForm");

	// Status
	UE_DEFINE_GAMEPLAY_TAG(Status_Attack,									"Status.Attack");
	UE_DEFINE_GAMEPLAY_TAG(Status_HitReact,									"Status.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Status_Death,									"Status.Death");
	UE_DEFINE_GAMEPLAY_TAG(Status_Phase,									"Status.Phase");
	UE_DEFINE_GAMEPLAY_TAG(Status_GiantForm,								"Status.GiantForm");
	
	// Gameplay Event
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Death,								"GameplayEvent.Death");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_HitReact,							"GameplayEvent.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Montage_Begin,						"GameplayEvent.Montage.Begin");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Montage_End,						"GameplayEvent.Montage.End");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Trace,								"GameplayEvent.Trace");
}