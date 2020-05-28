static string HeroesAndBanditsDIR = "$profile:HeroesAndBandits";
static string HeroesAndBanditsPlayerDB =  HeroesAndBanditsDIR + "\\PlayerDB";
static string HeroesAndBanditsPATH = HeroesAndBanditsDIR + "\\config.json";

class HeroesAndBanditsConfig
{ 
	//Default Values
	
	ref array< ref habLevel > Levels = new ref array< ref habLevel >;
	ref habLevel DefaultLevel = new ref habLevel("Bambi", "bambi", "HeroesAndBandits/gui/images/BambiNotification.paa", -1000, 1000);
	
	ref array< ref habAction > Actions = new ref array< ref habAction >;
	
	ref array< ref habZone > Zones = new ref array< ref habZone >;
	
	ref array<int> NotificationColor = {200, 0, 200, 200};
	
	ref array<int> WarningMessageColor = {200, 255, 0, 0};
	
	string WarningMessageImagePath = "HeroesAndBandits/gui/images/DeathWarning.paa";
	
	int NotificationMessageTime = 15;
	
	int ZoneCheckTimer = 2;
	
	bool NotifyLevelChange = true;
	
    bool ExceptionLogs = true;
    bool VerboseLogs = false;
    bool DebugLogs = false;
	
	
    void HeroesAndBanditsConfig()
    {

    }
	
	void Load(){
		if (FileExist(HeroesAndBanditsPATH)) //If config does not exist create File
	    {
	        	JsonFileLoader<HeroesAndBanditsConfig>.JsonLoadFile(HeroesAndBanditsPATH, this);
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
	
	int getNotificationColor(){
		return ARGB(NotificationColor[0], NotificationColor[1], NotificationColor[2], NotificationColor[3]);
	}

	void addZone(string name, int x, int z, float minHumanity, float maxHumanity, int warningRadius, int killRadius, string warningMessage = ""){
		habZone tempZone = new ref habZone(name, x, z, minHumanity, maxHumanity, warningRadius, killRadius, warningMessage);
		if (tempZone.Name == "Default Zone"){
			tempZone.Gaurds.Insert(new ref habGuard(x, GetGame().SurfaceY(x, z), z));
		}
		Zones.Insert(tempZone);
		habPrint("Zone Added: " + name + " There are now " +  Zones.Count() + " Zones", "Verbose");	
	}
	void addLevel(string name, string affinity, string levelImage, float minHumanity, float maxHumanity){
		habLevel tempLevel = new ref habLevel(name, affinity, levelImage, minHumanity, maxHumanity);
		Levels.Insert(tempLevel);
		habPrint("Level Added: " + name + " There are now " + Levels.Count() + " Levels", "Verbose");	
	}
	void addAction(string actionName, string affinity, float humanity, bool notifyPlayer = true){
		habAction tempAction = new ref habAction(actionName, affinity, humanity, notifyPlayer);
		Actions.Insert(tempAction);
		habPrint("Action Added: " + actionName + " There are now " + Actions.Count() + " Actions", "Verbose");	
	}
	
	void createDefaults(){
			addLevel( "Bambi", "bambi", "HeroesAndBandits/gui/images/BambiNotification.paa", -1000, 1000);
			addLevel( "Hero Lv1", "hero", "HeroesAndBandits/gui/images/HeroNotificationlv1.paa", 1001, 4000);
			addLevel( "Hero Lv2", "hero", "HeroesAndBandits/gui/images/HeroNotificationlv2.paa", 4001, 12000);
			addLevel( "Hero Lv3", "hero", "HeroesAndBandits/gui/images/HeroNotificationlv3.paa", 12001, 20000);
			addLevel( "Hero Lv4", "hero", "HeroesAndBandits/gui/images/HeroNotificationlv4.paa", 20001, 50000);
			addLevel( "Hero Lv5", "hero", "HeroesAndBandits/gui/images/HeroNotificationlv5.paa", 50001, -1);
			addLevel( "Bandit Lv1", "bandit", "HeroesAndBandits/gui/images/BanditNotificationlv1.paa", -4000, -1001);
			addLevel( "Bandit Lv2", "bandit", "HeroesAndBandits/gui/images/BanditNotificationlv2.paa", -12000, -4001);
			addLevel( "Bandit Lv3", "bandit", "HeroesAndBandits/gui/images/BanditNotificationlv3.paa", -20000, -12001);
			addLevel( "Bandit Lv4", "bandit", "HeroesAndBandits/gui/images/BanditNotificationlv4.paa", -50000, -20001);
			addLevel( "Bandit Lv5", "bandit", "HeroesAndBandits/gui/images/BanditNotificationlv5.paa", -1, -50001);
			addAction( "ZombieKill", "hero", 5);
			addAction( "heroSucide", "bandit", 150);
			addAction( "banditSucide", "hero", 150);
			addAction( "bambiSucide", "bambi", 0, false);
			addAction( "heroVshero", "bandit", 200);
			addAction( "heroVsbambi", "bandit", 400);
			addAction( "heroVsbandit", "hero", 250);
			addAction( "banditVshero", "bandit", 200);
			addAction( "banditVsbambi", "bandit", 200);
			addAction( "banditVsbandit", "bandit", 150);
			addAction( "bambiVshero", "bandit", 350);
			addAction( "bambiVsbambi", "bandit", 100);
			addAction( "bambiVsbandit", "hero", 350);
			addAction( "CombinationLockRaid", "bandit", 250);
			addAction( "FencePartRaid", "bandit", 100);
			addZone("Default Zone", 11250, 4300, -1000, 1000, 75, 50);
	}
}



class habLevel
{
    string Name;
	string Affinity; //bandit / hero / bambi
	string LevelImage;
    float MinHumanity;
    float MaxHumanity;

    void habLevel(string name, string affinity, string levelImage,float minHumanity, float maxHumanity) 
	{
        Name = name;
		Affinity = affinity;
		LevelImage = levelImage;
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
	int X;
	int Z;
	int WarningRadius;
	string WarningMessage;
	int KillRadius;
    float MinHumanity;
    float MaxHumanity;
	ref array< ref habGuard > Gaurds = new ref array< ref habGuard >;
	
    void habZone(string name, int x, int z, float minHumanity, float maxHumanity, int warningRadius, int killRadius, string warningMessage = "") 
	{
        Name = name;
		X = x;
		Z = z;
		WarningRadius = warningRadius;
		KillRadius = killRadius;
	    MinHumanity = minHumanity;
	    MaxHumanity = maxHumanity;
		if (warningMessage == ""){
			WarningMessage = "Warning!! you are about to enter " + name + " if you continue you will be shot!";
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
}

class habGuard
{
    int X;
    int Y;
    int Z;

    void habGuard(int x, int y, int z) 
	{
        X = x;
		Y = y;
        Z = z;
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