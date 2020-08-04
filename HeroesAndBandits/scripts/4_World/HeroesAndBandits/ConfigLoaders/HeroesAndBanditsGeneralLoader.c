static string HeroesAndBanditsDirectory = "$profile:HeroesAndBandits";
static string HeroesAndBanditsPlayerDB =  HeroesAndBanditsDirectory + "\\PlayerDB";
static string HeroesAndBanditsSettingsPATH = HeroesAndBanditsDirectory + "\\settings.json";


class HeroesAndBanditsSettings
{ 
	//Default Values
	string ConfigVersion = "5";
	
	int Mode = 0; // 0 is differantial / 1 is highest level / 2 is Mixed
	
	
	int NotificationMessageTime = 15;
	
	bool KillFeed = true;
	bool SucideFeed = false;
	
	ref array<int> KillFeedMessageColor = {200, 250, 0, 100};
	
	string CommandPrefix = "/";
	
	bool AllowStatCommand = true;
	bool AllowHumanityCommand = true;
	
	bool AllowGUI = true;
	string GUIHeading = "#HAB_TITLE";
	bool HideKillsInGUI = false;
	
	bool BanditCanRemoveMask = false;
	bool BanditCanRemoveArmBand = true;
	ref TStringArray BanditMasks = {"BandanaMask_RedPattern", "BandanaMask_BlackPattern","BandanaMask_CamoPattern","BandanaMask_GreenPattern", "BandanaMask_PolkaPattern", "Bandana_Blue", "Bandana_Pink", "Bandana_Yellow"};
	ref TStringArray BanditArmBads = {};
	bool HeroCanRemoveMask = true;
	bool HeroCanRemoveArmBand = true;
	ref TStringArray HeroMasks = {};
	ref TStringArray HeroArmBands = {};
	
	//This doesn't affect Humanity as its the calcuation of two Affinities
	bool AffintyCantGoBelowZero = true;
	
	//Expansion Settings 
	bool Expansion_EnableIconOnPlayerTag = true;
	int Expansion_ImageTypePlayerTag = 0; //0 Affinty / 1 Level
	bool Expansion_HideNameOnPlayerTag = false;
	
	
	
	bool ExceptionLogs = true;
	bool VerboseLogs = false;
	bool DebugLogs = false;
	bool DebugCommand = false;
		
	/*
	Bambis
	SurvivorM_Boris   Male White
	SurvivorM_Hassan  Male Black
	SurvivorF_Frida   Female White
	SurvivorM_Jose    Male Asian
	SurvivorM_Cyril   Male White
	SurvivorF_Gabi    Female White
	
	Heroes
	SurvivorM_Indar     Male White
	SurvivorM_Lewis     Male Black
	SurvivorF_Linda     Female White
	SurvivorM_Taiki     Male Asain
	SurvivorM_Mirek     Male White
	SurvivorM_Oliver    Male White
	SurvivorM_Niki      Male White
	SurvivorM_Rolf      Male White
	SurvivorF_Irena     Female White
	SurvivorF_Judy      Female Black
	SurvivorF_Keiko     Female Asain
	
	BanditSkins
	SurvivorM_Denis    Male White
	SurvivorM_Kaito    Male Black
	SurvivorF_Naomi    Female Black
	SurvivorM_Francis  Male Asain
	SurvivorM_Elias    Male Black
	SurvivorM_Manua    Male White
	SurvivorM_Peter    Male White
	SurvivorM_Quinn    Male White
	SurvivorM_Guo      Male White
	SurvivorM_Seth     Male White
	SurvivorF_Maria    Female White
	SurvivorF_Eva      Female White
	SurvivorF_Helga    Female White
	
	*/
	ref TStringArray BambiSkins = {"SurvivorM_Boris", "SurvivorM_Hassan", "SurvivorF_Frida", "SurvivorM_Jose", "SurvivorM_Cyril", "SurvivorF_Gabi"};
	
	ref TStringArray HeroSkins = {"SurvivorM_Indar","SurvivorM_Lewis","SurvivorF_Linda","SurvivorM_Taiki","SurvivorM_Mirek","SurvivorM_Oliver","SurvivorF_Irena","SurvivorF_Judy","SurvivorF_Keiko","SurvivorM_Niki","SurvivorM_Rolf"};
	
	ref TStringArray BanditSkins = {"SurvivorM_Peter","SurvivorM_Kaito","SurvivorF_Naomi","SurvivorM_Francis","SurvivorM_Elias","SurvivorM_Manua","SurvivorM_Denis","SurvivorM_Quinn","SurvivorM_Guo","SurvivorM_Seth","SurvivorF_Maria","SurvivorF_Eva", "SurvivorF_Helga"};
			
	
		// Load config file or create default file if config doesn't exsit
	void Load(){
		habCheckUpgradeToConfigV4();
		if (FileExist(HeroesAndBanditsSettingsPATH)) //If config exist load File
		{
	        JsonFileLoader<HeroesAndBanditsSettings>.JsonLoadFile(HeroesAndBanditsSettingsPATH, this);
			if (ConfigVersion == "4"){
				doV5Upgrade();
				JsonFileLoader<HeroesAndBanditsSettings>.JsonSaveFile(HeroesAndBanditsSettingsPATH, this);
			}
			
		}else{ //File does not exist create file
			MakeDirectory(HeroesAndBanditsDirectory);
			MakeDirectory(HeroesAndBanditsPlayerDB);
			habPrint("Creating Default Settings Config", "Always");	
			JsonFileLoader<HeroesAndBanditsSettings>.JsonSaveFile(HeroesAndBanditsSettingsPATH, this);
		}
	}
	
	
	//Returns the kill feed message color in an int value
	int getKillFeedMessageColor(){
		return ARGB(KillFeedMessageColor[0], KillFeedMessageColor[1], KillFeedMessageColor[2], KillFeedMessageColor[3]);
	}

	void doV5Upgrade(){
		ConfigVersion = "5";
		BanditCanRemoveMask = false;
		HeroCanRemoveMask = true;
		AffintyCantGoBelowZero = true;
		Expansion_EnableIconOnPlayerTag = true;
		Expansion_ImageTypePlayerTag = 0; //0 Affinty / 1 Level
		Expansion_HideNameOnPlayerTag = false;
		BanditMasks = {"BandanaMask_RedPattern", "BandanaMask_BlackPattern","BandanaMask_CamoPattern","BandanaMask_GreenPattern", "BandanaMask_PolkaPattern", "Bandana_Blue", "Bandana_Pink", "Bandana_Yellow"};
		BanditArmBads = {};
		HeroMasks = {};
		HeroArmBands = {};
	}
	
}
