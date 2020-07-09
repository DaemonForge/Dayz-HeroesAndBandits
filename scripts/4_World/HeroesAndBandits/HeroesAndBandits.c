ref HeroesAndBandits m_HeroesAndBandits;
ref HeroesAndBanditsConfig m_HeroesAndBanditsConfig;
ref HeroesAndBanditsSettings m_HeroesAndBanditsSettings;
ref HeroesAndBanditsConfigZones m_HeroesAndBanditsConfigZones;
ref HeroesAndBanditsConfigActions m_HeroesAndBanditsConfigActions;
ref HeroesAndBanditsConfigLevels m_HeroesAndBanditsConfigLevels;

ref HeroesAndBanditsPlayer 			g_HeroesAndBanditsPlayer;
ref HeroesAndBanditsSettings 		g_HeroesAndBanditsSettings;
ref HeroesAndBanditsConfigLevels 	g_HeroesAndBanditsConfigLevels;
ref HeroesAndBanditsConfigActions 	g_HeroesAndBanditsConfigActions;

ref NotificationSystem m_HeroesAndBanditsNotificationSystem = new NotificationSystem();

class HeroesAndBandits
{
	ref array<ref HeroesAndBanditsPlayer> HeroesAndBanditsPlayers = new ref array< ref HeroesAndBanditsPlayer >;
	
	
	ref array< ref HeroesAndBanditsZone > Zones = new ref array< ref HeroesAndBanditsZone >;
	
	void HeroesAndBandits()
	{
	}
	
	void Init(){
		if (GetHeroesAndBanditsZones().Zones) //Check if Zones are defined before loading
		{
			for ( int i = 0; i < GetHeroesAndBanditsZones().Zones.Count(); i++ )
			{
		    	habPrint("Loading Zone " + GetHeroesAndBanditsZones().Zones.Get(i).Name , "Verbose");
				string name = GetHeroesAndBanditsZones().Zones.Get(i).Name;
				int x = GetHeroesAndBanditsZones().Zones.Get(i).X;
				int z = GetHeroesAndBanditsZones().Zones.Get(i).Z;
				Zones.Insert(new ref HeroesAndBanditsZone(name, x, z));
				Zones.Get(i).Init(GetHeroesAndBanditsZones().Zones.Get(i), i + 1);
			}
		}
	}
	
	void SaveAllPlayers()
	{
		if ( HeroesAndBanditsPlayers ){
			for ( int i = 0; i < HeroesAndBanditsPlayers.Count(); i++ )
			{
				HeroesAndBanditsPlayers.Get(i).saveData();
			}
			habPrint("Saved all Player Data", "Verbose");
		} else {
			habPrint("No Players Loaded to Save", "Verbose");
		}
	}
	
	void OnPlayerConnect(PlayerIdentity playerIdent)
	{
		if (IsPlayerLoaded(playerIdent.GetPlainId()))
		{
			habPrint("Player " + playerIdent.GetPlainId()+ " connected and already loaded", "Verbose");	
			return;
		}else{
			loadPlayer(playerIdent.GetPlainId());
			
		}
	}
	
	void loadPlayer(string playerID){
		HeroesAndBanditsPlayers.Insert(new ref HeroesAndBanditsPlayer(playerID));
	}
	
	void OnPlayerDisconnect(PlayerBase player)
	{
		if ( player.GetIdentity() ) {
			habPrint("Player " + player.GetIdentity().GetPlainId()+ " Disconnecting saving player data", "Verbose");	
			GetPlayer(player.GetIdentity().GetPlainId()).saveData();
		}
	}
	
	void NewPlayerAction(string playerID, string action)
	{
		for ( int i = 0; i < HeroesAndBanditsPlayers.Count(); i++ )
		{
			HeroesAndBanditsPlayer p = HeroesAndBanditsPlayers.Get(i);
			if ( p.PlayerID ==  playerID)
			{
				bool didLevelUp = p.NewAction(action);
				habAction tempAction = GetHeroesAndBanditsActions().getAction(action);
				if (tempAction.NotifiyPlayer){
					string prefix = "";
					string postix = "";
					float actionHumanity = tempAction.Points;
					if (GetHeroesAndBanditsSettings().Mode == 0 && tempAction.Affinity == "hero"){
						prefix = "+";
						postix = " #HAB_HUMANITY";
					} else if (GetHeroesAndBanditsSettings().Mode == 0 && tempAction.Affinity == "bandit") {
						prefix = "-";
						postix = " #HAB_HUMANITY";
					} else if (GetHeroesAndBanditsSettings().Mode == 1){
						prefix = "+";
						postix = " " + GetHeroesAndBanditsLevels().getAffinity(tempAction.Affinity).DisplayName;
					}
					NotifyPlayer(playerID, p.getLevel().LevelImage, prefix + actionHumanity + postix );	
				}
				PlayerBase player = PlayerBase.Cast(habGetPlayerBaseByID(playerID));
				float affinityPoints = 0;
				if (GetHeroesAndBanditsSettings().Mode == 0){
					affinityPoints = p.getHumanity();
					if (affinityPoints < 0 ){
						affinityPoints = 0 - affinityPoints;
					}
				} else if ( GetHeroesAndBanditsSettings().Mode == 1 ){
					affinityPoints = p.getAffinityPoints(p.getAffinityName());
				}
				if (didLevelUp)
				{
					player.habLevelChange(p.getAffinityIndex(), affinityPoints, p.getLevelIndex());
					if (player){
						GetRPCManager().SendRPC("HaB", "RPCUpdateHABIcon", new Param2< string, string >(playerID, p.getLevel().LevelImage), false, player.GetIdentity());
					}
				} else {
					player.habCurrentAffinityPointUpdate(affinityPoints);
				}
				if (didLevelUp && GetHeroesAndBanditsLevels().NotifyLevelChange)
				{
					NotifyPlayer(playerID, p.getLevel().LevelImage, "#HAB_HUMANITY_LEVELUP_PRE " + p.getLevel().Name, " #HAB_HUMANITY_LEVELUP_HEADING" );
				}
				return;
			}
		}
	}
	
	void NotifyPlayer(string playerID, string image ,string message, string heading = "#HAB_HUMANITY_CHANGEHEADING")
	{
		PlayerBase player = PlayerBase.Cast(habGetPlayerBaseByID(playerID));
		if (player){
			m_HeroesAndBanditsNotificationSystem.CreateNotification(new ref StringLocaliser(heading), new ref StringLocaliser(message), image, GetHeroesAndBanditsActions().getNotificationColor(), GetHeroesAndBanditsActions().NotificationMessageTime, player.GetIdentity());
			habPrint("Notify Player: " + playerID +" Message: "+ message + " Image: " + image, "Verbose");	
		}
	}
	
	void NotifyKillFeed(string image ,string message, string heading = "#HAB_KILLFEED_HEADING")
	{
		m_HeroesAndBanditsNotificationSystem.CreateNotification(new ref StringLocaliser(heading), new ref StringLocaliser(message), image, GetHeroesAndBanditsSettings().getKillFeedMessageColor(), GetHeroesAndBanditsSettings().NotificationMessageTime);
		habPrint("Kill Feed Message: "+ message + " Image: " + image, "Verbose");	
	}
	
	void WarnPlayer( string header, string message, string playerID)
	{
		
		PlayerBase player = PlayerBase.Cast(habGetPlayerBaseByID(playerID));
		if (player){
			m_HeroesAndBanditsNotificationSystem.CreateNotification(new ref StringLocaliser(header), new ref StringLocaliser(message), GetHeroesAndBanditsZones().WarningMessageImagePath, GetHeroesAndBanditsZones().getWarningMessageColor(), GetHeroesAndBanditsZones().NotificationMessageTime, player.GetIdentity());
			habPrint("Issued Warning '"+ message + "' To Player: " + playerID, "Verbose");
		}
	}
		
	void WelcomePlayer( string zoneName, string message, string welcomeImage, string playerID, int welcomeColor)
	{
			
		PlayerBase player = PlayerBase.Cast(habGetPlayerBaseByID(playerID));
		if (player){
			m_HeroesAndBanditsNotificationSystem.CreateNotification(new ref StringLocaliser(zoneName), new ref StringLocaliser(message), welcomeImage, welcomeColor, GetHeroesAndBanditsZones().NotificationMessageTime, player.GetIdentity());
			habPrint("Welcome Message: '"+ message + "' To Player: " + playerID, "Verbose");
		}
	}
	
	void TriggerKillFeed(string sourcePlayerID, string targetPlayerID, string weaponName){
		habPrint("Kill Feed Player: " + sourcePlayerID +" killed " + targetPlayerID + " with " + weaponName , "Debug");
		if (GetHeroesAndBanditsSettings().KillFeed){
			PlayerBase sourcePlayer = PlayerBase.Cast(habGetPlayerBaseByID(sourcePlayerID));
			PlayerBase targetPlayer = PlayerBase.Cast(habGetPlayerBaseByID(targetPlayerID));
			if ( sourcePlayer && targetPlayer ){
				string levelImage = GetPlayerLevel(sourcePlayerID).LevelImage;
				string levelName = GetPlayerLevel(sourcePlayerID).Name;
				float distance = Math.Round(vector.Distance(sourcePlayer.GetPosition(), targetPlayer.GetPosition()));
				string message = "#HAB_KILLFEED_PRE " + levelName + " " + sourcePlayer.GetIdentity().GetName() + " #HAB_KILLFEED_KILLED " + targetPlayer.GetIdentity().GetName() + " #HAB_KILLFEED_WITH " + weaponName + " #HAB_KILLFEED_AT " + distance + " #HAB_KILLFEED_METERS" ;
				NotifyKillFeed(levelImage, message);
			}
		}
	}
	
	void TriggerSucideFeed(string sourcePlayerID){
		habPrint("Sucide Kill Feed Player: " + sourcePlayerID, "Debug");
		if (GetHeroesAndBanditsSettings().SucideFeed){
			PlayerBase sourcePlayer = PlayerBase.Cast(habGetPlayerBaseByID(sourcePlayerID));
			if (sourcePlayer) {
				string levelImage = GetPlayerLevel(sourcePlayerID).LevelImage;
				string message = sourcePlayer.GetIdentity().GetName() + " #HAB_KILLFEED_SUCIDEPOST" ;
				NotifyKillFeed(levelImage, message, "#HAB_KILLFEED_SUCIDEHEADING");
			}
		}
	}
	
	HeroesAndBanditsPlayer GetPlayer( string pID )
	{
		if (!IsPlayerLoaded(pID))
		{
			loadPlayer(pID);
		}
		for ( int i = 0; i < HeroesAndBanditsPlayers.Count(); i++ )
		{
			if ( HeroesAndBanditsPlayers.Get(i).PlayerID ==  pID)
			{
				return HeroesAndBanditsPlayers.Get(i);
			}
		}
	    habPrint("Failed to get HeroesAndBanditsPlayer for Player " + pID , "Verbose");
		return null;
	}
	
	bool IsPlayerLoaded( string pID )
	{
		for ( int i = 0; i < HeroesAndBanditsPlayers.Count(); i++ )
		{
			if ( HeroesAndBanditsPlayers.Get(i).PlayerID ==  pID)
			{
				return true;
			}
		}
		return false;
	}
	
	float GetPlayerHumanity( string pID )
	{
		if (!IsPlayerLoaded(pID))
		{
			loadPlayer(pID);
		}
		for ( int i = 0; i < HeroesAndBanditsPlayers.Count(); i++ )
		{
			HeroesAndBanditsPlayer p = HeroesAndBanditsPlayers.Get(i);
			if ( p.PlayerID ==  pID)
			{
				//habPrint("Getting Humanity for Player " + p.PlayerID + " Humanity is " + p.Humanity, "Debug");	
				return p.getHumanity();
			}
		}
	    habPrint("Failed to get Humanity for Player " + pID , "Verbose");
		return 0;
	}
	
	int GetPlayerStat( string pID, string stat )
	{
		if (!IsPlayerLoaded(pID))
		{
			loadPlayer(pID);
		}
		for ( int i = 0; i < HeroesAndBanditsPlayers.Count(); i++ )
		{
			HeroesAndBanditsPlayer p = HeroesAndBanditsPlayers.Get(i);
			if ( p.PlayerID ==  pID)
			{
				habPrint("Getting Stat " + stat + " for Player " + p.PlayerID + " total is " + p.getStat(stat), "Debug");	
				return p.getStat(stat);
			}
		}
	    habPrint("Failed to get Stat " + stat + " for Player " + pID , "Exception");	
		return 0;
	}
	
	string GetPlayerAffinity( string pID )
	{
		if (!IsPlayerLoaded(pID))
		{
			loadPlayer(pID);
		}
		for ( int i = 0; i < HeroesAndBanditsPlayers.Count(); i++ )
		{
			HeroesAndBanditsPlayer p = HeroesAndBanditsPlayers.Get(i);
			if ( p.PlayerID ==  pID)
			{
				habPrint("Geting Affinity Player " + p.PlayerID + " Affinity " + p.getAffinity().Name, "Debug");	
				return p.getAffinity().Name;
				
			}
		}
	    habPrint("Failed to get Affinity for Player " + pID , "Exception");	
		return GetHeroesAndBanditsLevels().DefaultAffinity.Name;
	}
	
	string GetPlayerHeroOrBandit( string pID )
	{
		if (!IsPlayerLoaded(pID))
		{
			loadPlayer(pID);
		}
		for ( int i = 0; i < HeroesAndBanditsPlayers.Count(); i++ )
		{
			HeroesAndBanditsPlayer p = HeroesAndBanditsPlayers.Get(i);
			if ( p.PlayerID ==  pID)
			{
				habPrint("Geting Hero or Bandit for Player " + p.PlayerID + " Affinity " + p.getHeroOrBandit(), "Debug");	
				return p.getHeroOrBandit();
				
			}
		}
	    habPrint("Failed to get Affinity for Player " + pID , "Verbose");	
		return GetHeroesAndBanditsLevels().DefaultLevel.Affinity;
	}
	
	float GetPlayerAffinityPoints( string pID, string affinityName )
	{
		if (!IsPlayerLoaded(pID))
		{
			loadPlayer(pID);
		}
		for ( int i = 0; i < HeroesAndBanditsPlayers.Count(); i++ )
		{
			HeroesAndBanditsPlayer p = HeroesAndBanditsPlayers.Get(i);
			if ( p.PlayerID ==  pID)
			{
				return p.getAffinityPoints(affinityName);
			}
		}
	    habPrint("Failed to get points for Affinity " + affinityName + " for Player " + pID , "Exception");	
		return 0;
	}
	
	habLevel GetPlayerLevel( string pID )
	{
		if (!IsPlayerLoaded(pID))
		{
			loadPlayer(pID);
		}
		for ( int i = 0; i < HeroesAndBanditsPlayers.Count(); i++ )
		{
			HeroesAndBanditsPlayer p = HeroesAndBanditsPlayers.Get(i);
			if ( p.PlayerID ==  pID)
			{
				habPrint("Geting Level Player " + p.PlayerID + " Affinity " + p.getLevel().Name, "Debug");	
				return p.getLevel();
				
			}
		}
	    habPrint("Failed to get Level for Player " + pID , "Verbose");
		return GetHeroesAndBanditsLevels().DefaultLevel;
	}
	
	string GetPlayerLevelName( string pID )
	{
		if (!IsPlayerLoaded(pID))
		{
			loadPlayer(pID);
		}
		for ( int i = 0; i < HeroesAndBanditsPlayers.Count(); i++ )
		{
			HeroesAndBanditsPlayer p = HeroesAndBanditsPlayers.Get(i);
			if ( p.PlayerID ==  pID)
			{
				habPrint("Geting Level Player " + p.PlayerID + " Level " + p.getLevel().Name, "Debug");	
				return p.getLevel().Name;
				
			}
		}
	    habPrint("Failed to get Level Name for Player " + pID , "Verbose");
		return GetHeroesAndBanditsLevels().DefaultLevel.Name;
	}
	
	string getPlayerSkin(string playerID, int skinIndex = -1){
		if (GetPlayerAffinity(playerID) == "hero")
		{
			if (skinIndex == -1 || GetHeroesAndBanditsSettings().HeroSkins.Count() > skinIndex){
				return GetHeroesAndBanditsSettings().HeroSkins.GetRandomElement();
			} else {
				return GetHeroesAndBanditsSettings().HeroSkins.Get(skinIndex);
			}
		} else if (GetPlayerAffinity(playerID) == "bandit")
		{
			if (skinIndex == -1 || GetHeroesAndBanditsSettings().BanditSkins.Count() > skinIndex){
				return GetHeroesAndBanditsSettings().BanditSkins.GetRandomElement();
			} else {
				return GetHeroesAndBanditsSettings().BanditSkins.Get(skinIndex);
			}
		}
		if (skinIndex == -1 || GetHeroesAndBanditsSettings().BambiSkins.Count() > skinIndex){
			return GetHeroesAndBanditsSettings().BambiSkins.GetRandomElement();
		} else {
			return GetHeroesAndBanditsSettings().BambiSkins.Get(skinIndex);
		}
	}
	
	
	void CheckPlayersEnterZones(){
		if (!Zones) //if no zones Defined exit
		{
			return;
		}
		//habPrint("Checking if Players are in Zones", "Debug");			
		ref array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		for (int j = 0; j < players.Count(); j++)
		{
			PlayerBase player = PlayerBase.Cast(players.Get(j));
			if ( player ){	
				if (player.IsAlive())
				{				
					for (int k = 0; k < Zones.Count(); k++)
					{	
						Zones.Get(k).CheckPlayer(player);
					}
				}
			}
		}
	}
	
	void updatePlayerTotals()
	{
		habPrint("Updating Player Totals", "Verbose");
		ref array<string> playerDataBaseFiles = new array<string>;
		ref array<string> playerList = new array<string>;
		playerDataBaseFiles = habFindFilesInDirectory(HeroesAndBanditsPlayerDB);
		habPrint("Found " + playerDataBaseFiles.Count() + " Players in database Located: " + HeroesAndBanditsPlayerDB, "Verbose");
		string tempFileName = "";
		string jsonName = "";
		int nameLength = 0;
		if (!playerDataBaseFiles) { return; }
		for ( int i =0; i < playerDataBaseFiles.Count(); i++ )
		{
			tempFileName = playerDataBaseFiles.Get(i);
			nameLength = tempFileName.Length();
			nameLength = nameLength - 5;
			jsonName = tempFileName.Substring(nameLength, 5);
			if ( jsonName == ".json" && tempFileName.Length() ==  22){
				playerList.Insert(tempFileName);
				habPrint("Valid File Found: " + tempFileName + " Loading it now", "Debug");
				tempFileName = tempFileName.Substring(0, 17);
				loadPlayer(tempFileName);
			} else {
				habPrint("Invalid File Found: " + tempFileName + " Name Length: " + tempFileName.Length() + "Json Name: " + jsonName , "Debug");
			}
		}
		for ( int j =0; j < HeroesAndBanditsPlayers.Count(); j++ )
		{
			habPrint("Recalcuating totals for " + HeroesAndBanditsPlayers.Get(j).PlayerID, "Verbose");
			HeroesAndBanditsPlayers.Get(j).recalculateTotals();
		}		
		SaveAllPlayers();
	}
	
}


static ref HeroesAndBandits GetHeroesAndBandits()
{
	if (!m_HeroesAndBandits)
	{
		habPrint("Init", "Always");
		m_HeroesAndBandits = new HeroesAndBandits;
		m_HeroesAndBandits.Init();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLaterByName(m_HeroesAndBandits, "SaveAllPlayers", 300 * 1000, true);
		if (GetHeroesAndBanditsZones().ZoneCheckTimer >= 0.5 && m_HeroesAndBandits.Zones ){
			if (m_HeroesAndBandits.Zones.Count() >= 1){
				habPrint("Creating Zone Check Timer", "Debug");
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLaterByName(m_HeroesAndBandits, "CheckPlayersEnterZones", GetHeroesAndBanditsZones().ZoneCheckTimer * 1000, true);
			} else {
				habPrint("No zones defined", "Debug");
			}
		} else {
			habPrint("Zone Check Time Less than 1 so not creating timer", "Debug");					
		}
	}
	return m_HeroesAndBandits;
}


//Helper function to return Config
static ref HeroesAndBanditsSettings GetHeroesAndBanditsSettings()
{
	if ( GetGame().IsServer() || !GetGame().IsMultiplayer()){
		if (!m_HeroesAndBanditsSettings)
		{
			m_HeroesAndBanditsSettings = new HeroesAndBanditsSettings;
			m_HeroesAndBanditsSettings.Load();
		}
		return m_HeroesAndBanditsSettings;
	} else {
		
		return g_HeroesAndBanditsSettings;
	}
}


//Helper function to return Config
static ref HeroesAndBanditsConfigLevels GetHeroesAndBanditsLevels()
{
	if ( GetGame().IsServer() || !GetGame().IsMultiplayer()){
		if (!m_HeroesAndBanditsConfigLevels)
		{
			m_HeroesAndBanditsConfigLevels = new HeroesAndBanditsConfigLevels;
			m_HeroesAndBanditsConfigLevels.Load();
		}
		return m_HeroesAndBanditsConfigLevels;
	} else {
		return g_HeroesAndBanditsConfigLevels;
	}
}



//Helper function to return Config
static ref HeroesAndBanditsConfigActions GetHeroesAndBanditsActions()
{
	if ( GetGame().IsServer() || !GetGame().IsMultiplayer()){
		if (!m_HeroesAndBanditsConfigActions)
		{
			m_HeroesAndBanditsConfigActions = new HeroesAndBanditsConfigActions;
			m_HeroesAndBanditsConfigActions.Load();
		}
		return m_HeroesAndBanditsConfigActions;
	} else {
		return g_HeroesAndBanditsConfigActions;
	}
}