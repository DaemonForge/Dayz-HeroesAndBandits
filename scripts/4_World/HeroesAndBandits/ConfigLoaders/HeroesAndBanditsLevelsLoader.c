static string HeroesAndBanditsLevelsPATH = HeroesAndBanditsDirectory + "\\levels.json";

class HeroesAndBanditsConfigLevels
{ 
	//Default Values
	string ConfigVersion = "5";
	ref array< ref habLevel > Levels = new ref array< ref habLevel >;
	ref habLevel DefaultLevel = new ref habLevel("Bambi", "bambi", "HeroesAndBandits/gui/images/BambiNotification.paa", -1, 1000);
	ref array< ref habAffinity > Affinities = new ref array< ref habAffinity >;
	ref habAffinity DefaultAffinity = new ref habAffinity("bambi", "#HAB_BAMBI");
	
	bool ShowLevelIcon = true;
	int  LevelIconLocation = 2;
	
	bool NotifyLevelChange = true;
	
		
	void Load(){
		habCheckUpgradeToConfigV4();
		if (FileExist(HeroesAndBanditsLevelsPATH)) //If config exist load File
		{
	        	JsonFileLoader<HeroesAndBanditsConfigLevels>.JsonLoadFile(HeroesAndBanditsLevelsPATH, this);
				if (ConfigVersion == "4"){
					doV5Upgrade();
				}
		}else{ //File does not exist create file
			createDefaults();
			habPrint("Creating Default Actions Config", "Always");	
			ReSave();
		}
	}
	
	void ReSave(){
		JsonFileLoader<HeroesAndBanditsConfigLevels>.JsonSaveFile(HeroesAndBanditsLevelsPATH, this);
	}
	
	//Returns the level based on points value
	habLevel getLevel(string affinity, float points){
		for ( int i =0; i < Levels.Count(); i++ )
		{
			if( Levels.Get(i).Affinity ==  affinity)
			{
				float minPoints = Levels.Get(i).MinPoints;
				float maxPoints = Levels.Get(i).MaxPoints;
				if ( minPoints != -1 && maxPoints != -1 && points >= minPoints && points <= maxPoints){
					return Levels.Get(i);
				}else if (minPoints == -1 && maxPoints != -1 && points <= maxPoints){
					return Levels.Get(i);
				}else if (minPoints != -1 && maxPoints == -1 && points >= minPoints){
					return Levels.Get(i);
				}
			}
		}
		return DefaultLevel;
	}
	
	habLevel getLevelByIndex(int index){
			if ( index != -1 ){
				return Levels.Get(index);
			} 
		return DefaultLevel;
	}
	
	int getLevelIndex(string affinity, float points){
		for ( int i =0; i < Levels.Count(); i++ )
		{
			if ( Levels.Get(i).Affinity ==  affinity)
			{
				float minPoints = Levels.Get(i).MinPoints;
				float maxPoints = Levels.Get(i).MaxPoints;
				if ( minPoints != -1 && maxPoints != -1 && points >= minPoints && points <= maxPoints){
					return i;
				}else if (minPoints == -1 && maxPoints != -1 && points <= maxPoints){
					return i;
				}else if (minPoints != -1 && maxPoints == -1 && points >= minPoints){
					return i;
				}
			}
		}
		return -1; // Default Level 
	}
	
	
	bool doesAffinityExsit(string name){
		for ( int i =0; i < Affinities.Count(); i++ )
		{
			if ( Affinities.Get(i).Name ==  name)
			{
				return true;
			}
		}
		return false;
	}
	
	habAffinity getAffinity(string name){
		for ( int i =0; i < Affinities.Count(); i++ )
		{
			if ( Affinities.Get(i).Name ==  name)
			{
				return Affinities.Get(i);
			}
		}
		return DefaultAffinity;
	}
	
	int getAffinityIndex(string name){
		for ( int i =0; i < Affinities.Count(); i++ )
		{
			if ( Affinities.Get(i).Name ==  name)
			{
				return i;
			}
		}
		return -1; // Default Affinity 
	}
		
	//Helper function for adding levels
	void addLevel(string name, string affinity, string levelImage, float minHumanity, float maxHumanity){
		habLevel tempLevel = new ref habLevel(name, affinity, levelImage, minHumanity, maxHumanity);
		Levels.Insert(tempLevel);
		habPrint("Level Added: " + name + " There are now " + Levels.Count() + " Levels", "Verbose");	
	}	
	
	//Helper function for adding levels
	void addAffinity(string name, string displayName){
		habAffinity tempAffinity = new ref habAffinity(name, displayName);
		Affinities.Insert(tempAffinity);
		habPrint("Affinity Added: " + name + " There are now " + Affinities.Count() + " Affinities", "Verbose");	
	}
	
	void createDefaults(){
		addLevel( "Hero Lv1", "hero", "HeroesAndBandits/gui/images/HeroNotificationlv1.paa", 1001, 4000);
		addLevel( "Hero Lv2", "hero", "HeroesAndBandits/gui/images/HeroNotificationlv2.paa", 4001, 12000);
		addLevel( "Hero Lv3", "hero", "HeroesAndBandits/gui/images/HeroNotificationlv3.paa", 12001, 20000);
		addLevel( "Hero Lv4", "hero", "HeroesAndBandits/gui/images/HeroNotificationlv4.paa", 20001, 50000);
		addLevel( "Hero Lv5", "hero", "HeroesAndBandits/gui/images/HeroNotificationlv5.paa", 50001, -1);
		addLevel( "Bandit Lv1", "bandit", "HeroesAndBandits/gui/images/BanditNotificationlv1.paa", 1001, 4000);
		addLevel( "Bandit Lv2", "bandit", "HeroesAndBandits/gui/images/BanditNotificationlv2.paa", 4001, 12000);
		addLevel( "Bandit Lv3", "bandit", "HeroesAndBandits/gui/images/BanditNotificationlv3.paa", 12001, 20000);
		addLevel( "Bandit Lv4", "bandit", "HeroesAndBandits/gui/images/BanditNotificationlv4.paa", 20001, 50000);
		addLevel( "Bandit Lv5", "bandit", "HeroesAndBandits/gui/images/BanditNotificationlv5.paa", 50001, -1);

		addLevel( "Medic Lv1", "medic", "HeroesAndBandits/gui/images/Mediclv1.paa", 1001, 4000);
		addLevel( "Medic Lv2", "medic", "HeroesAndBandits/gui/images/Mediclv2.paa", 4001, 12000);
		addLevel( "Medic Lv3", "medic", "HeroesAndBandits/gui/images/Mediclv3.paa", 12001, 20000);
		addLevel( "Medic Lv4", "medic", "HeroesAndBandits/gui/images/Mediclv4.paa", 20001, 50000);
		addLevel( "Medic Lv5", "medic", "HeroesAndBandits/gui/images/Mediclv5.paa", 50001, -1);
		addLevel( "Hunter Lv1", "hunter", "HeroesAndBandits/gui/images/Hunterlv1.paa", 1001, 4000);
		addLevel( "Hunter Lv2", "hunter", "HeroesAndBandits/gui/images/Hunterlv2.paa", 4001, 12000);
		addLevel( "Hunter Lv3", "hunter", "HeroesAndBandits/gui/images/Hunterlv3.paa", 12001, 20000);
		addLevel( "Hunter Lv4", "hunter", "HeroesAndBandits/gui/images/Hunterlv4.paa", 20001, 50000);
		addLevel( "Hunter Lv5", "hunter", "HeroesAndBandits/gui/images/Hunterlv5.paa", 50001, -1);
		
		addAffinity("hero", "#HAB_HERO");
		addAffinity("bandit", "#HAB_BANDIT");
		addAffinity("medic", "#HAB_MEDIC");
		addAffinity("hunter", "#HAB_HUNTER");
		TStringArray banditOnlyItems = {"BandanaMask_RedPattern", "BandanaMask_BlackPattern","BandanaMask_CamoPattern","BandanaMask_GreenPattern", "BandanaMask_PolkaPattern","Bandana_Blue", "Bandana_Pink", "Bandana_Yellow","Bandana_RedPattern","Bandana_BlackPattern","Bandana_CamoPattern","Bandana_GreenPattern","Bandana_PolkaPattern"};
		getAffinity("hero").addItemBlackList(-1, -1, "all", banditOnlyItems );
		DefaultAffinity.addItemBlackList(-1, -1, "all", banditOnlyItems);
	}
	
	void doV5Upgrade(){
		ConfigVersion = "5";
		TStringArray banditOnlyItems = {"BandanaMask_RedPattern", "BandanaMask_BlackPattern","BandanaMask_CamoPattern","BandanaMask_GreenPattern", "BandanaMask_PolkaPattern","Bandana_Blue", "Bandana_Pink", "Bandana_Yellow","Bandana_RedPattern","Bandana_BlackPattern","Bandana_CamoPattern","Bandana_GreenPattern","Bandana_PolkaPattern"};
		getAffinity("hero").addItemBlackList(-1, -1, "all", banditOnlyItems );
		DefaultAffinity.addItemBlackList(-1, -1, "all", banditOnlyItems);
		ReSave();
	}
}



//Class for holding levels
class habLevel
{
	string Name;
	string Affinity; //bandit / hero / bambi
	string LevelImage;
	float MinPoints;
	float MaxPoints;

	void habLevel(string name, string affinity, string levelImage, float minPoints, float maxPoints) 
	{
		Name = name;
		Affinity = affinity;
		LevelImage = levelImage;
		MinPoints = minPoints;
		MaxPoints = maxPoints;
	}
}

//Class for holding Actions
class habAffinity
{
	string Name;
	string DisplayName;
	ref array< ref habItemList > ItemBlackList = new ref array< ref habItemList >;

	void habAffinity(string name, string displayName) 
	{
		Name = name;
		DisplayName = displayName;
	}
	
	bool checkItem(float points, string itemType, string location){
		bool allowed = true;
		if (!ItemBlackList){ return true; }
		if (ItemBlackList.Count() == 0){ return true; }
		for (int i = 0; i < ItemBlackList.Count(); i++){
			float minPoints = ItemBlackList.Get(i).MinPoints;
			float maxPoints = ItemBlackList.Get(i).MaxPoints;
			if ( minPoints != -1 && maxPoints != -1 && points >= minPoints && points <= maxPoints){
				if ( !ItemBlackList.Get(i).checkItem(itemType, location) ){
					return false;
				}
			}else if (minPoints == -1 && maxPoints != -1 && points <= maxPoints){
				if ( !ItemBlackList.Get(i).checkItem(itemType, location) ){
					return false;
				}
			}else if (minPoints != -1 && maxPoints == -1 && points >= minPoints){
				if ( !ItemBlackList.Get(i).checkItem(itemType, location) ){
					return false;
				}
			}else if ( minPoints == -1 && maxPoints == -1 ){
				if ( !ItemBlackList.Get(i).checkItem(itemType, location) ){
					return false;
				}
			}
		}
		return true;
	}
	
	void addItemBlackList( float minPoints, float maxPoints, string type, ref TStringArray items)
	{
		ItemBlackList.Insert(new ref habItemList( minPoints, maxPoints, type));
		int index = ItemBlackList.Count() - 1;
		ItemBlackList.Get(index).Items = items;
	}
}


class habItemList
{
	float MinPoints;
	float MaxPoints;
	string Location; // attach / inventory / inhands / all 
	ref TStringArray Items = new ref TStringArray;
	
	void habItemList( float minPoints, float maxPoints, string location )
	{
		MinPoints = minPoints;
		MaxPoints = maxPoints;
		Location = location;
	}
	
	bool checkItem( string itemType , string location){
		bool check = true;
		bool doCheck = false;
		if ( Location == "all" ){
			doCheck = true;
		} else if ( Location == location){
			doCheck = true;
		} else if (location == "inventory" && Location == "attach"){
			doCheck = true;
		}
		
		if (doCheck){
			int index = Items.Find(itemType);
				habPrint("Checking for " + itemType + " Found " + index, "Debug");
				if (index != -1){
						check = !check;
				}
		}
		return check;
	}	
}
