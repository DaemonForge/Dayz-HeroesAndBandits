class HeroesAndBanditsConfigActions
{ 
	//Default Values
	string ConfigVersion = "5";
	
	int NotificationMessageTime = 10;
	
	ref array<int> NotificationColor = {200, 0, 200, 200};
	
	ref array< ref habAction > Actions = new ref array< ref habAction >;
	
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
				}else{ //File does not exist create file
					createDefaults();
					Print("Creating Default Actions Config");	
					Save();
				}
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
		for ( int i =0; i < Actions.Count(); i++ )
		{
			tempActionName = Actions.Get(i).Name;
			tempActionName.ToLower();
			actionName.ToLower();
			if ( tempActionName == actionName ){
					return Actions.Get(i);
			}
		}
		Print("Action: " + actionName + " not found");	
		return new habAction("Null", "Null", "Null", 0, false);
	}
	
	//Helper function for adding Actions
	void addAction(string actionName, string affinity, string secondaryAffinity, float points, bool notifyPlayer = true){
		habAction tempAction = new ref habAction(actionName, affinity, secondaryAffinity, points, notifyPlayer);
		Actions.Insert(tempAction);
		Print("Action Added: " + actionName + " There are now " + Actions.Count() + " Actions");	
	}
	
	//Returns the Notification color in an int value
	int getNotificationColor(){
		return ARGB(NotificationColor[0], NotificationColor[1], NotificationColor[2], NotificationColor[3]);
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
		#endif
		
		addAction( "MedicBandagePlayer", "hero", "medic", 50);
		addAction( "MedicGiveBlood", "hero", "medic",25);
		addAction( "MedicGiveSaline", "hero","medic", 25);
		addAction( "MedicGiveCPR", "hero", "medic" ,75);
		addAction( "MedicFeedTetracycline", "hero", "medic", 15);
		addAction( "MedicFeedPainkiller", "hero", "medic", 15);
		addAction( "MedicFeedCharcoal", "hero", "medic", 15);
		addAction( "MedicFeedVitamin", "hero", "medic", 10);
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
	
	}
	
	void DoV5Upgrade(){
		ConfigVersion = "5";
		Save();
	}
};



//Class for holding Actions
class habAction
{
	string Name;
	string Affinity; //bandit / hero / none
	string SecondaryAffinity = "none";
	float Points;
	bool NotifiyPlayer;

	void habAction(string actionName, string affinity, string secondaryAffinity, float points, bool notifiyPlayer = true) 
	{
		Name = actionName;
		Affinity = affinity;
		SecondaryAffinity =  secondaryAffinity;
		Points = points;
		NotifiyPlayer = notifiyPlayer;
	}
};
