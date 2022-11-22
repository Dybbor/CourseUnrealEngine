// CourseShootThemUp

using UnrealBuildTool;

public class CourseShootThemUp : ModuleRules
{
    public CourseShootThemUp(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        PublicIncludePaths.AddRange(new string[] {
            "CourseShootThemUp/Public/Player" ,
            "CourseShootThemUp/Public/Components",
            "CourseShootThemUp/Public/DevelopmentContent",
            "CourseShootThemUp/Public/Weapon",
            "CourseShootThemUp/Public/Animations",
            "CourseShootThemUp/Public/UI"});
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
