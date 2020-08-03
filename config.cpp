class CfgPatches
{
	class HeroesAndBandits
	{
		requiredVersion=0.1;
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
                value="";
                files[]={
					"HeroesAndBandits/scripts/Common",
					"HeroesAndBandits/scripts/3_Game"
					};
            };
			
			class worldScriptModule
            {
                value="";
                files[]={ 
					"HeroesAndBandits/scripts/Common",
					"HeroesAndBandits/scripts/4_World" 
					};
            };
			
	        class missionScriptModule
            {
                value="";
                files[]={ 
					"HeroesAndBandits/scripts/Common",
					"HeroesAndBandits/scripts/5_Mission" 
					};
            };
			
        };
    };
};
