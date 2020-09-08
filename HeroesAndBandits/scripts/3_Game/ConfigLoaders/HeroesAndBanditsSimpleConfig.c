class HeroesAndBanditsSimpleConfig
{ 
	
	string ConfigVersion = "5";
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
	
	ref array< ref HABSimpleLevel > Levels = new ref array< ref HABSimpleLevel >;
	ref array< ref HABSimpleAction > Actions = new ref array< ref HABSimpleAction >;
	ref array< ref HABSimpleZone > Zones = new ref array< ref HABSimpleZone >;
	
	// Load config file or create default file if config doesn't exsit
	int Load(){
		if (GetGame().IsServer()){
			MakeDirectory(habConstant.Directory);
			MakeDirectory(habConstant.PlayerDB);
			if (FileExist(habConstant.ConfigPATH)) //If config exist load File
			{
				JsonFileLoader<HeroesAndBanditsSimpleConfig>.JsonLoadFile(habConstant.ConfigPATH, this);
			}else{ //File does not exist create file
				SetDefaults();
				if (FileExist(habConstant.SettingsPATH) || FileExist(habConstant.ActionsPATH) || FileExist(habConstant.LevelsPATH) || FileExist(habConstant.ZonesPATH)){
					UseSimple = 0;
				}
				Save();
			}
			if (UseSimple != 0){
				Print("Loading Simple Config");
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
		Levels.Insert(new ref HABSimpleLevel( "Hero Lv5", 50001));
		Levels.Insert(new ref HABSimpleLevel( "Hero Lv4", 20001));
		Levels.Insert(new ref HABSimpleLevel( "Hero Lv3", 12001));
		Levels.Insert(new ref HABSimpleLevel( "Hero Lv2", 4001));
		Levels.Insert(new ref HABSimpleLevel( "Hero Lv1", 1001));
		Levels.Insert(new ref HABSimpleLevel( "Bambi", 0));
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
			Actions.Insert(new ref HABSimpleAction( "ExpansionCodeLockTentRaid", -100));
			Actions.Insert(new ref HABSimpleAction( "HackExpansionCodeLockTentRaid", -100));
			Actions.Insert(new ref HABSimpleAction( "HackExpansionCodeLockDoorRaid", -200));
		#endif
		
		Actions.Insert(new ref HABSimpleAction( "MedicBandagePlayer", 50));
		Actions.Insert(new ref HABSimpleAction( "MedicGiveBlood", 25));
		Actions.Insert(new ref HABSimpleAction( "MedicGiveSaline", 25));
		Actions.Insert(new ref HABSimpleAction( "MedicGiveCPR", 75));
		Actions.Insert(new ref HABSimpleAction( "MedicFeedTetracycline", 15));
		Actions.Insert(new ref HABSimpleAction( "MedicFeedPainkiller", 15));
		Actions.Insert(new ref HABSimpleAction( "MedicFeedCharcoal", 15));
		Actions.Insert(new ref HABSimpleAction( "MedicFeedVitamin", 10));
		Zones.Insert(new ref HABSimpleZone( "Default Zone", 11250, 4300, 60));
		Zones.Get(0).Guards.Insert(new ref HABSimpleGuard( 11250, 290.2, 4300));
	}
	
};
