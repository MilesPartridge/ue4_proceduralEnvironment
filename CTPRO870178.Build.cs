// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CTPRO870178 : ModuleRules
{
	public CTPRO870178(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "PhysXVehicles", "HeadMountedDisplay", "SignalProcessing" });

		PublicDefinitions.Add("HMD_MODULE_INCLUDED=1");
	}
}
