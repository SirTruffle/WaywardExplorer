// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WaywardExplorer : ModuleRules
{
	public WaywardExplorer(ReadOnlyTargetRules Target) : base(Target)
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
			"UMG"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"WaywardExplorer",
			"WaywardExplorer/Variant_Platforming",
			"WaywardExplorer/Variant_Combat",
			"WaywardExplorer/Variant_Combat/AI",
			"WaywardExplorer/Variant_SideScrolling",
			"WaywardExplorer/Variant_SideScrolling/Gameplay",
			"WaywardExplorer/Variant_SideScrolling/AI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
