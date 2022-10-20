// CourseShootThemUp

using UnrealBuildTool;
using System.Collections.Generic;

public class CourseShootThemUpTarget : TargetRules
{
	public CourseShootThemUpTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "CourseShootThemUp" } );
	}
}
