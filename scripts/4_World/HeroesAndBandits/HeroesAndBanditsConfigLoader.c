/*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
*                                                                                   *
*                                                                                   *
*   LEGACY FILE WILL BE REMOVED ONLY BEING KEPT TO HELP WITH UPGRADE TO NEW CONFIG  *
*                                                                                   *
*                                                                                   *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
*/
static string HeroesAndBanditsDIR = "$profile:HeroesAndBandits";
static string HeroesAndBanditsPATH = HeroesAndBanditsDIR + "\\config.json";

class HeroesAndBanditsConfig
{ 
	//Default Values
	string ConfigVersion = "3";
	ref array< ref habOldLevel > Levels = new ref array< ref habOldLevel >;
	ref habOldLevel DefaultLevel = new ref habOldLevel("Bambi", "bambi", "HeroesAndBandits/gui/images/BambiNotification.paa", -1000, 1000);
	
	ref array< ref habOldAction > Actions = new ref array< ref habOldAction >;
	
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
	
	string CommandPrefix = "/";
	
	bool AllowStatCommand = true;
	bool AllowHumanityCommand = true;
	
	bool AllowGUI = true;
	string GUIHeading = "#HAB_TITLE";
	bool HideKillsInGUI = false;
	
	bool ShowLevelIcon = true;
	int  LevelIconLocation = 2;
	
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
	
	// Load config file or create default file if config doesn't exsit
	void Load(){
		if (FileExist(HeroesAndBanditsPATH)) //If config exist load File
		{
	        	JsonFileLoader<HeroesAndBanditsConfig>.JsonLoadFile(HeroesAndBanditsPATH, this);
				if ( !ConfigVersion )
				{
					UpdateDefaults();
				}
				if ( ConfigVersion == "1" )
				{
					UpdateDefaultsV2();
				}
				if ( ConfigVersion == "2" )
				{
					UpdateDefaultsV3();
	       			JsonFileLoader<HeroesAndBanditsConfig>.JsonSaveFile(HeroesAndBanditsPATH, this);
				}
			
		}else{ //File does not exist create file
			habPrint("Something went wrong trying to create orginal config", "Exception");	
		}
	}

	//Returns the level based on humanity value
	habOldLevel getLevel(float humanity){
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
	
        //Returns the players Affinity based on their level
	string getAffinity(float humanity){
		return getLevel(humanity).Affinity;
	}

	//Returns the Humanity value for the action requested
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

	//Returns the action based on the name it is NOT case sensitive
	habOldAction getAction(string actionName){
		string tempActionName
		for ( int i =0; i < Actions.Count(); i++ )
		{
			tempActionName = Actions.Get(i).Name;
			tempActionName.ToLower();
			actionName.ToLower();
			if ( tempActionName == actionName ){
					return Actions.Get(i);
			}
		}
		habPrint("Action: " + actionName + " not found", "Exception");	
		return new habOldAction("Null", "Null", 0, false);
	}
	
	//Returns the warning message color in an int value
	int getWarningMessageColor(){
		return ARGB(WarningMessageColor[0], WarningMessageColor[1], WarningMessageColor[2], WarningMessageColor[3]);
	}
	
	//Returns the kill feed message color in an int value
	int getKillFeedMessageColor(){
		return ARGB(KillFeedMessageColor[0], KillFeedMessageColor[1], KillFeedMessageColor[2], KillFeedMessageColor[3]);
	}

	//Returns the Notification color in an int value
	int getNotificationColor(){
		return ARGB(NotificationColor[0], NotificationColor[1], NotificationColor[2], NotificationColor[3]);
	}


	//Helper function for defaults to add Zones
	void addZone(string name, int x, int z, float minHumanity, float maxHumanity, int warningRadius, int killRadius, string warningMessage = ""){
		habZone tempZone = new ref habZone(name, x, z, minHumanity, maxHumanity, warningRadius, killRadius, warningMessage);
		if (tempZone.Name == "Default Zone"){
			tempZone.Guards.Insert(new ref habGuard(x, GetGame().SurfaceY(x, z), z));
		}
		Zones.Insert(tempZone);
		habPrint("Zone Added: " + name + " There are now " +  Zones.Count() + " Zones", "Verbose");	
	}


	//Helper function for adding levels
	void addLevel(string name, string affinity, string levelImage, float minHumanity, float maxHumanity){
		habOldLevel tempLevel = new ref habOldLevel(name, affinity, levelImage, minHumanity, maxHumanity);
		Levels.Insert(tempLevel);
		habPrint("Level Added: " + name + " There are now " + Levels.Count() + " Levels", "Verbose");	
	}


	//Helper function for adding Actions
	void addAction(string actionName, string affinity, float humanity,  bool notifyPlayer = true){
		habOldAction tempAction = new ref habOldAction(actionName, affinity, humanity, notifyPlayer);
		Actions.Insert(tempAction);
		habPrint("Action Added: " + actionName + " There are now " + Actions.Count() + " Actions", "Verbose");	
	}
	

	void UpdateDefaults()
	{
		ConfigVersion = "1";
		AllowStatCommand = true;
		AllowHumanityCommand = true;
		ShowLevelIcon = false;
	}

	void UpdateDefaultsV2()
	{
		ConfigVersion = "2";
		LevelIconLocation = 2;
		addAction( "HuntAnimal_BosTaurus_Brown", "hero", 0, false);
		addAction( "HuntAnimal_BosTaurus_White", "hero", 0, false);
		addAction( "HuntAnimal_BosTaurus_Spotted", "hero", 0, false);
		addAction( "HuntAnimal_BosTaurusF_Brown", "hero", 0, false);
		addAction( "HuntAnimal_BosTaurusF_White", "hero", 0, false);
		addAction( "HuntAnimal_BosTaurusF_Spotted", "hero",  0, false);
		addAction( "HuntAnimal_CanisLupus_Grey", "hero", 0, false);
		addAction( "HuntAnimal_CanisLupus_White", "hero", 0, false);
		addAction( "HuntAnimal_CapraHircus_Brown", "hero", 0, false);
		addAction( "HuntAnimal_CapraHircus_Black", "hero", 0, false);
		addAction( "HuntAnimal_CapraHircus_White", "hero", 0, false);
		addAction( "HuntAnimal_CapraHircusF", "hero", 0, false);
		addAction( "HuntAnimal_CapreolusCapreolus", "hero", 0, false);
		addAction( "HuntAnimal_CapreolusCapreolusF_Brown", "hero", 0, false);
		addAction( "HuntAnimal_CapreolusCapreolusF_Spotted", "hero", 0, false);
		addAction( "HuntAnimal_CapreolusCapreolusF_White", "hero", 0, false);
		addAction( "HuntAnimal_CervusElaphus", "hero", 0, false);
		addAction( "HuntAnimal_CervusElaphusF", "hero", 0, false);
		addAction( "HuntAnimal_GallusGallusDomesticus", "hero", 0, false);
		addAction( "HuntAnimal_GallusGallusDomesticusF", "hero", 0, false);
		addAction( "HuntAnimal_OvisAries", "hero", 0, false);
		addAction( "HuntAnimal_OvisAriesF", "hero", 0, false);
		addAction( "HuntAnimal_SusDomesticus", "hero", 0, false);
		addAction( "HuntAnimal_SusScrofa", "hero", 0, false);
		addAction( "HuntAnimal_LepusEuropaeus", "hero", 0, false);
		addAction( "HuntAnimal_UrsusArctos", "hero", 0, false);
		
	}
	
	void UpdateDefaultsV3()
	{
		ConfigVersion = "3";
		CommandPrefix = "/";
		AllowGUI = true;
		HideKillsInGUI = false;
		GUIHeading = "#HAB_TITLE";
	}
	
	
	
}


//Class for holding levels
class habOldLevel
{
	string Name;
	string Affinity; //bandit / hero / bambi
	string LevelImage;
	float MinHumanity;
	float MaxHumanity;

	void habOldLevel(string name, string affinity, string levelImage, float minHumanity, float maxHumanity) 
	{
		Name = name;
		Affinity = affinity;
		LevelImage = levelImage;
		MinHumanity = minHumanity;
		MaxHumanity = maxHumanity;
	}
}


//Class for holding Actions
class habOldAction
{
	string Name;
	string Affinity; //bandit / hero / bambi
	float Humanity;
	bool NotifiyPlayer;

	void habOldAction(string actionName, string affinity, float humanity, bool notifiyPlayer = true) 
	{
		Name = actionName;
		Affinity = affinity;
		Humanity = humanity;
		NotifiyPlayer = notifiyPlayer;
	}
}


//Helper function to return Config
static ref HeroesAndBanditsConfig GetHeroesAndBanditsConfig()
{
	if (!m_HeroesAndBanditsConfig)
	{
		m_HeroesAndBanditsConfig = new HeroesAndBanditsConfig;
		m_HeroesAndBanditsConfig.Load();
	}
	return m_HeroesAndBanditsConfig;
}
