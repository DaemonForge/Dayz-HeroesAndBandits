class CfgPatches
{
	class HeroesAndBandits
	{
		requiredVersion=0.1;
		requiredAddons[]={
			"DZ_Data"
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
                files[]={
					"HeroesAndBandits/scripts/Common",
					"HeroesAndBandits/scripts/3_Game"
					};
            };
			
			class worldScriptModule
            {
                files[]={ 
					"HeroesAndBandits/scripts/Common",
					"HeroesAndBandits/scripts/4_World" 
					};
            };
			
	        class missionScriptModule
            {
                files[]={ 
					"HeroesAndBandits/scripts/Common",
					"HeroesAndBandits/scripts/5_Mission" 
					};
            };
			
        };
    };
};
