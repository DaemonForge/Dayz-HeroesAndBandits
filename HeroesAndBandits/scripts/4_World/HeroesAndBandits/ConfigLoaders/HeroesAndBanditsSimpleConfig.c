 
static string HeroesAndBanditsSimpleConfigPATH = HeroesAndBanditsDirectory + "\\simple.json";

class HeroesAndBanditsSimpleConfig
{ 
	string ConfigVersion = "5";
	bool UseSimple = true;
	
	//Default Values
	bool KillFeed = true;
	bool SucideFeed = false;
		
	bool BanditCanRemoveMask = false;
	ref TStringArray BanditOnlyMasks = {"BandanaMask_RedPattern", "BandanaMask_BlackPattern","BandanaMask_CamoPattern","BandanaMask_GreenPattern", "BandanaMask_PolkaPattern", "Bandana_Blue", "Bandana_Pink", "Bandana_Yellow"};
	bool HeroCanRemoveMask = true;
	ref TStringArray HeroOnlyMasks = {};
	
	//Expansion Settings 
	bool Expansion_EnableIconOnPlayerTag = true;
	
	ref array< ref HABSimpleLevel > Levels = new ref array< ref HABSimpleLevel >;
	ref array< ref HABSimpleAction > Actions = new ref array< ref HABSimpleAction >;
	ref array< ref HABSimpleZones > Zones = new ref array< ref HABSimpleZones >;
	
	void HeroesAndBanditsSimpleConfig()
	{

	}
	
	// Load config file or create default file if config doesn't exsit
	bool Load(){
		if(GetGame().IsServer()){
			if (FileExist(HeroesAndBanditsSimpleConfigPATH)) //If config exist load File
			{
				JsonFileLoader<HeroesAndBanditsSimpleConfig>.JsonLoadFile(HeroesAndBanditsSimpleConfigPATH, this);
			}else{ //File does not exist create file
				SetDefaults();
				if(FileExist(HeroesAndBanditsSettingsPATH) || FileExist(HeroesAndBanditsActionsPATH) || FileExist(HeroesAndBanditsLevelsPATH) || FileExist(HeroesAndBanditsZonesPATH)){
					UseSimple = false;
				}
				SetDefaults();
				JsonFileLoader<HeroesAndBanditsSimpleConfig>.JsonSaveFile(HeroesAndBanditsSimpleConfigPATH, this);
			}
			if(UseSimple){
				ConvertToFull();
			}
			return UseSimple;
		}
		return false; //If client return false
	}
	
	void ConvertToFull(){
		
	}
	
	void SetDefaults(){
		Levels.Insert(new ref HABSimpleLevel( "Hero Lv5", 50001));
		Levels.Insert(new ref HABSimpleLevel( "Hero Lv4", 20001));
		Levels.Insert(new ref HABSimpleLevel( "Hero Lv3", 12001));
		Levels.Insert(new ref HABSimpleLevel( "Hero Lv2", 4001));
		Levels.Insert(new ref HABSimpleLevel( "Hero Lv1", 1001));
		Levels.Insert(new ref HABSimpleLevel( "Bandit Lv1", -1001));
		Levels.Insert(new ref HABSimpleLevel( "Bandit Lv2", -4001));
		Levels.Insert(new ref HABSimpleLevel( "Bandit Lv3", -12001));
		Levels.Insert(new ref HABSimpleLevel( "Bandit Lv4", -20001));
		Levels.Insert(new ref HABSimpleLevel( "Bandit Lv5", -50001));
		
		Actions.Insert(new ref HABSimpleAction( "ZombieKill", 5));
		Actions.Insert(new ref HABSimpleAction( "heroSucide", -100));
		Actions.Insert(new ref HABSimpleAction( "banditSucide", 100));
		Actions.Insert(new ref HABSimpleAction( "heroVshero", -150));
		Actions.Insert(new ref HABSimpleAction( "heroVsbambi", -300));
		Actions.Insert(new ref HABSimpleAction( "heroVsbandit", 250));
		Actions.Insert(new ref HABSimpleAction( "banditVshero", -250));
		Actions.Insert(new ref HABSimpleAction( "banditVsbambi", -125));
		Actions.Insert(new ref HABSimpleAction( "banditVsbandit", -150));
		Actions.Insert(new ref HABSimpleAction( "bambiVshero", -250));
		Actions.Insert(new ref HABSimpleAction( "bambiVsbambi", -100));
		Actions.Insert(new ref HABSimpleAction( "bambiVsbandit",300));
		Actions.Insert(new ref HABSimpleAction( "CombinationLockRaid", -150));
		Actions.Insert(new ref HABSimpleAction( "FencePartRaid", -50));
		Actions.Insert(new ref HABSimpleAction( "WatchtowerPartRaid", -30));
		#ifdef EXPANSIONMOD
			Actions.Insert(new ref HABSimpleAction( "ExpansionWall6x3Raid", -150));
			Actions.Insert(new ref HABSimpleAction( "ExpansionRamp6x1_5x6Raid", -150));
			Actions.Insert(new ref HABSimpleAction( "ExpansionRamp3x1_5x6Raid", -150));
			Actions.Insert(new ref HABSimpleAction( "ExpansionStairS1_5x3x3Raid", -150));
			Actions.Insert(new ref HABSimpleAction( "ExpansionFloor_6x6Raid", -150));
			Actions.Insert(new ref HABSimpleAction( "ExpansionFloor_3x3Raid", -150));
			Actions.Insert(new ref HABSimpleAction( "ExpansionFloor_3x6Raid", -150));
			Actions.Insert(new ref HABSimpleAction( "ExpansionSafeMiniRaid", -100));
			Actions.Insert(new ref HABSimpleAction( "ExpansionSafeMediumRaid", -150));
			Actions.Insert(new ref HABSimpleAction( "ExpansionSafeLargeRaid", -200));
		#endif
		
		#ifdef EXPANSIONCODELOCKEXPANDED
			Actions.Insert(new ref HABSimpleAction( "ExpansionCodeLockTentRaid", -150));
		#endif
		
		Actions.Insert(new ref HABSimpleAction( "MedicBandagePlayer", 50));
		Actions.Insert(new ref HABSimpleAction( "MedicGiveBlood", 25));
		Actions.Insert(new ref HABSimpleAction( "MedicGiveSaline", 25));
		Actions.Insert(new ref HABSimpleAction( "MedicGiveCPR", 75));
		Actions.Insert(new ref HABSimpleAction( "MedicFeedTetracycline", 15));
		Actions.Insert(new ref HABSimpleAction( "MedicFeedPainkiller", 15));
		Actions.Insert(new ref HABSimpleAction( "MedicFeedCharcoal", 15));
		Actions.Insert(new ref HABSimpleAction( "MedicFeedVitamin", 10));
		Zones.Insert(new ref HABSimpleZones( "Default Zone", 11250, 4300, 60));
	}
	
}

class HABSimpleLevel {
	string Name;
	float Humanity;
	void HABSimpleLevel(string name, float humanity){
		Name = name;
		Humanity = humanity;
	}
}

class HABSimpleAction{
	string Name;
	float Humanity;
	void HABSimpleAction(string name, float humanity){
		Name = name;
		Humanity = humanity;
	}
}

class HABSimpleZones{
    string Name;
	float X;
	float Z;
	float Radius;
	float MinHumanity = -1000;
	float MaxHumanity = 1000;
	string WelcomeMessage = "Welcome to the Default Zone";
	string WarningMessage = "!!Warning!! you are about to enter Default Zone if you continue you will be shot!";
	bool OverrideSafeZone = true;
	bool GodModPlayers = true;
	bool PreventWeaponRaise = true;
	bool PreventActions = true;
	
	void HABSimpleZones(string name, float x, float y, float radius){
		Name = name;
		X = x;
		Z = y;
		Radius = radius;
	}
}