class CfgPatches
{
	class ZenExpansionAirdropFloat
	{
		requiredVersion = 0.1;
		units[] = {};
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts",
			"DayZExpansion_Missions_Scripts"
		};
	};
};

class CfgMods
{
	class ZenExpansionAirdropFloat
	{
		dir = "ZenExpansionAirdropFloat";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "ZenExpansionAirdropFloat";
		credits = "";
		author = "Zenarchist";
		authorID = "0";
		version = "1.0";
		extra = 0;
		type = "mod";
		dependencies[] = { "Game","World","Mission" };
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = { "ZenExpansionAirdropFloat/scripts/3_game" };
			};
			class worldScriptModule
			{
				value = "";
				files[] = { "ZenExpansionAirdropFloat/scripts/4_World" };
			};
			class missionScriptModule
			{
				value = "";
				files[] = { "ZenExpansionAirdropFloat/scripts/5_mission" };
			};
		};
	};
};