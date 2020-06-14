static string HeroesAndBanditsDIR = "$profile:HeroesAndBandits";
static string HeroesAndBanditsPlayerDB =  HeroesAndBanditsDIR + "\\PlayerDB";
static string HeroesAndBanditsPATH = HeroesAndBanditsDIR + "\\config.json";

class HeroesAndBanditsConfig
{ 
	//Default Values
	string ConfigVersion = "1";
	ref array< ref habLevel > Levels = new ref array< ref habLevel >;
	ref habLevel DefaultLevel = new ref habLevel("Bambi", "bambi", "HeroesAndBandits/gui/images/BambiNotification.paa", "set:HeroesAndBandits image:Bambi", -1000, 1000);
	
	ref array< ref habAction > Actions = new ref array< ref habAction >;
	
	ref array< ref habZone > Zones = new ref array< ref habZone >;
	
	bool NotifyLevelChange = true;
	
    bool KillFeed = true;
    bool SucideFeed = false;
	
	int ZoneCheckTimer = 3;
	
	ref array<int> NotificationColor = {200, 0, 200, 200};
	
	int NotificationMessageTime = 15;
	
	string WarningMessageImagePath = "HeroesAndBandits/gui/images/DeathWarning.paa";
	
	ref array<int> WarningMessageColor = {200, 255, 0, 0};
	
	ref array<int> KillFeedMessageColor = {200, 250, 0, 100};
	
	bool AllowStatCommand = true;
	bool AllowHumanityCommand = true;
	
	bool ShowLevelIcon = false;
	
    bool ExceptionLogs = true;
    bool VerboseLogs = false;
    bool DebugLogs = false;
	
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
		
	
	
	
    void HeroesAndBanditsConfig()
    {

    }
	
	void Load(){
		if (FileExist(HeroesAndBanditsPATH)) //If config does not exist create File
	    {
	        	JsonFileLoader<HeroesAndBanditsConfig>.JsonLoadFile(HeroesAndBanditsPATH, this);
				if ( !ConfigVersion )
				{
					UpdateDefaults();
					JsonFileLoader<HeroesAndBanditsConfig>.JsonSaveFile(HeroesAndBanditsPATH, this);
				}
	    }else{
			createDefaults();
	        MakeDirectory(HeroesAndBanditsDIR);
	        MakeDirectory(HeroesAndBanditsPlayerDB);
			habPrint("Creating Default Config with "+ Actions.Count() +" Actions and " +  Levels.Count() +" Levels", "Always");	
	        JsonFileLoader<HeroesAndBanditsConfig>.JsonSaveFile(HeroesAndBanditsPATH, this);
		}
	}
	habLevel getLevel(float humanity){
		for ( int i =0; i < Levels.Count(); i++ )
		{
			float minHumanity = Levels.Get(i).MinHumanity;
			float maxHumanity = Levels.Get(i).MaxHumanity;
			if ( minHumanity != -1 && maxHumanity != -1 && humanity >= minHumanity && humanity <= maxHumanity){
				return Levels.Get(i);
			}else if (minHumanity == -1 && maxHumanity != -1 && humanity <= maxHumanity){
				return Levels.Get(i);
			}else if (minHumanity != -1 && maxHumanity == -1 && humanity >= minHumanity){
				return Levels.Get(i);
			}
		}
		return DefaultLevel;
	}
	
	string getAffinity(float humanity){
		return getLevel(humanity).Affinity;
	}
	
	float getActionHumanity(string actionName){
		for ( int i =0; i < Actions.Count(); i++ )
		{
			if (Actions.Get(i).Name == actionName){
				if (Actions.Get(i).Affinity == "bandit"){
					return -Actions.Get(i).Humanity;
				}else if (Actions.Get(i).Affinity == "hero"){
					return Actions.Get(i).Humanity;
				}else if (Actions.Get(i).Affinity == "bambi"){
					return 0;
				}else{
					return 0;
					habPrint("Action: " +actionName+ " Affinity not specified", "Exception");	
				}
			}
		}
		habPrint("Action: " +actionName+ " not found", "Exception");	
		return 0;
	}
	
	habAction getAction(string actionName){
		for ( int i =0; i < Actions.Count(); i++ )
		{
			if (Actions.Get(i).Name == actionName){
					return Actions.Get(i);
			}
		}
		habPrint("Action: " +actionName+ " not found", "Exception");	
		return new habAction("Null", "Null", 0, false);
	}
	
	int getWarningMessageColor(){
		return ARGB(WarningMessageColor[0], WarningMessageColor[1], WarningMessageColor[2], WarningMessageColor[3]);
	}
	
	int getKillFeedMessageColor(){
		return ARGB(KillFeedMessageColor[0], KillFeedMessageColor[1], KillFeedMessageColor[2], KillFeedMessageColor[3]);
	}
	
	int getNotificationColor(){
		return ARGB(NotificationColor[0], NotificationColor[1], NotificationColor[2], NotificationColor[3]);
	}

	void addZone(string name, int x, int z, float minHumanity, float maxHumanity, int warningRadius, int killRadius, string warningMessage = ""){
		habZone tempZone = new ref habZone(name, x, z, minHumanity, maxHumanity, warningRadius, killRadius, warningMessage);
		if (tempZone.Name == "Default Zone"){
			tempZone.Guards.Insert(new ref habGuard(x, GetGame().SurfaceY(x, z), z));
		}
		Zones.Insert(tempZone);
		habPrint("Zone Added: " + name + " There are now " +  Zones.Count() + " Zones", "Verbose");	
	}
	void addLevel(string name, string affinity, string levelImage, string imageSet, float minHumanity, float maxHumanity){
		habLevel tempLevel = new ref habLevel(name, affinity, levelImage, imageSet, minHumanity, maxHumanity);
		Levels.Insert(tempLevel);
		habPrint("Level Added: " + name + " There are now " + Levels.Count() + " Levels", "Verbose");	
	}
	void addAction(string actionName, string affinity, float humanity, bool notifyPlayer = true){
		habAction tempAction = new ref habAction(actionName, affinity, humanity, notifyPlayer);
		Actions.Insert(tempAction);
		habPrint("Action Added: " + actionName + " There are now " + Actions.Count() + " Actions", "Verbose");	
	}
	
	void createDefaults(){
			addLevel( "Bambi", "bambi", "HeroesAndBandits/gui/images/BambiNotification.paa", "set:HeroesAndBandits image:Bambi", -1000, 1000);
			addLevel( "Hero Lv1", "hero", "HeroesAndBandits/gui/images/HeroNotificationlv1.paa", "set:HeroesAndBandits image:HeroLv1", 1001, 4000);
			addLevel( "Hero Lv2", "hero", "HeroesAndBandits/gui/images/HeroNotificationlv2.paa", "set:HeroesAndBandits image:HeroLv2", 4001, 12000);
			addLevel( "Hero Lv3", "hero", "HeroesAndBandits/gui/images/HeroNotificationlv3.paa", "set:HeroesAndBandits image:HeroLv3", 12001, 20000);
			addLevel( "Hero Lv4", "hero", "HeroesAndBandits/gui/images/HeroNotificationlv4.paa",  "set:HeroesAndBandits image:HeroLv4", 20001, 50000);
			addLevel( "Hero Lv5", "hero", "HeroesAndBandits/gui/images/HeroNotificationlv5.paa",  "set:HeroesAndBandits image:HeroLv5", 50001, -1);
			addLevel( "Bandit Lv1", "bandit", "HeroesAndBandits/gui/images/BanditNotificationlv1.paa", "set:HeroesAndBandits image:BanditLv1", -4000, -1001);
			addLevel( "Bandit Lv2", "bandit", "HeroesAndBandits/gui/images/BanditNotificationlv2.paa", "set:HeroesAndBandits image:BanditLv2", -12000, -4001);
			addLevel( "Bandit Lv3", "bandit", "HeroesAndBandits/gui/images/BanditNotificationlv3.paa", "set:HeroesAndBandits image:BanditLv3", -20000, -12001);
			addLevel( "Bandit Lv4", "bandit", "HeroesAndBandits/gui/images/BanditNotificationlv4.paa", "set:HeroesAndBandits image:BanditLv4", -50000, -20001);
			addLevel( "Bandit Lv5", "bandit", "HeroesAndBandits/gui/images/BanditNotificationlv5.paa", "set:HeroesAndBandits image:BanditLv5", -1, -50001);
			addAction( "ZombieKill", "hero", 5);
			addAction( "heroSucide", "bandit", 150);
			addAction( "banditSucide", "hero", 150);
			addAction( "bambiSucide", "bambi", 0, false);
			addAction( "heroVshero", "bandit", 150);
			addAction( "heroVsbambi", "bandit", 300);
			addAction( "heroVsbandit", "hero", 250);
			addAction( "banditVshero", "bandit", 250);
			addAction( "banditVsbambi", "bandit", 125);
			addAction( "banditVsbandit", "bandit", 150);
			addAction( "bambiVshero", "bandit", 250);
			addAction( "bambiVsbambi", "bandit", 100);
			addAction( "bambiVsbandit", "hero", 300);
			addAction( "CombinationLockRaid", "bandit", 150);
			addAction( "FencePartRaid", "bandit", 100);
			addAction( "BandagePlayer", "hero", 50);
			addAction( "GiveBloodPlayer", "hero", 25);
			addAction( "GiveSalinePlayer", "hero", 25);
			addAction( "GivePlayerCPR", "hero", 75);
			addZone("Default Zone", 11250, 4300, -1000, 1000, 75, 50);
	}

	void UpdateDefaults()
	{
		ConfigVersion = "1";
		AllowStatCommand = true;
		AllowHumanityCommand = true;
		ShowLevelIcon = true;
	}

}



class habLevel
{
    string Name;
	string Affinity; //bandit / hero / bambi
	string LevelImage;
	string ImageSet;
    float MinHumanity;
    float MaxHumanity;

    void habLevel(string name, string affinity, string levelImage, string imageSet, float minHumanity, float maxHumanity) 
	{
        Name = name;
		Affinity = affinity;
		LevelImage = levelImage;
		ImageSet = imageSet;
        MinHumanity = minHumanity;
		MaxHumanity = maxHumanity;
    }
}

class habAction
{
    string Name;
	string Affinity; //bandit / hero / bambi
    float Humanity;
	bool NotifiyPlayer;

    void habAction(string actionName, string affinity, float humanity, bool notifiyPlayer = true) 
	{
        Name = actionName;
		Affinity = affinity;
        Humanity = humanity;
		NotifiyPlayer = notifiyPlayer;
    }
}


class habZone
{
    string Name;
	float X;
	float Z;
	int KillRadius;
    float MinHumanity;
    float MaxHumanity;
	int WarningRadius;
	bool ShowWarningMsg = true;
	string WarningMessage = "";
	bool ShowWelcomeMsg = false;
	string WelcomeMessage = "Welcome to the Default Zone";
	string WelcomeIcon = "HeroesAndBandits/gui/images/BambiNotification.paa";
	ref array<int> WelcomeMessageColor = {200, 0, 200, 200};
	bool OverrideSafeZone = false;
	bool GodModPlayers = false;
	ref array< ref habGuard > Guards = new ref array< ref habGuard >;
	
    void habZone(string name, float x, float z, float minHumanity, float maxHumanity, int warningRadius, int killRadius, string warningMessage = "", bool overrideSafeZone = false, bool godModPlayers = false) 
	{
        Name = name;
		X = x;
		Z = z;
		WarningRadius = warningRadius;
		KillRadius = killRadius;
	    MinHumanity = minHumanity;
	    MaxHumanity = maxHumanity;
		OverrideSafeZone = overrideSafeZone;
		GodModPlayers = godModPlayers;
		if (warningMessage == ""){
			WarningMessage = "!!Warning!! you are about to enter " + name + " if you continue you will be shot!";
		}else{
			WarningMessage = warningMessage;
		}
    }
	
	vector getVector(){
		return Vector( X, GetGame().SurfaceY(X, Z), Z );
	}
	
	bool validHumanity(float humanity){
			if ( MinHumanity != -1 && MaxHumanity != -1 && humanity >= MinHumanity && humanity <= MaxHumanity){
				return true;
			}else if (MinHumanity == -1 && MaxHumanity == -1){
				return true;
			}else if (MinHumanity == -1 && MaxHumanity != -1 && humanity <= MaxHumanity){
				return true;
			}else if (MinHumanity != -1 && MaxHumanity == -1 && humanity >= MinHumanity){
				return true;
			}
		return false;
	}

	int getWelcomeMessageColor(){
		return ARGB(WelcomeMessageColor[0], WelcomeMessageColor[1], WelcomeMessageColor[2], WelcomeMessageColor[3]);
	}
}


class habGuard
{
    float X;
    float Y;
    float Z;
	float Orientation;
	string Skin = "";
	string WeaponInHands = "M4A1";
	string WeaponInHandsMag = "Mag_STANAGCoupled_30Rnd";
	ref TStringArray WeaponInHandsAttachments =  {"M4_RISHndgrd", "M4_OEBttstck", "M68Optic"};
	ref TStringArray GuardGear =  { "PlateCarrierVest", "JungleBoots_Black", "CargoPants_Black", "M65Jacket_Black"};

    void habGuard(float x, float y, float z, float orientation = 0.0, string skin = "") 
	{
        X = x;
		Y = y;
        Z = z;
		Orientation = orientation;
		if ( skin == ""){
		 	Skin = GetHeroesAndBanditsConfig().BanditSkins.GetRandomElement();
		}else {
			Skin = skin;
		}
    }

	vector getVector(){
		return Vector( X, Y, Z );
	}
}

static ref HeroesAndBanditsConfig GetHeroesAndBanditsConfig()
{
	if (!m_HeroesAndBanditsConfig)
	{
		m_HeroesAndBanditsConfig = new HeroesAndBanditsConfig;
		m_HeroesAndBanditsConfig.Load();
	}
	return m_HeroesAndBanditsConfig;
}