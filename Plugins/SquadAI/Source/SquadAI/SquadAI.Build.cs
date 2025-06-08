using UnrealBuildTool;
using System.IO;

public class SquadAI : ModuleRules
{
    public SquadAI(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core", "CoreUObject", "Engine",
            "AIModule",
            "GameplayTasks"
        });
    }
}
