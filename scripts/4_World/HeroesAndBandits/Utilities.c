
//Return an array of file names for all the files in the specified directory
TStringArray habFindFilesInDirectory(string directory) 
{ 
	
	TStringArray fileList = new TStringArray;
	
	string		fileName;
	int		fileAttr;
	int		flags;
	//Add \ to directory path and add search parameter (*)
	string pathpattern = directory + "\\*";
	
	//Search for files in file directory
	FindFileHandle fileHandler = FindFile(pathpattern, fileName, fileAttr, flags);
	
	bool found = true;
        while ( found ) //while there are files loop through looking for more
        {                
	        if ( fileAttr )
	        {
	        	//If file exsit add to array
	        	fileList.Insert(fileName);
	        }
		found = FindNextFile(fileHandler, fileName, fileAttr);
        }

	return fileList; 
}


//Always Exception Verbose Debug
static void habPrint(string message, string msgType){
	if(!GetGame().IsServer())
	{
		return;
	}
	if (msgType == "Always"){
		Print("[HeroesAndBandits]  " + message);
	}else if (msgType == "Exception" && GetHeroesAndBanditsSettings().ExceptionLogs){
		Print("[HeroesAndBandits] [Exception]  " + message);
	}else if (msgType == "Verbose" && GetHeroesAndBanditsSettings().VerboseLogs){
		Print("[HeroesAndBandits] [Verbose]  " + message);
	}else if (msgType == "Debug" && GetHeroesAndBanditsSettings().DebugLogs){
		Print("[HeroesAndBandits] [Debug]  " + message);
	}
}


PlayerBase habGetPlayerBaseByID( string pID )
{
	PlayerBase p;
	ref array<Man> players = new array<Man>;
	GetGame().GetPlayers(players);
	for ( int i = 0; i < players.Count(); i++ )
	{
		p = PlayerBase.Cast(players.Get(i));
		if ( p.GetIdentity().GetPlainId() ==  pID)
		{
			habPrint("Found Player " + p.GetIdentity().GetName() + " with id " + pID, "Debug");	
			return p;
		}
	}
	habPrint("Failed to GetPlayerBaseByID for Player " + pID , "Verbose");
	return null;
}

PlayerBase habGetPlayerBaseByName( string pName )
{
	PlayerBase p;
	ref array<Man> players = new array<Man>;
	GetGame().GetPlayers(players);
	for ( int i = 0; i < players.Count(); i++ )
	{
		p = PlayerBase.Cast(players.Get(i));
		habPrint("Looking for Player " + pName + " Checking against Name " + p.GetIdentity().GetName() + " Full Name: " + p.GetIdentity().GetFullName(), "Debug");	
		if ( p.GetIdentity().GetName() ==  pName)
		{
			habPrint("Found Player " + p.GetIdentity().GetName() + " with Name " + pName, "Debug");	
			return p;
		}
	}
	habPrint("Failed to GetPlayerBaseByName for Player " + pName , "Verbose");
	return null;
}




/*

This is tempoary code for upgrading the config to v4
*/

bool habCheckUpgradeToConfigV4(){
	bool didUpgrade = false;
 	if (FileExist(HeroesAndBanditsPATH)){//Old File Exists Next Steps
		//If none of the file exsit
		if (!FileExist(HeroesAndBanditsSettingsPATH) && !FileExist(HeroesAndBanditsZonesPATH) && !FileExist(HeroesAndBanditsActionsPATH) && !FileExist(HeroesAndBanditsLevelsPATH)){
			habPrint("Converting Old Config to new config","Always");
			ref HeroesAndBanditsSettings temp_Settings = new ref HeroesAndBanditsSettings;
			ref HeroesAndBanditsConfigZones temp_Zones = new ref HeroesAndBanditsConfigZones;
			ref HeroesAndBanditsConfigActions temp_Actions = new ref HeroesAndBanditsConfigActions;
			ref HeroesAndBanditsConfigLevels temp_Levels = new ref HeroesAndBanditsConfigLevels;
			GetHeroesAndBanditsConfig();
			//Settings 
			temp_Settings.ConfigVersion = "4";
			temp_Settings.Mode = 0;
			temp_Settings.NotificationMessageTime = GetHeroesAndBanditsConfig().NotificationMessageTime;
			temp_Settings.KillFeed = GetHeroesAndBanditsConfig().KillFeed;
			temp_Settings.SucideFeed= GetHeroesAndBanditsConfig().SucideFeed;
			temp_Settings.KillFeedMessageColor = GetHeroesAndBanditsConfig().KillFeedMessageColor;
			temp_Settings.CommandPrefix = GetHeroesAndBanditsConfig().CommandPrefix;
			temp_Settings.AllowStatCommand = GetHeroesAndBanditsConfig().AllowStatCommand;
			temp_Settings.AllowHumanityCommand = GetHeroesAndBanditsConfig().AllowHumanityCommand;
			temp_Settings.AllowGUI = GetHeroesAndBanditsConfig().AllowGUI;
			temp_Settings.GUIHeading = GetHeroesAndBanditsConfig().GUIHeading;
			temp_Settings.HideKillsInGUI = GetHeroesAndBanditsConfig().HideKillsInGUI;
			temp_Settings.ExceptionLogs = GetHeroesAndBanditsConfig().ExceptionLogs;
			temp_Settings.VerboseLogs = GetHeroesAndBanditsConfig().VerboseLogs;
			temp_Settings.DebugLogs = GetHeroesAndBanditsConfig().DebugLogs;
			temp_Settings.BambiSkins = GetHeroesAndBanditsConfig().BambiSkins;
			temp_Settings.HeroSkins = GetHeroesAndBanditsConfig().HeroSkins;
			temp_Settings.BanditSkins = GetHeroesAndBanditsConfig().BanditSkins;
			habPrint("Saving new settings.json", "Always");
			JsonFileLoader<HeroesAndBanditsSettings>.JsonSaveFile(HeroesAndBanditsSettingsPATH, temp_Settings);
			
			//Zones 
			temp_Zones.ConfigVersion = "4";
			temp_Zones.ZoneCheckTimer = GetHeroesAndBanditsConfig().ZoneCheckTimer;
			temp_Zones.WarningMessageColor = GetHeroesAndBanditsConfig().WarningMessageColor;
			temp_Zones.NotificationMessageTime = GetHeroesAndBanditsConfig().NotificationMessageTime;
			temp_Zones.WarningMessageImagePath = GetHeroesAndBanditsConfig().WarningMessageImagePath;
			for ( int i = 0; i < GetHeroesAndBanditsConfig().Zones.Count(); i++)
			{//TODO
				temp_Zones.Zones.Insert(GetHeroesAndBanditsConfig().Zones.Get(i));
			}
			
			habPrint("Saving new zones.json","Always");
			JsonFileLoader<HeroesAndBanditsConfigZones>.JsonSaveFile(HeroesAndBanditsZonesPATH, temp_Zones);
			
			//Actions
			temp_Actions.ConfigVersion = "4";
			temp_Actions.NotificationMessageTime = GetHeroesAndBanditsConfig().NotificationMessageTime;
			temp_Actions.NotificationColor = GetHeroesAndBanditsConfig().NotificationColor;
			for ( int j = 0; j < GetHeroesAndBanditsConfig().Actions.Count(); j++)
			{//TODO
				temp_Actions.Actions.Insert(GetHeroesAndBanditsConfig().Actions.Get(i));
			}
			habPrint("Saving new actions.json","Always");
			JsonFileLoader<HeroesAndBanditsConfigActions>.JsonSaveFile(HeroesAndBanditsActionsPATH, temp_Actions);
			
			//Levels
			temp_Levels.ConfigVersion = "4";
			temp_Levels.Affinities = new ref array< ref habAffinity >;
			temp_Levels.DefaultLevel = GetHeroesAndBanditsConfig().DefaultLevel;
			temp_Levels.ShowLevelIcon = GetHeroesAndBanditsConfig().ShowLevelIcon;
			temp_Levels.LevelIconLocation = GetHeroesAndBanditsConfig().LevelIconLocation;
			temp_Levels.NotifyLevelChange = GetHeroesAndBanditsConfig().NotifyLevelChange;
			for ( int j = 0; j < GetHeroesAndBanditsConfig().Levels.Count(); j++)
			{
				float minPoints = GetHeroesAndBanditsConfig().Levels.Get(j).MinHumanity;
				float maxPoints = GetHeroesAndBanditsConfig().Levels.Get(j).MaxHumanity;
				if (GetHeroesAndBanditsConfig().Levels.Get(j).Affinity == "bandit")
				{
					minPoints = 0 - GetHeroesAndBanditsConfig().Levels.Get(j).MaxHumanity;
					maxPoints = 0 - GetHeroesAndBanditsConfig().Levels.Get(j).MinHumanity;
					if ( GetHeroesAndBanditsConfig().Levels.Get(j).MinHumanity == -1){
						maxPoints = -1;
					}
				}
				temp_Levels.Levels.Insert(GetHeroesAndBanditsConfig().Levels.Get(j).Name, GetHeroesAndBanditsConfig().Levels.Get(j).Affinity, GetHeroesAndBanditsConfig().Levels.Get(j).LevelImage, minPoints, maxPoints);
			}
			
			temp_Levels.addLevel( "Medic Lv1", "medic", "HeroesAndBandits/gui/images/Mediclv1.paa", 1001, 4000);
			temp_Levels.addLevel( "Medic Lv2", "medic", "HeroesAndBandits/gui/images/Mediclv2.paa", 4001, 12000);
			temp_Levels.addLevel( "Medic Lv3", "medic", "HeroesAndBandits/gui/images/Mediclv3.paa", 12001, 20000);
			temp_Levels.addLevel( "Medic Lv4", "medic", "HeroesAndBandits/gui/images/Mediclv4.paa", 20001, 50000);
			temp_Levels.addLevel( "Medic Lv5", "medic", "HeroesAndBandits/gui/images/Mediclv5.paa", 50001, -1);
			temp_Levels.addLevel( "Hunter Lv1", "hunter", "HeroesAndBandits/gui/images/Hunterlv1.paa", 1001, 4000);
			temp_Levels.addLevel( "Hunter Lv2", "hunter", "HeroesAndBandits/gui/images/Hunterlv2.paa", 4001, 12000);
			temp_Levels.addLevel( "Hunter Lv3", "hunter", "HeroesAndBandits/gui/images/Hunterlv3.paa", 12001, 20000);
			temp_Levels.addLevel( "Hunter Lv4", "hunter", "HeroesAndBandits/gui/images/Hunterlv4.paa", 20001, 50000);
			temp_Levels.addLevel( "Hunter Lv5", "hunter", "HeroesAndBandits/gui/images/Hunterlv5.paa", 50001, -1);
			temp_Levels.addAffinity("bambi", "#HAB_BAMBI");
			temp_Levels.addAffinity("hero", "#HAB_HERO");
			temp_Levels.addAffinity("bandit", "#HAB_BANDIT");
			temp_Levels.addAffinity("medic", "#HAB_MEDIC");
			temp_Levels.addAffinity("hunter", "#HAB_HUNTER");
			
			habPrint("Saving new levels.json","Always");
			JsonFileLoader<HeroesAndBanditsConfigLevels>.JsonSaveFile(HeroesAndBanditsLevelsPATH, temp_Levels);
			
			didUpgrade = true;
			
		}
	}
	return didUpgrade;
}