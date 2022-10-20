// CourseShootThemUp

using UnrealBuildTool;
using System.Collections.Generic;

public class CourseShootThemUpEditorTarget : TargetRules
{
	public CourseShootThemUpEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "CourseShootThemUp" } );
	}
}
