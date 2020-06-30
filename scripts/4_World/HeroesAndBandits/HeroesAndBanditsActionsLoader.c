static string HeroesAndBanditsActionsPATH = HeroesAndBanditsDirectory + "\\actions.json";

class HeroesAndBanditsConfigActions
{ 
	//Default Values
	string ConfigVersion = "4";
	
	int NotificationMessageTime = 10;
	
	ref array<int> NotificationColor = {200, 0, 200, 200};
	
	ref array< ref habAction > Actions = new ref array< ref habAction >;
	
	void Load(){
		habCheckUpgradeToConfigV4();
		if (FileExist(HeroesAndBanditsActionsPATH)) //If config exist load File
		{
	        	JsonFileLoader<HeroesAndBanditsConfigActions>.JsonLoadFile(HeroesAndBanditsActionsPATH, this);
		}else{ //File does not exist create file
			createDefaults();
			habPrint("Creating Default Actions Config", "Always");	
			JsonFileLoader<HeroesAndBanditsConfigActions>.JsonSaveFile(HeroesAndBanditsActionsPATH, this);
		}
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
		habPrint("Action: " +actionName+ " not found", "Exception");	
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
		habPrint("Action: " +actionName+ " not found", "Exception");	
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
		habPrint("Action: " + actionName + " not found", "Exception");	
		return new habAction("Null", "Null", "Null", 0, false);
	}
	
	//Helper function for adding Actions
	void addAction(string actionName, string affinity, string secondaryAffinity, float points, bool notifyPlayer = true){
		habAction tempAction = new ref habAction(actionName, affinity, secondaryAffinity, points, notifyPlayer);
		Actions.Insert(tempAction);
		habPrint("Action Added: " + actionName + " There are now " + Actions.Count() + " Actions", "Verbose");	
	}
	
	//Returns the Notification color in an int value
	int getNotificationColor(){
		return ARGB(NotificationColor[0], NotificationColor[1], NotificationColor[2], NotificationColor[3]);
	}
	
	void createDefaults(){
		addAction( "ZombieKill", "hero", "none", 5);
		addAction( "heroSucide", "bandit", "none",100);
		addAction( "banditSucide", "hero", "none",100);
		addAction( "bambiSucide", "bambi", "none",0, false);
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
		addAction( "BandagePlayer", "hero", "medic", 50);
		addAction( "GiveBloodPlayer", "hero", "medic",25);
		addAction( "GiveSalinePlayer", "hero","medic", 25);
		addAction( "GivePlayerCPR", "hero", "medic" ,75);
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
	
}



//Class for holding Actions
class habAction
{
	string Name;
	string Affinity; //bandit / hero / bambi
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
}
