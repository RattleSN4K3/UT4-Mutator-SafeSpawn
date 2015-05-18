namespace UnrealBuildTool.Rules
{
	public class SafeSpawn : ModuleRules
	{
		public SafeSpawn(TargetInfo Target)
		{
			PrivateIncludePaths.Add("SafeSpawn/Private");

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
					"Engine",
					"UnrealTournament",
					"InputCore",
					"SlateCore",
				}
			);
		}
	}
}
