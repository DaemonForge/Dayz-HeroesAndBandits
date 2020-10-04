modded class MissionServer extends MissionBase
{
    void ~MissionServer()
	{
		habPrint("Saving All Player Data From ~MissionServer", "Debug");
		GetHeroesAndBandits().SaveAllPlayers();
	}
	
	override void OnInit()
	{
		super.OnInit();
		ref HeroesAndBanditsSimpleConfig simpleConfig = new ref HeroesAndBanditsSimpleConfig();
		int useSimple = simpleConfig.Load();
		if (simpleConfig.UseSimple == 2){
			habPrint("Converting Simple Config", "Always");
			GetHeroesAndBanditsSettings().Save();
			GetHeroesAndBanditsLevels().Save();
			GetHeroesAndBanditsActions().Save();
			GetHeroesAndBanditsZones().Save();
			simpleConfig.UseSimple = 0;
			simpleConfig.Save();
		}
		GetHeroesAndBanditsSettings();
		GetHeroesAndBanditsLevels();
		GetHeroesAndBanditsActions();
		GetHeroesAndBanditsZones();
		GetHeroesAndBandits();
		GetRPCManager().AddRPC( "HaB", "RPCSendHumanityNotification", this, SingeplayerExecutionType.Both );
		GetRPCManager().AddRPC( "HaB", "RPCSendStatNotification", this, SingeplayerExecutionType.Both );
		GetRPCManager().AddRPC( "HaB", "RPCSendAffinityUpdate", this, SingeplayerExecutionType.Both );
		GetRPCManager().AddRPC( "HaB", "RPCRequestHABPlayerData", this, SingeplayerExecutionType.Both );
		GetRPCManager().AddRPC( "HaB", "RPCRequestHABIcon", this, SingeplayerExecutionType.Both );
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLaterByName(this, "UpdateAllPlayersSettings", 600 * 1000, true);
	}
	
	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		habPrint("InvokeOnConnect Player Connected", "Debug");
		GetHeroesAndBandits().OnPlayerConnect(identity);
		super.InvokeOnConnect(player, identity);
		if ( identity )
		{
			string playerID = identity.GetPlainId();
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLaterByName(this, "SendHeroesAndBanditsSettings", 2000, false, new Param1<ref PlayerBase >( player ));
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLaterByName(this, "SendHeroesAndBanditsSettings", 5500, false, new Param1<ref PlayerBase >( player ));
		
		}
	}
	
	
	
	override void InvokeOnDisconnect( PlayerBase player )
	{
		if ( player.GetIdentity() ){
			habPrint("InvokeOnDisconnect Player Disconneted", "Debug");
			GetHeroesAndBandits().OnPlayerDisconnect(player);
		}
		
		super.InvokeOnDisconnect(player);
	}
	
	
		
	void RPCRequestHABIcon( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
		Param1< string > data;
		if ( !ctx.Read( data ) ) return;
        string playerName = data.param1;
		if ( playerName != "" ){
			PlayerBase player = PlayerBase.Cast(habGetPlayerBaseByName(playerName));
			string imageIcon = GetHeroesAndBanditsLevels().DefaultAffinity.Image;
			if (player.habGetAffinityIndex() != -1){
				imageIcon = GetHeroesAndBanditsLevels().Affinities.Get(player.habGetAffinityIndex()).Image;
			}
			if ( GetHeroesAndBanditsSettings().Expansion_ImageTypePlayerTag == 1 ){
				imageIcon = GetHeroesAndBanditsLevels().DefaultLevel.LevelImage;
				if (player.habGetAffinityIndex() != -1){
					imageIcon =  GetHeroesAndBanditsLevels().Levels.Get(player.habGetLevelIndex()).LevelImage;
				}
			}
			GetRPCManager().SendRPC("HaB", "RPCReceiveHABIcon", new Param2< string, string >( imageIcon, playerName ), true, sender);
		}
	}
	
	void SendHeroesAndBanditsSettings( PlayerBase player ){
		if (player.IsPlayerDisconnected()) { return; }
		PlayerIdentity identity = PlayerIdentity.Cast(player.GetIdentity());
		if (identity){
			string playerID = identity.GetPlainId();
			habPrint("Sending Settings to Player: " + playerID, "Debug");
			GetRPCManager().SendRPC("HaB", "RPCUpdateHABSettings", new Param4< HeroesAndBanditsSettings, HeroesAndBanditsConfigActions, HeroesAndBanditsConfigLevels, HeroesAndBanditsPlayer> (GetHeroesAndBanditsSettings(), GetHeroesAndBanditsActions(), GetHeroesAndBanditsLevels(), GetHeroesAndBandits().GetPlayer(playerID)), true, identity);
			HeroesAndBanditsPlayer playerData = GetHeroesAndBandits().GetPlayer(playerID);
			habLevel playerLevel = playerData.getLevel();
			
			GetRPCManager().SendRPC("HaB", "RPCUpdateHABPlayerData", new Param2< HeroesAndBanditsPlayer, habLevel >( playerData, playerLevel ), true, identity);
		}
	}
	
	
	void RPCSendHumanityNotification( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
		if ( GetHeroesAndBanditsSettings().AllowHumanityCommand )
		{
			if (!sender)
			{
				return;
			}
			Param1< string > data  //Icon
			if ( !ctx.Read( data ) ) return;
			
			string playerID = sender.GetPlainId();
			habPrint(" Command from:" + playerID + " Command:" + data.param1 , "Debug");
			string habMessage = "#HAB_CHECK_PREHUMANITY " + GetHeroesAndBandits().GetPlayerHumanity(playerID);
			GetHeroesAndBandits().NotifyPlayer( playerID, GetHeroesAndBandits().GetPlayerLevel(playerID).LevelImage , habMessage, GetHeroesAndBandits().GetPlayerLevel(playerID).Name);
		}
	}
	
	
	void RPCRequestHABPlayerData( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
		if (!sender)
		{
			return;
		}
		string playerID = sender.GetPlainId();
		habPrint(" Request from:" + playerID + " for player data:", "Debug");
		HeroesAndBanditsPlayer playerData = GetHeroesAndBandits().GetPlayer(playerID);
		habLevel playerLevel = playerData.getLevel();
		habPrint("Player: " + playerID + " Requested Player Data", "Debug");
		GetRPCManager().SendRPC("HaB", "RPCUpdateHABPlayerData", new Param2< HeroesAndBanditsPlayer, habLevel >( playerData, playerLevel ), true, sender);
	}
	
	
	void RPCSendStatNotification( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
		if ( GetHeroesAndBanditsSettings().AllowStatCommand )
		{
			if (!sender)
			{
				return;
			}
			Param2< string, string > data  //Player ID, Icon
			if ( !ctx.Read( data ) ) 
			{
				return;
			}
			string statname = data.param2;
			string playerID = sender.GetPlainId();
			habPrint(" Command from:" + playerID + " Command:" + data.param1 + " " + statname , "Debug");
			string habMessage;
			int statTotal = 0;
			bool statExsit = false;
			string statDisplayName;
			if ( statname ==  "kills" || statname == "kill"){
				statExsit = true;
				statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, "Kill");
				statDisplayName = "Kill";
			} else if ( statname ==  "hunt" || statname == "hunts"){
				statExsit = true;
				statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, "Hunt");
				statDisplayName = "Hunt";
			}  else if ( statname ==  "medic" ){
				statExsit = true;
				statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, "Medic");
				statDisplayName = "Medic";
			} else if ( statname ==  "raid" ||  statname ==  "raids" ){
				statExsit = true;
				statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, "Raid");
				statDisplayName = "Raid";
			} else if ( statname ==  "mission" || statname ==  "missions" ){
				statExsit = true;
				statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, "Mission");
				statDisplayName = "Mission";
			} else if ( statname ==  "sucide" || statname ==  "sucides" ){
				statExsit = true;
				statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, "Sucide");
				statDisplayName = "Sucides";
			} else {
				if (GetHeroesAndBanditsActions().getAction(statname).Name != "Null")
				{
					statExsit = true;
					statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, GetHeroesAndBanditsActions().getAction(statname).Name);
					statDisplayName = GetHeroesAndBanditsActions().getAction(statname).Name;
				}
			}
			if (statExsit){
				habMessage = "#HAB_CHECK_PRESTAT '" + statDisplayName + "' #HAB_CHECK_IS " + statTotal;
				GetHeroesAndBandits().NotifyPlayer( playerID, GetHeroesAndBandits().GetPlayerLevel(playerID).LevelImage , habMessage, GetHeroesAndBandits().GetPlayerLevel(playerID).Name);
			} else {
				habMessage = "#HAB_CHECK_NOTFOUND '" + statname + "'";
				GetHeroesAndBandits().NotifyPlayer( playerID, GetHeroesAndBandits().GetPlayerLevel(playerID).LevelImage , habMessage, GetHeroesAndBandits().GetPlayerLevel(playerID).Name);
			}
		}
	}
	
	//Adding so debuggin is easier :)
	void RPCSendAffinityUpdate( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
		if ( GetHeroesAndBanditsSettings().DebugCommand )
		{
			if (!sender)
			{
				return;
			}
			Param3< string, string, int > data  //Player ID, Icon
			if ( !ctx.Read( data ) ) 
			{
				return;
			}
			string statname = data.param2;
			string playerID = sender.GetPlainId();
			habPrint("Affinity Update from:" + playerID + " Command:" + data.param1 + " " + statname + " " + data.param3, "Always");
			GetHeroesAndBandits().GetPlayer(playerID).addAffinityPoints(statname, data.param3);
			PlayerBase player = PlayerBase.Cast(habGetPlayerBaseByID(playerID));
			player.habLevelChange( GetHeroesAndBandits().GetPlayer(playerID).getAffinityIndex() , GetHeroesAndBandits().GetPlayer(playerID).getAffinityPoints(GetHeroesAndBandits().GetPlayer(playerID).getAffinityName()), GetHeroesAndBandits().GetPlayer(playerID).getLevelIndex());
		}
	}
	
	void UpdateAllPlayersSettings(){
		PlayerBase p;
		string playerID;
		ref array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		for ( int i = 0; i < players.Count(); i++ )
		{
			p = PlayerBase.Cast(players.Get(i));
			if (!p.IsPlayerDisconnected())
			{
				playerID = p.GetIdentity().GetPlainId();
				GetRPCManager().SendRPC("HaB", "RPCUpdateHABSettings", new Param4< HeroesAndBanditsSettings, HeroesAndBanditsConfigActions, HeroesAndBanditsConfigLevels, HeroesAndBanditsPlayer> (GetHeroesAndBanditsSettings(), GetHeroesAndBanditsActions(), GetHeroesAndBanditsLevels(), GetHeroesAndBandits().GetPlayer(playerID)), true, p.GetIdentity());
			}
		}
	}
		
};
