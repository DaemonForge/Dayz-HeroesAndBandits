ref HeroesAndBandits m_HeroesAndBandits;
ref HeroesAndBanditsConfig m_HeroesAndBanditsConfig
ref Timer m_HeroesAndBanditsSaveTimer = new Timer();
ref Timer m_HeroesAndBanditsCheckTimer = new Timer();

class HeroesAndBandits
{
	ref array<ref HeroesAndBanditsPlayer> HeroesAndBanditsPlayers = new ref array< ref HeroesAndBanditsPlayer >;
		
	void HeroesAndBandits()
	{
	}
	
	void SaveAllPlayers()
	{
		ref array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		for ( int i = 0; i < players.Count(); i++ )
		{
			PlayerBase player;
			player = PlayerBase.Cast(players.Get(i));
			GetPlayer(player.GetIdentity().GetPlainId()).saveData();
		}
		habPrint("Saved all Player Data", "Verbose");
	}
	
	void OnPlayerConnect(PlayerIdentity playerIdent)
	{
		if (IsPlayerLoaded(playerIdent.GetPlainId()))
		{
			habPrint("Player " + playerIdent.GetPlainId()+ " connected and already loaded", "Verbose");	
			return;
		}else{
			habPrint("Player " + playerIdent.GetPlainId()+ " connected loading player from database", "Verbose");	
			HeroesAndBanditsPlayers.Insert(new ref HeroesAndBanditsPlayer(playerIdent.GetPlainId()));
		}
	}
	
	void OnPlayerDisconnect(PlayerBase player)
	{
		habPrint("Player " + player.GetIdentity().GetPlainId()+ " Disconnecting saving player data", "Verbose");	
		GetPlayer(player.GetIdentity().GetPlainId()).saveData();
	}
	
	void NewPlayerAction(string playerID, string action)
	{
		for ( int i = 0; i < HeroesAndBanditsPlayers.Count(); i++ )
		{
			HeroesAndBanditsPlayer p = HeroesAndBanditsPlayers.Get(i);
			if ( p.PlayerID ==  playerID)
			{
				bool didLevelUp = p.NewAction(action);
				if (GetHeroesAndBanditsConfig().getAction(action).NotifiyPlayer){
					
					NotifyPlayer(playerID, p.getLevel().LevelImage, "" + GetHeroesAndBanditsConfig().getActionHumanity(action) + " Humanity" );	
				}
				if (didLevelUp && GetHeroesAndBanditsConfig().NotifyLevelChange)
				{
					NotifyPlayer(playerID, p.getLevel().LevelImage, "You are now " + p.getLevel().Name, "Level Changed" );
				}
				return;
			}
		}
	}
	
	void NotifyPlayer(string playerID, string image ,string message, string heading = "Humanity Affected")
	{
		GetNotificationSystem().CreateNotification(heading, message , image, GetHeroesAndBanditsConfig().getNotificationColor(), GetHeroesAndBanditsConfig().NotificationMessageTime, GetPlayerBaseByID(playerID).GetIdentity());
		habPrint("Notify Player: " + playerID +" Message: "+ message + " Image: " + image, "Verbose");	
	}
	
	void WarnPlayer( string header, string message, string playerID)
	{
		GetNotificationSystem().CreateNotification(header, message , GetHeroesAndBanditsConfig().WarningMessageImagePath, GetHeroesAndBanditsConfig().getWarningMessageColor(), GetHeroesAndBanditsConfig().NotificationMessageTime, GetPlayerBaseByID(playerID).GetIdentity());
		habPrint("Issued Warning '"+ message + "' To Player: " + playerID, "Verbose");
	}
	
	PlayerBase GetPlayerBaseByID( string pID )
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
		return p;
	}
	
	HeroesAndBanditsPlayer GetPlayer( string pID )
	{
		for ( int i = 0; i < HeroesAndBanditsPlayers.Count(); i++ )
		{
			if ( HeroesAndBanditsPlayers.Get(i).PlayerID ==  pID)
			{
				return HeroesAndBanditsPlayers.Get(i);
			}
		}
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
		for ( int i = 0; i < HeroesAndBanditsPlayers.Count(); i++ )
		{
			HeroesAndBanditsPlayer p = HeroesAndBanditsPlayers.Get(i);
			if ( p.PlayerID ==  pID)
			{
				habPrint("Getting Humanity for Player " + p.PlayerID + " Humanity is " + p.Humanity, "Debug");	
				return p.Humanity;
			}
		}
		return 0;
	}
	
	int GetPlayerStat( string pID, string stat )
	{
		for ( int i = 0; i < HeroesAndBanditsPlayers.Count(); i++ )
		{
			HeroesAndBanditsPlayer p = HeroesAndBanditsPlayers.Get(i);
			if ( p.PlayerID ==  pID)
			{
				habPrint("Getting Stat " + stat + " for Player " + p.PlayerID + " total is " + p.getStat(stat), "Debug");	
				return p.getStat(stat);
			}
		}
		return 0;
	}
	
	string GetPlayerAffinity( string pID )
	{
		for ( int i = 0; i < HeroesAndBanditsPlayers.Count(); i++ )
		{
			HeroesAndBanditsPlayer p = HeroesAndBanditsPlayers.Get(i);
			if ( p.PlayerID ==  pID)
			{
				habPrint("Geting Affinity Player " + p.PlayerID + " Affinity " + p.getAffinity(), "Debug");	
				return p.getAffinity();
				
			}
		}
		return GetHeroesAndBanditsConfig().DefaultLevel.Affinity;
	}
	
	habLevel GetPlayerLevel( string pID )
	{
		for ( int i = 0; i < HeroesAndBanditsPlayers.Count(); i++ )
		{
			HeroesAndBanditsPlayer p = HeroesAndBanditsPlayers.Get(i);
			if ( p.PlayerID ==  pID)
			{
				habPrint("Geting Affinity Player " + p.PlayerID + " Affinity " + p.getAffinity(), "Debug");	
				return p.getLevel();
				
			}
		}
		return GetHeroesAndBanditsConfig().DefaultLevel;
	}
	
	string GetPlayerLevelName( string pID )
	{
		for ( int i = 0; i < HeroesAndBanditsPlayers.Count(); i++ )
		{
			HeroesAndBanditsPlayer p = HeroesAndBanditsPlayers.Get(i);
			if ( p.PlayerID ==  pID)
			{
				habPrint("Geting Affinity Player " + p.PlayerID + " Affinity " + p.getAffinity(), "Debug");	
				return p.getLevel().Name;
				
			}
		}
		return GetHeroesAndBanditsConfig().DefaultLevel.Name;
	}
	
	void CheckPlayersEnterZones(){
			habPrint("Checking if Players are in Zones", "Debug");			
			ref array<Man> players = new array<Man>;
			GetGame().GetPlayers(players);
			for (int j = 0; j < players.Count(); j++)
			{
				PlayerBase player = PlayerBase.Cast(players.Get(j));
				float playerHumanity = GetPlayerHumanity(player.GetIdentity().GetPlainId());
				if ( !player )
					continue;
				
				if (player.IsAlive())
				{				
					for (int k = 0; k < GetHeroesAndBanditsConfig().Zones.Count(); k++)
					{	
						if (GetHeroesAndBanditsConfig().Zones.Get(k).validHumanity(playerHumanity) && vector.Distance(player.GetPosition(), GetHeroesAndBanditsConfig().Zones.Get(k).getVector()) <= GetHeroesAndBanditsConfig().Zones.Get(k).WarningRadius && player.m_HeroesAndBandits_WarningSent != k ){
							habPrint("Player: " + player.GetIdentity().GetName() + " ("+player.GetIdentity().GetPlainId()+") Entered: " + GetHeroesAndBanditsConfig().Zones.Get(k).Name, "Verbose");
							player.m_HeroesAndBandits_WarningSent = k;
						}
						else if (!GetHeroesAndBanditsConfig().Zones.Get(k).validHumanity(playerHumanity) && player.m_HeroesAndBandits_WarningSent == k && vector.Distance(player.GetPosition(), GetHeroesAndBanditsConfig().Zones.Get(k).getVector()) <= GetHeroesAndBanditsConfig().Zones.Get(k).KillRadius)
						{
							//Player Entered Kill Zone Kill Player if warning has been given
							player.m_HeroesAndBandits_WarningSent = -1;
							habPrint("Killed Player: " + player.GetIdentity().GetName() + " ("+player.GetIdentity().GetPlainId()+") for Entering Zone: " + GetHeroesAndBanditsConfig().Zones.Get(k).Name, "Always");
							player.SetHealth(0.0);
						}
						else if (!GetHeroesAndBanditsConfig().Zones.Get(k).validHumanity(playerHumanity) && player.m_HeroesAndBandits_WarningSent != k && vector.Distance(player.GetPosition(), GetHeroesAndBanditsConfig().Zones.Get(k).getVector()) <= GetHeroesAndBanditsConfig().Zones.Get(k).WarningRadius)
						{
							//Player Entered Warning Zone Issue Warning
							player.m_HeroesAndBandits_WarningSent = k ;
							habPrint("Player: " + player.GetIdentity().GetName() + " ("+player.GetIdentity().GetPlainId()+") Entered: " + GetHeroesAndBanditsConfig().Zones.Get(k).Name, "Verbose");
							GetHeroesAndBandits().WarnPlayer(GetHeroesAndBanditsConfig().Zones.Get(k).Name, GetHeroesAndBanditsConfig().Zones.Get(k).WarningMessage, player.GetIdentity().GetPlainId());
						}else if (vector.Distance(player.GetPosition(), GetHeroesAndBanditsConfig().Zones.Get(k).getVector()) > GetHeroesAndBanditsConfig().Zones.Get(k).WarningRadius && player.m_HeroesAndBandits_WarningSent == k)
						{
							//Player Left Warning Radius
							habPrint("Player: " + player.GetIdentity().GetName() + " ("+player.GetIdentity().GetPlainId()+") Left: " + GetHeroesAndBanditsConfig().Zones.Get(k).Name, "Verbose");
							player.m_HeroesAndBandits_WarningSent = -1;
						}
					}
				}
			}
	}
	
}

static ref HeroesAndBandits GetHeroesAndBandits()
{
	if (!m_HeroesAndBandits)
	{
		habPrint("Init", "Always");
		m_HeroesAndBandits = new HeroesAndBandits;
		m_HeroesAndBanditsSaveTimer.Run(300, m_HeroesAndBandits, "SaveAllPlayers", NULL, true);
		if (GetHeroesAndBanditsConfig().ZoneCheckTimer >= 1 ){
			habPrint("Creating Zone Check Timer", "Debug");			
			m_HeroesAndBanditsCheckTimer.Run(GetHeroesAndBanditsConfig().ZoneCheckTimer, m_HeroesAndBandits, "CheckPlayersEnterZones", NULL, true);
		}else{
			habPrint("Zone Check Time Less than 1 so not creating timer", "Debug");					
		}
	}
	return m_HeroesAndBandits;
}

//Always Exception Verbose Debug
static void habPrint(string message, string msgType){
	if (msgType == "Always"){
		Print("[HeroesAndBandits]  " + message);
	}else if (msgType == "Exception" && GetHeroesAndBanditsConfig().ExceptionLogs){
		Print("[HeroesAndBandits] [Exception]  " + message);
	}else if (msgType == "Verbose" && GetHeroesAndBanditsConfig().VerboseLogs){
		Print("[HeroesAndBandits] [Verbose]  " + message);
	}else if (msgType == "Debug" && GetHeroesAndBanditsConfig().DebugLogs){
		Print("[HeroesAndBandits] [Debug]  " + message);
	}
}
