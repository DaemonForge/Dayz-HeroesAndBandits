class CfgPatches
{
	class HABDefines
	{
		requiredVersion=0.1;
		requiredAddons[]={
		};
	};
};

class CfgMods
{
	class HABDefines
	{
		dir="HABDefines";
        name="HeroesAndBandits";
        version="1.2";
        type="mod";
	    dependencies[]={ "Game", "World", "Mission" };
	    class defs
	    {
			class gameScriptModule
            {
				value = "";
                files[]={
					"HABDefines/scripts/Common"
					};
            };
			
			class worldScriptModule
            {
				value = "";
                files[]={ 
					"HABDefines/scripts/Common"
					};
            };
			
	        class missionScriptModule
            {
				value = "";
                files[]={ 
					"HABDefines/scripts/Common"
					};
            };
			
        };
    };
};
