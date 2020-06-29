static string HeroesAndBanditsZonesPATH = HeroesAndBanditsDirectory + "\\zones.json";

class HeroesAndBanditsConfigZones
{ 
	//Default Values
	string ConfigVersion = "4";
	
	int ZoneCheckTimer = 3;
	
	
	ref array<int> WarningMessageColor = {200, 255, 0, 0};
	
	int NotificationMessageTime = 15;
	
	string WarningMessageImagePath = "HeroesAndBandits/gui/images/DeathWarning.paa";
	// Load config file or create default file if config doesn't exsit
	
	ref array< ref habZone > Zones = new ref array< ref habZone >;
	
	
	void Load(){
		habCheckUpgradeToConfigV4();
		if (FileExist(HeroesAndBanditsZonesPATH)) //If config exist load File
		{
	        	JsonFileLoader<HeroesAndBanditsConfigZones>.JsonLoadFile(HeroesAndBanditsZonesPATH, this);
		}else{ //File does not exist create file
			createDefaults();
			habPrint("Creating Default Zones Config", "Always");	
			JsonFileLoader<HeroesAndBanditsConfigZones>.JsonSaveFile(HeroesAndBanditsZonesPATH, this);
		}
	}
	
		//Returns the warning message color in an int value
	int getWarningMessageColor(){
		return ARGB(WarningMessageColor[0], WarningMessageColor[1], WarningMessageColor[2], WarningMessageColor[3]);
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
	
	void createDefaults(){
		addZone("Default Zone", 11250, 4300, -1000, 1000, 75, 50);
	}
}



//Class for holding Zones
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
	ref array< ref habZone > SubZones = new ref array< ref habZone >;
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
	
	//Converts the x and y to vector
	vector getVector(){
		return Vector( X, GetGame().SurfaceY(X, Z), Z );
	}
	
	//Helper function to check if player has valid humanity for the zone
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

	//Returns the welcome color in an int value
	int getWelcomeMessageColor(){
		return ARGB(WelcomeMessageColor[0], WelcomeMessageColor[1], WelcomeMessageColor[2], WelcomeMessageColor[3]);
	}
}


//Class for holding guard values
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
		 	Skin = GetHeroesAndBanditsSettings().BanditSkins.GetRandomElement();
		}else {
			Skin = skin;
		}
	}

	//Converts the x and y to vector
	vector getVector(){
		return Vector( X, Y, Z );
	}
}

//Helper function to return Config
static ref HeroesAndBanditsConfigZones GetHeroesAndBanditsZones()
{
	if (!m_HeroesAndBanditsConfigZones)
	{
		m_HeroesAndBanditsConfigZones = new HeroesAndBanditsConfigZones;
		m_HeroesAndBanditsConfigZones.Load();
	}
	return m_HeroesAndBanditsConfigZones;
}