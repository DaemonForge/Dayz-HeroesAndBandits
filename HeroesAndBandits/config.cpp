class CfgPatches
{
	class HeroesAndBandits
	{
		requiredVersion=0.1;
		requiredAddons[]={
			"HABDefines",
			"JM_CF_Scripts"
		};
	};
};

class CfgMods
{
	class HeroesAndBandits
	{
		dir="HeroesAndBandits";
        hideName=1;
        hidePicture=1;
        name="HeroesAndBandits";
        credits="DaemonForge";
        author="DaemonForge";
        authorID="0";
        version="1.2";
        type="mod";
		inputs = "HeroesAndBandits/data/Inputs.xml";
	    dependencies[]={ "Game", "World", "Mission" };
	    class defs
	    {
			class imageSets
			{
				files[]=
				{
					"HeroesAndBandits/gui/images/HeroesAndBandits.imageset"
				};
			};
			class gameScriptModule
            {
				value = "";
                files[]={
					"HeroesAndBandits/scripts/3_Game"
					};
            };
			
			class worldScriptModule
            {
				value = "";
                files[]={ 
					"HeroesAndBandits/scripts/4_World" 
					};
            };
			
	        class missionScriptModule
            {
				value = "";
                files[]={ 
					"HeroesAndBandits/scripts/5_Mission" 
					};
            };
			
        };
    };
};

class CfgVehicles
{
	class Flag_Base;
	class Hero_Flag: Flag_Base
	{
		scope=2;
		displayName  = "Hero Flag";
		hiddenSelectionsTextures[]=
		{
			"HeroesAndBandits\data\texture\flag_hero_co.paa"
		};
	};
	class Bandit_Flag: Flag_Base
	{
		scope=2;
		displayName  = "Bandit Flag";
		hiddenSelectionsTextures[]=
		{
			"HeroesAndBandits\data\texture\flag_bandit_co.paa"
		};
	};
};
