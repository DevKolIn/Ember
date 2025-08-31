// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Ember : ModuleRules
{
	public Ember(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.Add(ModuleDirectory);

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"AIModule",
			"GameplayAbilities",
			"GamePlayTags",
			"GamePlayTasks",
			"Niagara"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {"GameFeatures","GameplayAbilities","GameplayTasks","GameplayTags"});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
