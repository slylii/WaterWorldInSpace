// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WaterWorldInSpace : ModuleRules
{
	public WaterWorldInSpace(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"WaterWorldInSpace",
			"WaterWorldInSpace/Variant_Horror",
			"WaterWorldInSpace/Variant_Horror/UI",
			"WaterWorldInSpace/Variant_Shooter",
			"WaterWorldInSpace/Variant_Shooter/AI",
			"WaterWorldInSpace/Variant_Shooter/UI",
			"WaterWorldInSpace/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
