ref HeroesAndBandits m_HeroesAndBandits;
ref HeroesAndBanditsConfig m_HeroesAndBanditsConfig
ref Timer m_HeroesAndBanditsSaveTimer = new Timer();
ref Timer m_HeroesAndBanditsCheckTimer = new Timer();
ref Timer m_HeroesAndBanditsReloadTest = new Timer();
ref NotificationSystem m_HeroesAndBanditsNotificationSystem = new NotificationSystem();

class HeroesAndBandits
{
	ref array<ref HeroesAndBanditsPlayer> HeroesAndBanditsPlayers = new ref array< ref HeroesAndBanditsPlayer >;
	
	
	ref array< ref HeroesAndBanditsZone > Zones = new ref array< ref HeroesAndBanditsZone >;
	
	void HeroesAndBandits()
	{
	}
	
	void Init(){
		if (GetHeroesAndBanditsConfig().Zones) //Check if Zones are defined before loading
		{
			for ( int i = 0; i < GetHeroesAndBanditsConfig().Zones.Count(); i++ )
			{
		    	habPrint("Loading Zone " + GetHeroesAndBanditsConfig().Zones.Get(i).Name , "Verbose");
				string name = GetHeroesAndBanditsConfig().Zones.Get(i).Name;
				int x = GetHeroesAndBanditsConfig().Zones.Get(i).X;
				int z = GetHeroesAndBanditsConfig().Zones.Get(i).Z;
				float minHumanity = GetHeroesAndBanditsConfig().Zones.Get(i).MinHumanity;
				float maxHumanity = GetHeroesAndBanditsConfig().Zones.Get(i).MaxHumanity;
				int warningRadius = GetHeroesAndBanditsConfig().Zones.Get(i).WarningRadius;
				int killRadius = GetHeroesAndBanditsConfig().Zones.Get(i).KillRadius;
				string warningMessage = GetHeroesAndBanditsConfig().Zones.Get(i).WarningMessage;
				bool overrideSafeZone = GetHeroesAndBanditsConfig().Zones.Get(i).OverrideSafeZone;
				bool godModPlayers = GetHeroesAndBanditsConfig().Zones.Get(i).GodModPlayers;
				Zones.Insert(new ref HeroesAndBanditsZone(name, x, z, minHumanity, maxHumanity, warningRadius, killRadius, warningMessage, overrideSafeZone, godModPlayers));
				if (GetHeroesAndBanditsConfig().Zones.Get(i).Guards){
					for ( int j = 0; j < GetHeroesAndBanditsConfig().Zones.Get(i).Guards.Count(); j++ )
					{	
						float guardX = GetHeroesAndBanditsConfig().Zones.Get(i).Guards.Get(j).X;
						float guardY = GetHeroesAndBanditsConfig().Zones.Get(i).Guards.Get(j).Y; 
						float guardZ = GetHeroesAndBanditsConfig().Zones.Get(i).Guards.Get(j).Z; 
						float orientation = GetHeroesAndBanditsConfig().Zones.Get(i).Guards.Get(j).Orientation; 
						string skin = GetHeroesAndBanditsConfig().Zones.Get(i).Guards.Get(j).Skin; 
						string weaponInHands = GetHeroesAndBanditsConfig().Zones.Get(i).Guards.Get(j).WeaponInHands; 
						string weaponInHandsMag = GetHeroesAndBanditsConfig().Zones.Get(i).Guards.Get(j).WeaponInHandsMag;
						m_HeroesAndBanditsReloadTest.Run(120, GetHeroesAndBanditsConfig().Zones.Get(i).Guards.Get(j), "ReloadWeapon", NULL, false); //Reload gun 2 minutes after server start
						TStringArray weaponInHandsAttachments = GetHeroesAndBanditsConfig().Zones.Get(i).Guards.Get(j).WeaponInHandsAttachments; 
						TStringArray guardGear = GetHeroesAndBanditsConfig().Zones.Get(i).Guards.Get(j).GuardGear;
						Zones.Get(i).Guards.Insert(new ref HeroesAndBanditsGuard(guardX, guardY, guardZ, orientation, skin, weaponInHands, weaponInHandsMag, weaponInHandsAttachments, guardGear));
			    		Zones.Get(i).Guards.Get(j).Spawn();
					}
				}
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
					string prefix = "";
					float actionHumanity = GetHeroesAndBanditsConfig().getActionHumanity(action);
					if (actionHumanity > 0){
						prefix = "+";
					}
					NotifyPlayer(playerID, p.getLevel().LevelImage, prefix + actionHumanity + " Humanity" );	
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
		m_HeroesAndBanditsNotificationSystem.CreateNotification(new ref StringLocaliser(heading), new ref StringLocaliser(message), image, GetHeroesAndBanditsConfig().getNotificationColor(), GetHeroesAndBanditsConfig().NotificationMessageTime, GetPlayerBaseByID(playerID).GetIdentity());
		habPrint("Notify Player: " + playerID +" Message: "+ message + " Image: " + image, "Verbose");	
	}
	
	void NotifyKillFeed(string image ,string message, string heading = "Kill Feed")
	{
		m_HeroesAndBanditsNotificationSystem.CreateNotification(new ref StringLocaliser(heading), new ref StringLocaliser(message), image, GetHeroesAndBanditsConfig().getKillFeedMessageColor(), GetHeroesAndBanditsConfig().NotificationMessageTime);
		habPrint("Kill Feed Message: "+ message + " Image: " + image, "Verbose");	
	}
	
	void WarnPlayer( string header, string message, string playerID)
	{
		m_HeroesAndBanditsNotificationSystem.CreateNotification(new ref StringLocaliser(header), new ref StringLocaliser(message), GetHeroesAndBanditsConfig().WarningMessageImagePath, GetHeroesAndBanditsConfig().getWarningMessageColor(), GetHeroesAndBanditsConfig().NotificationMessageTime, GetPlayerBaseByID(playerID).GetIdentity());
		habPrint("Issued Warning '"+ message + "' To Player: " + playerID, "Verbose");
	}
	
	
	void TriggerKillFeed(string sourcePlayerID, string targetPlayerID, string weaponName){
		habPrint("Kill Feed Player: " + sourcePlayerID +" killed " + targetPlayerID + " with " + weaponName , "Debug");
		if (GetHeroesAndBanditsConfig().KillFeed){
			PlayerBase sourcePlayer = GetPlayerBaseByID(sourcePlayerID);
			PlayerBase targetPlayer = GetPlayerBaseByID(targetPlayerID);
			string levelImage = GetPlayerLevel(sourcePlayerID).LevelImage;
			string levelName = GetPlayerLevel(sourcePlayerID).Name;
			float distance = Math.Round(vector.Distance(sourcePlayer.GetPosition(), targetPlayer.GetPosition()));
			string message = "A " + levelName + " " + sourcePlayer.GetIdentity().GetName() + " killed " + targetPlayer.GetIdentity().GetName() + " with " + weaponName + " at " + distance + " meters" ;
			NotifyKillFeed(levelImage, message);
		}
	}
	
	void TriggerSucideFeed(string sourcePlayerID){
		habPrint("Sucide Kill Feed Player: " + sourcePlayerID, "Debug");
		if (GetHeroesAndBanditsConfig().SucideFeed){
			PlayerBase sourcePlayer = GetPlayerBaseByID(sourcePlayerID);
			string levelImage = GetPlayerLevel(sourcePlayerID).LevelImage;
			string message = sourcePlayer.GetIdentity().GetName() + " killed them self" ;
			NotifyKillFeed(levelImage, message, "Player Sucide");
		}
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
	    habPrint("Failed to GetPlayerBaseByID for Player " + pID , "Verbose");
		return null;
	}
		
	PlayerBase GetPlayerBaseByName( string pName )
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
				habPrint("Getting Humanity for Player " + p.PlayerID + " Humanity is " + p.Humanity, "Debug");	
				return p.Humanity;
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
	    habPrint("Failed to get Stat " + stat + " for Player " + pID , "Verbose");	
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
				habPrint("Geting Affinity Player " + p.PlayerID + " Affinity " + p.getAffinity(), "Debug");	
				return p.getAffinity();
				
			}
		}
	    habPrint("Failed to get Affinity for Player " + pID , "Verbose");	
		return GetHeroesAndBanditsConfig().DefaultLevel.Affinity;
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
				habPrint("Geting Affinity Player " + p.PlayerID + " Affinity " + p.getAffinity(), "Debug");	
				return p.getLevel();
				
			}
		}
	    habPrint("Failed to get Level for Player " + pID , "Verbose");
		return GetHeroesAndBanditsConfig().DefaultLevel;
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
				habPrint("Geting Affinity Player " + p.PlayerID + " Affinity " + p.getAffinity(), "Debug");	
				return p.getLevel().Name;
				
			}
		}
	    habPrint("Failed to get Level Name for Player " + pID , "Verbose");
		return GetHeroesAndBanditsConfig().DefaultLevel.Name;
	}
	
	string getPlayerSkin(string playerID, int skinIndex = -1){
		if (GetPlayerAffinity(playerID) == "hero")
		{
			if (skinIndex == -1 || GetHeroesAndBanditsConfig().HeroSkins.Count() > skinIndex){
				return GetHeroesAndBanditsConfig().HeroSkins.GetRandomElement();
			} else {
				return GetHeroesAndBanditsConfig().HeroSkins.Get(skinIndex);
			}
		} else if (GetPlayerAffinity(playerID) == "bandit")
		{
			if (skinIndex == -1 || GetHeroesAndBanditsConfig().BanditSkins.Count() > skinIndex){
				return GetHeroesAndBanditsConfig().BanditSkins.GetRandomElement();
			} else {
				return GetHeroesAndBanditsConfig().BanditSkins.Get(skinIndex);
			}
		}
		if (skinIndex == -1 || GetHeroesAndBanditsConfig().BambiSkins.Count() > skinIndex){
			return GetHeroesAndBanditsConfig().BambiSkins.GetRandomElement();
		} else {
			return GetHeroesAndBanditsConfig().BambiSkins.Get(skinIndex);
		}
	}
	
	
	void CheckPlayersEnterZones(){
			if (!Zones) //if no zones Defined exit
			{
				return;
			}
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
					for (int k = 0; k < Zones.Count(); k++)
					{	
						habPrint("Checking if Players is in Zone " + Zones.Get(k).Name, "Debug");	
						if (Zones.Get(k).validHumanity(playerHumanity) && vector.Distance(player.GetPosition(), Zones.Get(k).getVector()) <= Zones.Get(k).WarningRadius && player.m_HeroesAndBandits_WarningSent != k ){
							habPrint("Player: " + player.GetIdentity().GetName() + " ("+player.GetIdentity().GetPlainId()+") Entered: " + Zones.Get(k).Name, "Verbose");
							player.m_HeroesAndBandits_WarningSent = k;
							if ( Zones.Get(k).GodModPlayers && Zones.Get(k).validHumanity(playerHumanity))
							{
								player.SetAllowDamage(false);
							}
						}
						else if (!Zones.Get(k).validHumanity(playerHumanity) && player.m_HeroesAndBandits_WarningSent == k && vector.Distance(player.GetPosition(), Zones.Get(k).getVector()) <= Zones.Get(k).KillRadius)
						{
							//Player Entered Kill Zone Kill Player if warning has been given
							player.m_HeroesAndBandits_WarningSent = -1;
							habPrint("Killed Player: " + player.GetIdentity().GetName() + " ("+player.GetIdentity().GetPlainId()+") for Entering Zone: " + Zones.Get(k).Name, "Always");
							if ( Zones.Get(k).OverrideSafeZone )
							{
								player.SetAllowDamage(true);
							}
							Zones.Get(k).FireWeaponClosestGuard(player.GetPosition());
							player.SetHealth(0.0);
						}
						else if (!Zones.Get(k).validHumanity(playerHumanity) && player.m_HeroesAndBandits_WarningSent != k && vector.Distance(player.GetPosition(), Zones.Get(k).getVector()) <= Zones.Get(k).WarningRadius)
						{
							//Player Entered Warning Zone Issue Warning
							player.m_HeroesAndBandits_WarningSent = k ;
							habPrint("Player: " + player.GetIdentity().GetName() + " ("+player.GetIdentity().GetPlainId()+") Entered: " + Zones.Get(k).Name, "Verbose");
							GetHeroesAndBandits().WarnPlayer(Zones.Get(k).Name, Zones.Get(k).WarningMessage, player.GetIdentity().GetPlainId());
						}else if (vector.Distance(player.GetPosition(), Zones.Get(k).getVector()) > Zones.Get(k).WarningRadius && player.m_HeroesAndBandits_WarningSent == k)
						{
							if ( Zones.Get(k).GodModPlayers && Zones.Get(k).validHumanity(playerHumanity) )
							{
								player.SetAllowDamage(true);
							}
							//Player Left Warning Radius
							habPrint("Player: " + player.GetIdentity().GetName() + " ("+player.GetIdentity().GetPlainId()+") Left: " + Zones.Get(k).Name, "Verbose");
							player.m_HeroesAndBandits_WarningSent = -1;
						}
					}
				}
			}
	}
	
}


class HeroesAndBanditsZone
{
    string Name;
	float X;
	float Z;
	int WarningRadius;
	string WarningMessage;
	int KillRadius;
    float MinHumanity;
    float MaxHumanity;
	bool OverrideSafeZone;
	bool GodModPlayers;
	ref array< ref HeroesAndBanditsGuard > Guards = new ref array< ref HeroesAndBanditsGuard >;
	
    void HeroesAndBanditsZone(string name, float x, float z, float minHumanity, float maxHumanity, int warningRadius, int killRadius, string warningMessage, bool overrideSafeZone, bool godModPlayers) 
	{
        Name = name;
		X = x;
		Z = z;
		WarningRadius = warningRadius;
		KillRadius = killRadius;
	    MinHumanity = minHumanity;
	    MaxHumanity = maxHumanity;
		WarningMessage = warningMessage;
		OverrideSafeZone = overrideSafeZone;
		GodModPlayers = godModPlayers;
    }
	
	vector getVector(){
		return Vector( X, GetGame().SurfaceY(X, Z), Z );
	}
	
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
	
	void FireWeaponClosestGuard(vector playerPostion)
	{
		if (!Guards)//If no guards defined exit
		{
			return;
		}
		int closestGuardIndex = -1;
		float closestGuardDistance = 600;
		for ( int i = 0; i < Guards.Count(); i++ )
		{	
			float currentGuardDistance = vector.Distance( Guards.Get(i).getVector(), playerPostion);
			if ( currentGuardDistance < closestGuardDistance)
			{
				closestGuardIndex = i;
				closestGuardDistance = currentGuardDistance;
			}
		}
		if ( closestGuardIndex == -1 ){
			return;
		} else {
			Guards.Get(closestGuardIndex).FireWeapon();
		} 
	}
	
}

class HeroesAndBanditsGuard
{
    float X;
    float Y;
    float Z;
	float Orientation;
	private PlayerBase Guard;
	string Skin;
	string WeaponInHands;
	string WeaponInHandsMag;
	ref TStringArray WeaponInHandsAttachments;
	ref TStringArray GuardGear;

    void HeroesAndBanditsGuard(float x, float y, float z, float orientation, string skin, string weaponInHands, string weaponInHandsMag, TStringArray weaponInHandsAttachments, TStringArray guardGear) 
	{
        X = x;
		Y = y;
        Z = z;
		Orientation = orientation;
		Skin = skin;
		WeaponInHands = weaponInHands;
		WeaponInHandsMag = weaponInHandsMag;
		WeaponInHandsAttachments =  weaponInHandsAttachments;
		GuardGear = guardGear;
		
    }
	
	void Spawn()
	{
			habPrint("Spawning Guard: " + Skin + " at " + " X:" + X + " Y:" + Y +" Z:" + Z, "Verbose");	
			Object obj = GetGame().CreateObject( Skin , getVector(), false, false, true );
			obj.SetPosition(getVector());
			EntityAI weaponInHands;
			EntityAI weaponInHandsMag;
			if (Class.CastTo(Guard, obj))
			{	
				Guard.SetAllowDamage(false);
				for ( int i =0; i < GuardGear.Count(); i++ )
				{
					Guard.GetInventory().CreateAttachment(GuardGear.Get(i));
				}
				weaponInHandsMag = Guard.GetInventory().CreateAttachment("Mag_STANAGCoupled_30Rnd");
				if (Guard.GetHumanInventory().GetEntityInHands()){
					habPrint("Item spawned in Guard: " + Skin + " at " + " X:" + X + " Y:" + Y +" Z:" + Z + " Item: " + Guard.GetHumanInventory().GetEntityInHands().GetDisplayName() +  " Removing it", "Exception");	
					Guard.GetHumanInventory().GetEntityInHands().Delete(); //Remove any Items in Hand
				}
				weaponInHands = Guard.GetHumanInventory().CreateInHands(WeaponInHands);
				weaponInHands.GetInventory().CreateAttachment("Mag_STANAGCoupled_30Rnd");
				for ( int j =0; j < WeaponInHandsAttachments.Count(); j++ )
				{
					weaponInHands.GetInventory().CreateAttachment(WeaponInHandsAttachments.Get(j));
				}
				
			}
			vector guardOrientation = vector.Zero;
			guardOrientation[0] = Orientation;
			guardOrientation[1] = 0;
			guardOrientation[2] = 0;
			
			obj.SetOrientation(guardOrientation);
	}
	
	void ReloadWeapon()
	{
		habPrint("Reloading Gun Guard: " + Skin + " at " + " X:" + X + " Y:" + Y +" Z:" + Z, "Verbose");	
		EntityAI weaponInHands = Guard.GetHumanInventory().GetEntityInHands();
		EntityAI weaponInHandsMag;
		if (weaponInHands.IsWeapon())
		{
			Guard.GetHumanInventory().GetEntityInHands();
			weaponInHandsMag = Guard.GetInventory().CreateAttachment("Mag_STANAGCoupled_30Rnd");
			Guard.ReloadWeapon(Weapon_Base.Cast(weaponInHands), weaponInHandsMag);
		}
	}
	
	void FireWeapon()
	{
		habPrint("Firing Gun Guard: " + Skin + " at " + " X:" + X + " Y:" + Y +" Z:" + Z, "Verbose");	
		EntityAI weaponInHands = EntityAI.Cast(Guard.GetHumanInventory().GetEntityInHands());
		if (weaponInHands.IsWeapon())
		{
			WeaponManager(PlayerBase.Cast(Guard)).Fire(Weapon_Base.Cast(weaponInHands));
		}
	}

	vector getVector(){
		return Vector( X, Y, Z );
	}
}


static ref HeroesAndBandits GetHeroesAndBandits()
{
	if (!m_HeroesAndBandits)
	{
		habPrint("Init", "Always");
		m_HeroesAndBandits = new HeroesAndBandits;
		m_HeroesAndBanditsSaveTimer.Run(300, m_HeroesAndBandits, "SaveAllPlayers", NULL, true);
		m_HeroesAndBandits.Init();
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
