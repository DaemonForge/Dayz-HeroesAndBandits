class HeroesAndBanditsConfigActions
{ 
	//Default Values
	string ConfigVersion = "7";
	
	int NotificationMessageTime = 10;
	
	ref array<int> NotificationColor = {200, 0, 200, 200};
	
	ref array< ref habAction > Actions = new ref array< ref habAction >;
	ref array< ref habAggressorAction > AggressorActions = new ref array< ref habAggressorAction >;
	
	void Load(){
		if (GetGame().IsServer()){
			ref HeroesAndBanditsSimpleConfig simpleConfig = new ref HeroesAndBanditsSimpleConfig();
			simpleConfig.Load();
			if (simpleConfig.UseSimple == 0){
				if (FileExist(habConstant.ActionsPATH)) //If config exist load File
				{
			        	JsonFileLoader<HeroesAndBanditsConfigActions>.JsonLoadFile(habConstant.ActionsPATH, this);
						if(ConfigVersion == "4"){
							DoV5Upgrade();
						}
						if(ConfigVersion == "5"){
							DoV6Upgrade();
						}
						if(ConfigVersion == "6"){
							DoV7Upgrade();
						}
				}else{ //File does not exist create file
					createDefaults();
					habPrint("Creating Default Actions Config", "Always");	
					Save();
				}
				habPrint("Loading Actions Config Version: " + ConfigVersion + "- " + Actions.Count() + " Actions Loaded and " + AggressorActions.Count() + " AggressorActions Loaded", "Verbose");	
			}
		}
	}
	
	void Save(){
		JsonFileLoader<HeroesAndBanditsConfigActions>.JsonSaveFile(habConstant.ActionsPATH, this);
	}
	
	//Returns the Humanity value for the action requested
	float getActionHumanity(string actionName){
		for ( int i =0; i < Actions.Count(); i++ )
		{
			if (Actions.Get(i).Name == actionName){
				if (Actions.Get(i).Affinity == "bandit"){
					return -Actions.Get(i).Points;
				}else if (Actions.Get(i).Affinity == "hero"){
					return Actions.Get(i).Points;
				}else if (Actions.Get(i).Affinity == "bambi"){
					return 0;
				}else{
					return 0;
					habPrint("Action: " +actionName+ " Affinity not specified", "Exception");	
				}
			}
		}
		Print("Action: " +actionName+ " not found");	
		return 0;
	}
	
		//Returns the Humanity value for the action requested
	float getActionPoints(string actionName){
		for ( int i =0; i < Actions.Count(); i++ )
		{
			if (Actions.Get(i).Name == actionName){
				return Actions.Get(i).Points;
			}
		}
		Print("Action: " +actionName+ " not found");	
		return 0;
	}

	//Returns the action based on the name it is NOT case sensitive
	habAction getAction(string actionName){
		string tempActionName
		for ( int i = 0; i < Actions.Count(); i++ )
		{
			tempActionName = Actions.Get(i).Name;
			tempActionName.ToLower();
			actionName.ToLower();
			if ( tempActionName == actionName ){
					return Actions.Get(i);
			}
		}
		habPrint("Action: " + actionName + " not found", "Exception");	
		return new habAction("Null", "Null", "Null", 0, false);
	}
	
	//Helper function for adding Actions
	void addAction(string actionName, string affinity, string secondaryAffinity, float points, bool notifyPlayer = true){
		habAction tempAction = new ref habAction(actionName, affinity, secondaryAffinity, points, notifyPlayer);
		Actions.Insert(tempAction);
		habPrint("Action Added: " + actionName + " There are now " + Actions.Count() + " Actions", "Debug");	
	}
	
	//Returns the Notification color in an int value
	int getNotificationColor(){
		return ARGB(NotificationColor[0], NotificationColor[1], NotificationColor[2], NotificationColor[3]);
	}
	
	float getAggressionAmount(string action){
		for (int i = 0; i < AggressorActions.Count(); i++){
			if (AggressorActions.Get(i).Name == action){
				return AggressorActions.Get(i).Amount;
			}
		}
		return 0;
	}
	
	void createDefaults(){
		addAction( "ZombieKill", "hero", "none", 5);
		addAction( "heroSucide", "bandit", "none",100);
		addAction( "banditSucide", "hero", "none",100);
		addAction( "bambiSucide", "none", "none",0, false);
		addAction( "heroVshero", "bandit", "none",150);
		addAction( "heroVsbambi", "bandit","none", 300);
		addAction( "heroVsbandit", "hero", "none",250);
		addAction( "banditVshero", "bandit", "none",250);
		addAction( "banditVsbambi", "bandit", "none",125);
		addAction( "banditVsbandit", "bandit", "none",150);
		addAction( "bambiVshero", "bandit", "none",250);
		addAction( "bambiVsbambi", "bandit", "none",100);
		addAction( "bambiVsbandit", "hero", "none",300);
		addAction( "CombinationLockRaid", "bandit", "none", 150);
		addAction( "FencePartRaid", "bandit","none", 50);
		addAction( "WatchtowerPartRaid", "bandit", "none", 30);
		
		#ifdef EXPANSIONMOD
			addAction( "ExpansionWall6x3Raid", "bandit", "none", 150);
			addAction( "ExpansionRamp6x1_5x6Raid", "bandit", "none", 150);
			addAction( "ExpansionRamp3x1_5x6Raid", "bandit", "none", 150);
			addAction( "ExpansionStairS1_5x3x3Raid", "bandit", "none", 150);
			addAction( "ExpansionFloor_6x6Raid", "bandit", "none", 150);
			addAction( "ExpansionFloor_3x3Raid", "bandit", "none", 150);
			addAction( "ExpansionFloor_3x6Raid", "bandit", "none", 150);
			addAction( "ExpansionSafeMiniRaid", "bandit", "none", 100);
			addAction( "ExpansionSafeMediumRaid", "bandit", "none", 150);
			addAction( "ExpansionSafeLargeRaid", "bandit", "none", 200);
		#endif
		
		#ifdef EXPANSIONCODELOCKEXPANDED
			addAction( "ExpansionCodeLockTentRaid", "bandit", "none", 150);
			addAction( "HacktentRaid", "bandit", "none", 100);
			addAction( "HackexpansionsafeRaid", "bandit", "none", 200);
			addAction( "HackfenceRaid", "bandit", "none", 200);
			addAction( "HackexpansionwallRaid", "bandit", "none", 200);
			addAction( "HackexpansionfloorRaid", "bandit", "none", 200);
			addAction( "Hackbbp_t1Raid", "bandit", "none", 200);
			addAction( "Hackbbp_t2Raid", "bandit", "none", 250);
			addAction( "Hackbbp_t3Raid", "bandit", "none", 300);
		#endif
		
		#ifdef HACKINGMOD
			addAction( "HacktentRaid", "bandit", "none", 100);
			addAction( "HackfenceRaid", "bandit", "none", 200);
			addAction( "Hackbbp_t1Raid", "bandit", "none", 200);
			addAction( "Hackbbp_t2Raid", "bandit", "none", 250);
			addAction( "Hackbbp_t3Raid", "bandit", "none", 300);
		#endif
		
		
		addAction( "MedicBandagePlayer", "hero", "medic", 50);
		addAction( "MedicGiveBlood", "hero", "medic",25);
		addAction( "MedicGiveSaline", "hero","medic", 25);
		addAction( "MedicGiveCPR", "hero", "medic" ,75);
		addAction( "MedicFeedTetracycline", "hero", "medic", 15);
		addAction( "MedicFeedPainkiller", "hero", "medic", 15);
		addAction( "MedicFeedCharcoal", "hero", "medic", 15);
		addAction( "MedicFeedVitamin", "hero", "medic", 10);
		addAction( "MedicSplintPlayer", "hero", "medic", 100);
		addAction( "HuntAnimal_BosTaurus_Brown", "hunter", "none", 25, false);
		addAction( "HuntAnimal_BosTaurus_White", "hunter", "none", 25, false);
		addAction( "HuntAnimal_BosTaurus_Spotted", "hunter", "none", 25, false);
		addAction( "HuntAnimal_BosTaurusF_Brown", "hunter", "none", 25, false);
		addAction( "HuntAnimal_BosTaurusF_White", "hunter", "none", 25, false);
		addAction( "HuntAnimal_BosTaurusF_Spotted", "hunter","none", 25, false);
		addAction( "HuntAnimal_CanisLupus_Grey", "hunter", "none", 25, false);
		addAction( "HuntAnimal_CanisLupus_White", "hunter", "none", 25, false);
		addAction( "HuntAnimal_CapraHircus_Brown", "hunter", "none", 25, false);
		addAction( "HuntAnimal_CapraHircus_Black", "hunter", "none", 25, false);
		addAction( "HuntAnimal_CapraHircus_White", "hunter", "none", 25, false);
		addAction( "HuntAnimal_CapraHircusF", "hunter", "none", 25, false);
		addAction( "HuntAnimal_CapreolusCapreolus", "hunter","none", 25, false);
		addAction( "HuntAnimal_CapreolusCapreolusF_Brown", "hunter", "none", 25, false);
		addAction( "HuntAnimal_CapreolusCapreolusF_Spotted", "hunter","none", 25, false);
		addAction( "HuntAnimal_CapreolusCapreolusF_White", "hunter", "none", 25, false);
		addAction( "HuntAnimal_CervusElaphus", "hunter", "none", 25, false);
		addAction( "HuntAnimal_CervusElaphusF", "hunter", "none", 25, false);
		addAction( "HuntAnimal_GallusGallusDomesticus", "hunter", "none", 25, false);
		addAction( "HuntAnimal_GallusGallusDomesticusF", "hunter", "none", 25, false);
		addAction( "HuntAnimal_OvisAries", "hunter", "none", 25, false);
		addAction( "HuntAnimal_OvisAriesF", "hunter", "none", 25, false);
		addAction( "HuntAnimal_SusDomesticus", "hunter", "none", 25, false);
		addAction( "HuntAnimal_SusScrofa", "hunter", "none", 25, false);
		addAction( "HuntAnimal_LepusEuropaeus", "hunter", "none", 25, false);
		addAction( "HuntAnimal_UrsusArctos", "hunter", "none", 200, false);
		
		AggressorActions.Insert( new ref habAggressorAction("ShotFired", 75));
		AggressorActions.Insert( new ref habAggressorAction("HitZombie", -75));
		AggressorActions.Insert( new ref habAggressorAction("HitAnimal", -75));
		AggressorActions.Insert( new ref habAggressorAction("HitPlayer", 350));
		AggressorActions.Insert( new ref habAggressorAction("KillPlayer", 1500));
		AggressorActions.Insert( new ref habAggressorAction("HitGuard", 150));
		AggressorActions.Insert( new ref habAggressorAction("KillGuard", 750));
	}
	
	void DoV5Upgrade(){
		ConfigVersion = "5";
		Save();
	}
	
	void DoV6Upgrade(){
		ConfigVersion = "6";
		AggressorActions.Insert( new ref habAggressorAction("ShotFired", 75));
		AggressorActions.Insert( new ref habAggressorAction("HitZombie", -75));
		AggressorActions.Insert( new ref habAggressorAction("HitPlayer", 350));
		AggressorActions.Insert( new ref habAggressorAction("KillPlayer", 1500));
		AggressorActions.Insert( new ref habAggressorAction("HitGuard", 150));
		AggressorActions.Insert( new ref habAggressorAction("KillGuard", 750));
		addAction( "MedicSplintPlayer", "hero", "medic", 100);
		
		Save();
	}
	
	void DoV7Upgrade(){
		ConfigVersion = "7";
		AggressorActions.Insert( new ref habAggressorAction("HitAnimal", -75));
		Save();
	}
	
};
