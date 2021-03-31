class HeroesAndBanditsSimpleConfig
{ 
	
	string ConfigVersion = "7";
	int UseSimple = 1; //1 use simple 2 convert and save simple to advanced 0 use advanced (2 will set this to 0)
	
	//Default Values
	bool KillFeed = true;
	bool SucideFeed = false;
		
	bool BanditCanRemoveMask = false;
	bool BanditCanRemoveArmBand = true;
	ref TStringArray BanditMasks = {"BandanaMask_RedPattern", "BandanaMask_BlackPattern","BandanaMask_CamoPattern","BandanaMask_GreenPattern", "BandanaMask_PolkaPattern", "Bandana_Blue", "Bandana_Pink", "Bandana_Yellow"};
	ref TStringArray BanditArmBands = {};
	bool HeroCanRemoveMask = true;
	bool HeroCanRemoveArmBand = true;
	ref TStringArray HeroMasks = {};
	ref TStringArray HeroArmBands = {};
	
	
	ref TStringArray HeroOnlyItems = {};
	ref TStringArray BanditOnlyItems = {};
	
	
	//Expansion Settings 
	bool Expansion_EnableIconOnPlayerTag = true;
	
	ref array< ref HABSimpleLevel > Levels = new array< ref HABSimpleLevel >;
	ref array< ref HABSimpleAction > Actions = new array< ref HABSimpleAction >;
	ref array< ref HABSimpleZone > Zones = new array< ref HABSimpleZone >;
	
	// Load config file or create default file if config doesn't exsit
	int Load(){
		if (GetGame().IsServer()){
			MakeDirectory(habConstant.Directory);
			MakeDirectory(habConstant.PlayerDB);
			MakeDirectory(habConstant.ZoneDB);
			if (FileExist(habConstant.ConfigPATH)) //If config exist load File
			{
				JsonFileLoader<HeroesAndBanditsSimpleConfig>.JsonLoadFile(habConstant.ConfigPATH, this);
				if(ConfigVersion == "5"){
					DoV6Upgrade();
				}
				if(ConfigVersion == "6"){
					DoV7Upgrade();
				}
			}else{ //File does not exist create file
				SetDefaults();
				if (FileExist(habConstant.SettingsPATH) || FileExist(habConstant.ActionsPATH) || FileExist(habConstant.LevelsPATH) || FileExist(habConstant.ZonesPATH)){
					UseSimple = 0;
				}
				Save();
			}
			if (UseSimple != 0){
				habPrint("Loading Simple Config", "Always");
				ConvertToFull();
			}
			return UseSimple;
		}
		return 0; //If client return 0
	}
	
	void Save(){	
		JsonFileLoader<HeroesAndBanditsSimpleConfig>.JsonSaveFile(habConstant.ConfigPATH, this);
	}
	
	void ConvertToFull(){
		m_HeroesAndBanditsSettings = habConverter.ConvertSettings(this);
		m_HeroesAndBanditsConfigActions = habConverter.ConvertActions(this);
		m_HeroesAndBanditsConfigLevels = habConverter.ConvertLevels(this);
		m_HeroesAndBanditsConfigZones = habConverter.ConvertZones(this);
	}
	
	void SetDefaults(){
		Levels.Insert(new HABSimpleLevel( "Hero Lv5", 50001));
		Levels.Insert(new HABSimpleLevel( "Hero Lv4", 20001));
		Levels.Insert(new HABSimpleLevel( "Hero Lv3", 12001));
		Levels.Insert(new HABSimpleLevel( "Hero Lv2", 4001));
		Levels.Insert(new HABSimpleLevel( "Hero Lv1", 1001));
		Levels.Insert(new HABSimpleLevel( "Bambi", 0));
		Levels.Insert(new HABSimpleLevel( "Bandit Lv1", -1001));
		Levels.Insert(new HABSimpleLevel( "Bandit Lv2", -4001));
		Levels.Insert(new HABSimpleLevel( "Bandit Lv3", -12001));
		Levels.Insert(new HABSimpleLevel( "Bandit Lv4", -20001));
		Levels.Insert(new HABSimpleLevel( "Bandit Lv5", -50001));
		
		Actions.Insert(new HABSimpleAction( "ZombieKill", 5));
		Actions.Insert(new HABSimpleAction( "heroSucide", -100));
		Actions.Insert(new HABSimpleAction( "banditSucide", 100));
		Actions.Insert(new HABSimpleAction( "heroVshero", -150));
		Actions.Insert(new HABSimpleAction( "heroVsbambi", -300));
		Actions.Insert(new HABSimpleAction( "heroVsbandit", 250));
		Actions.Insert(new HABSimpleAction( "banditVshero", -250));
		Actions.Insert(new HABSimpleAction( "banditVsbambi", -125));
		Actions.Insert(new HABSimpleAction( "banditVsbandit", -150));
		Actions.Insert(new HABSimpleAction( "bambiVshero", -250));
		Actions.Insert(new HABSimpleAction( "bambiVsbambi", -100));
		Actions.Insert(new HABSimpleAction( "bambiVsbandit",300));
		Actions.Insert(new HABSimpleAction( "CombinationLockRaid", -150));
		Actions.Insert(new HABSimpleAction( "FencePartRaid", -50));
		Actions.Insert(new HABSimpleAction( "WatchtowerPartRaid", -30));
		#ifdef EXPANSIONMOD
			Actions.Insert(new HABSimpleAction( "ExpansionWall6x3Raid", -150));
			Actions.Insert(new HABSimpleAction( "ExpansionRamp6x1_5x6Raid", -150));
			Actions.Insert(new HABSimpleAction( "ExpansionRamp3x1_5x6Raid", -150));
			Actions.Insert(new HABSimpleAction( "ExpansionStairS1_5x3x3Raid", -150));
			Actions.Insert(new HABSimpleAction( "ExpansionFloor_6x6Raid", -150));
			Actions.Insert(new HABSimpleAction( "ExpansionFloor_3x3Raid", -150));
			Actions.Insert(new HABSimpleAction( "ExpansionFloor_3x6Raid", -150));
			Actions.Insert(new HABSimpleAction( "ExpansionSafeMiniRaid", -100));
			Actions.Insert(new HABSimpleAction( "ExpansionSafeMediumRaid", -150));
			Actions.Insert(new HABSimpleAction( "ExpansionSafeLargeRaid", -200));
		#endif
		
		#ifdef EXPANSIONCODELOCKEXPANDED
			Actions.Insert(new HABSimpleAction( "ExpansionCodeLockTentRaid", -100));
			Actions.Insert(new HABSimpleAction( "HackExpansionCodeLockTentRaid", -100));
			Actions.Insert(new HABSimpleAction( "HackExpansionCodeLockDoorRaid", -200));
		#endif
		
		#ifdef EXPANSIONCODELOCKEXPANDED
			Actions.Insert(new HABSimpleAction( "ExpansionCodeLockTentRaid", -150));
			Actions.Insert(new HABSimpleAction( "HacktentRaid", -100));
			Actions.Insert(new HABSimpleAction( "HackexpansionsafeRaid", -200));
			Actions.Insert(new HABSimpleAction( "HackfenceRaid", "bandit", -200));
			Actions.Insert(new HABSimpleAction( "HackexpansionwallRaid", -200));
			Actions.Insert(new HABSimpleAction( "HackexpansionfloorRaid", -200));
			Actions.Insert(new HABSimpleAction( "Hackbbp_t1Raid", -200));
			Actions.Insert(new HABSimpleAction( "Hackbbp_t2Raid", -250));
			Actions.Insert(new HABSimpleAction( "Hackbbp_t3Raid", -300));
		#endif
		
		#ifdef HACKINGMOD
			Actions.Insert(new HABSimpleAction( "HacktentRaid", -100));
			Actions.Insert(new HABSimpleAction( "HackfenceRaid", -200));
			Actions.Insert(new HABSimpleAction( "Hackbbp_t1Raid", -200));
			Actions.Insert(new HABSimpleAction( "Hackbbp_t2Raid", -250));
			Actions.Insert(new HABSimpleAction( "Hackbbp_t3Raid", -300));
		#endif
		
		Actions.Insert(new HABSimpleAction( "MedicBandagePlayer", 50));
		Actions.Insert(new HABSimpleAction( "MedicGiveBlood", 25));
		Actions.Insert(new HABSimpleAction( "MedicGiveSaline", 25));
		Actions.Insert(new HABSimpleAction( "MedicGiveCPR", 75));
		Actions.Insert(new HABSimpleAction( "MedicFeedTetracycline", 15));
		Actions.Insert(new HABSimpleAction( "MedicFeedPainkiller", 15));
		Actions.Insert(new HABSimpleAction( "MedicFeedCharcoal", 15));
		Actions.Insert(new HABSimpleAction( "MedicFeedVitamin", 10));
		Actions.Insert(new HABSimpleAction( "MedicSplintPlayer", 100));
		Zones.Insert(new HABSimpleZone( "Default Zone", 11250, 4300, 60));
		Zones.Get(0).Guards.Insert(new HABSimpleGuard( 11250, 290.2, 4300));
	}
	
	void DoV6Upgrade(){
		ConfigVersion = "6";
		Actions.Insert(new HABSimpleAction( "MedicSplintPlayer", 100));
		
		Save();
	}
	void DoV7Upgrade(){
		ConfigVersion = "7";
		Save();
	}
};
