//CopAndRobber.Build.cs

using UnrealBuildTool;

public class CopAndRobber : ModuleRules
{
	public CopAndRobber(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			//Initialize
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
			//GAS
			"GameplayAbilities","GameplayTags","GameplayTasks",
			//Nav
			"NavigationSystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"AIModule", "AITestSuite", "NavigationSystem",
		});
		
		PublicIncludePaths.AddRange(new string[] {
    		"CopAndRobber"
		});

		PublicIncludePaths.AddRange(new string[] { "CopAndRobber" });
	}
}
