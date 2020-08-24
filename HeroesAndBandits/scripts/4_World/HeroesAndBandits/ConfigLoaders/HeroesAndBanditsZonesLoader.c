class HeroesAndBanditsConfigZones
{ 
	//Has to be in world as it uses other configs for refernce
	//Default Values
	string ConfigVersion = "5";
	
	int ZoneCheckTimer = 3;
	
	
	ref array<int> WarningMessageColor = {200, 255, 0, 0};
	
	int NotificationMessageTime = 15;
	
	string WarningMessageImagePath = "HeroesAndBandits/gui/images/DeathWarning.paa";
	// Load config file or create default file if config doesn't exsit
	
	ref array< ref habZone > Zones = new ref array< ref habZone >;
	
	
	void Load(){
		if (GetGame().IsServer()){
			ref HeroesAndBanditsSimpleConfig simpleConfig = new ref HeroesAndBanditsSimpleConfig();
			simpleConfig.Load();
			if (simpleConfig.UseSimple != 0){
				if (FileExist(habConstant.ZonesPATH)) //If config exist load File
				{
			        JsonFileLoader<HeroesAndBanditsConfigZones>.JsonLoadFile(habConstant.ZonesPATH, this);
					if (ConfigVersion == "4"){
						doV5Upgrade();
					}
				}else{ //File does not exist create file
					createDefaults();
					Print("Creating Default Zones Config");	
					Save();
				}
			}
		}
	}
	
	void Save(){
		JsonFileLoader<HeroesAndBanditsConfigZones>.JsonSaveFile(habConstant.ZonesPATH, this);
	}
	
		//Returns the warning message color in an int value
	int getWarningMessageColor(){
		return ARGB(WarningMessageColor[0], WarningMessageColor[1], WarningMessageColor[2], WarningMessageColor[3]);
	}
	
	//Helper function for defaults to add Zones
	void addZone(string name, int x, int z, int warningRadius, int killRadius, string warningMessage = ""){
		habZone tempZone = new ref habZone(name, x, z, warningRadius, killRadius, warningMessage);
		if (tempZone.Name == "Default Zone"){
			tempZone.Guards.Insert(new ref habGuard(x, GetGame().SurfaceY(x, z), z));
			tempZone.Affinities.Insert(new ref habZoneAffinity("bambi"));
		}
		Zones.Insert(tempZone);
		Print("Zone Added: " + name + " There are now " +  Zones.Count() + " Zones");	
	}
	
	void createDefaults(){
		addZone("Default Zone", 11250, 4300, 75, 50);
	}
	
	void doV5Upgrade(){
		ConfigVersion = "5";
		if (Zones.Count() > 0){
			for (int i = 0; i < Zones.Count(); i++){
				Zones.Get(i).convertHumanityToAffinty();
				if (Zones.Get(i).WelcomeIcon == "HeroesAndBandits/gui/images/BambiNotification.paa"){
					Zones.Get(i).WelcomeIcon = "HeroesAndBandits/gui/images/Bambi.paa";
				}
				if (Zones.Get(i).WelcomeIcon == "HeroesAndBandits/gui/images/HeroNotification.paa"){
					Zones.Get(i).WelcomeIcon = "HeroesAndBandits/gui/images/Hero.paa";
				}
				if (Zones.Get(i).WelcomeIcon == "HeroesAndBandits/gui/images/BanditNotification.paa"){
					Zones.Get(i).WelcomeIcon = "HeroesAndBandits/gui/images/Bandit.paa";
				}
				if (Zones.Get(i).Guards && Zones.Get(i).Guards.Count() > 0){
					for(int j = 0; j < Zones.Get(i).Guards.Count(); j++){
						habConverter().init();
						string soundset = habConverter().GunToSound.Get(Zones.Get(i).Guards.Get(j).WeaponInHands);
						float damage = 24;
						if (soundset && soundset != ""){
							Zones.Get(i).Guards.Get(j).GunSound = soundset;
							damage = habConverter().GunToDmg.Get(Zones.Get(i).Guards.Get(j).WeaponInHands);
						} else {
							Zones.Get(i).Guards.Get(j).GunSound = "M4_Shot_SoundSet";
						}
						Zones.Get(i).Guards.Get(j).DamagePerTickMin = damage;
						Zones.Get(i).Guards.Get(j).DamagePerTickRand = damage * 1.2;
						Zones.Get(i).Guards.Get(j).GunTickMulitplier = 3.0;
						Zones.Get(i).Guards.Get(j).HitChance = 1;
						Zones.Get(i).Guards.Get(j).RespawnTimer = 1200;
						Zones.Get(i).Guards.Get(j).CanBeKilled = false;
						Zones.Get(i).Guards.Get(j).RequireLineOfSight = true;
					}
				}
			}
		}
		Save();
	}
	
};



//Class for holding Zones
class habZone
{
	string Name;
	float X;
	float Z;
	float WarningRadius;
	float KillRadius;
	float MinHumanity;
	float MaxHumanity;
	bool ShowWarningMsg = true;
	string WarningMessage = "";
	bool ShowWelcomeMsg = false;
	string WelcomeMessage = "Welcome to the Default Zone";
	string WelcomeIcon = "HeroesAndBandits/gui/images/Bambi.paa";
	ref array<int> WelcomeMessageColor = {200, 0, 200, 200};
	bool OverrideSafeZone = false;
	bool GodModPlayers = false;
	bool PreventWeaponRaise = false;
	bool PreventActions = false;
	bool BlockTrader = false;
	bool KillAggressors = false;
	ref array< ref habZoneAffinity > Affinities = new ref array< ref habZoneAffinity >;
	ref array< ref habGuard > Guards = new ref array< ref habGuard >;
	
	ref array< ref habZone > SubZones = new ref array< ref habZone >;
	
	void habZone(string name, float x, float z, int warningRadius, int killRadius, string warningMessage = "", bool overrideSafeZone = false, bool godModPlayers = false) 
	{
		Name = name;
		X = x;
		Z = z;
		WarningRadius = warningRadius;
		KillRadius = killRadius;		
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
	
	void convertHumanityToAffinty(){
		
		bool HeroesAdded = false;
		bool BanditsAdded = false;
		bool BambiAdded = false;
		if (MaxHumanity == 0 && MinHumanity == 0){ //Allow all no players
			return;
		}
		
		if (MaxHumanity == -1 && MinHumanity == -1){ //Allow all players
			Affinities.Insert(new ref habZoneAffinity("bambi"));
			Affinities.Insert(new ref habZoneAffinity("hero"));
			Affinities.Insert(new ref habZoneAffinity("bandit"));
			return;
		}
		if ((MinHumanity < GetHeroesAndBanditsLevels().DefaultLevel.MinPoints && MaxHumanity > GetHeroesAndBanditsLevels().DefaultLevel.MaxPoints) || (MaxHumanity <= GetHeroesAndBanditsLevels().DefaultLevel.MaxPoints && MaxHumanity != -1 && MaxHumanity >= 0) || ( MinHumanity >= -GetHeroesAndBanditsLevels().DefaultLevel.MinPoints && MinHumanity <= 0 && MinHumanity != -1)){ //Default zone
			Affinities.Insert(new ref habZoneAffinity("bambi"));
			BambiAdded = true;
		}
		
		//Bandits
		if (GetHeroesAndBanditsLevels().DefaultLevel.MinPoints || MinHumanity == -1){
			float newBanditsMax = -1;
			float newBanditsMin = -1;
			if (MaxHumanity >= 0){
				newBanditsMin = 0;
			} else {
				newBanditsMin = 0 - MaxHumanity;
			}
			if (MinHumanity != -1){
				newBanditsMax = 0 - MinHumanity;
			}
			Affinities.Insert(new ref habZoneAffinity("bandit", newBanditsMin, newBanditsMax));
			BanditsAdded = true;
		}
		
		//Heroes
		if (MaxHumanity >= GetHeroesAndBanditsLevels().DefaultLevel.MaxPoints || MaxHumanity == -1){
			float newHeroesMax = -1;
			float newHeroesMin = -1;
			if (MinHumanity <= 0){
				newHeroesMin = 0;
			} else {
				newHeroesMin = MinHumanity;
			}
			if (MaxHumanity != -1){
				newHeroesMax = MaxHumanity;
			}
			Affinities.Insert(new ref habZoneAffinity("hero", newHeroesMin, newHeroesMax));
			HeroesAdded = true;
		}
		
		if (HeroesAdded && BanditsAdded && !BambiAdded){
			Affinities.Insert(new ref habZoneAffinity("bambi"));
		}
	}
};

class habZoneAffinity{
	string Affinity;
	float MinPoints = -1;
	float MaxPoints = -1;
	
	void habZoneAffinity(string affinity, float minPoints = -1, float maxPoints = -1){
		Affinity = affinity;
		MinPoints = minPoints;
		MaxPoints = maxPoints;
	}
	
	bool Check(float points, string affinity = ""){
		if (affinity != "")
		{
			if (Affinity != affinity){
				return false;
			}
		}
		if ( Affinity == GetHeroesAndBanditsLevels().DefaultAffinity.Name ){ //Default affinity doesn't have points
			return true; 
		}
		if ( MinPoints != -1 && MaxPoints != -1 && points >= MinPoints && points <= MaxPoints){
			return true;
		}else if (MinPoints == -1 && MaxPoints == -1){
			return true;
		}else if (MinPoints == -1 && MaxPoints != -1 && points <= MaxPoints){
			return true;
		}else if (MinPoints != -1 && MaxPoints == -1 && points >= MinPoints){
			return true;
		}
		
		return false;
	}
	
};

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
	string GunSound = "M4_Shot_SoundSet";
	float DamagePerTickMin = 28;
	float DamagePerTickRand = 35;
	float GunTickMulitplier = 2.0;
	float HitChance = 1;
	float RespawnTimer = 1200;
	bool CanBeKilled = false;
	bool RequireLineOfSight = true;

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
};
