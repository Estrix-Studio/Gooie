using UnrealBuildTool;

public class LoadingScreenModule : ModuleRules
{
	public LoadingScreenModule(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore", 
			"MoviePlayer",
			"Slate",
			"SlateCore"
		});
	}
}
