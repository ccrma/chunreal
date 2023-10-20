using UnrealBuildTool;

public class Chunreal : ModuleRules
{
    public Chunreal(ReadOnlyTargetRules Target) : base(Target)
    {
        //Allow C++ try catch
        bEnableExceptions = true;

        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDefinitions.Add("__DISABLE_MIDI__");
        PublicDefinitions.Add("__DISABLE_WATCHDOG__");
        PublicDefinitions.Add("__DISABLE_KBHIT__");
        PublicDefinitions.Add("__DISABLE_PROMPTER__");
        PublicDefinitions.Add("__DISABLE_OTF_SERVER__");
        PublicDefinitions.Add("__DISABLE_ALTER_HID__");
        PublicDefinitions.Add("__DISABLE_HID__");
        PublicDefinitions.Add("__DISABLE_SERIAL__");
        PublicDefinitions.Add("__DISABLE_FILEIO__");
        PublicDefinitions.Add("__DISABLE_THREADS__");
        PublicDefinitions.Add("__DISABLE_NETWORK__");
        PublicDefinitions.Add("__DISABLE_SHELL__");
        PublicDefinitions.Add("__DISABLE_WORDEXP__");
        PublicDefinitions.Add("__ALTER_HID__");
        PublicDefinitions.Add("YY_NO_UNISTD_H");
        //PublicDefinitions.Add("__DISABLE_SNDBUF__");
        PublicDefinitions.Add("__DISABLE_REGEX__");
	PublicDefinitions.Add("__USE_CHUCK_YACC__");

        PublicDefinitions.Add("__CHUNREAL_ENGINE__");

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "MetasoundFrontend",
                "MetasoundGraphCore",
            }
            );
            
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
            }
            );
    }
}
