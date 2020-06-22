

modded class MissionServer
{
    void ~MissionServer()
	{
		habPrint("Saving All Player Data From ~MissionServer", "Debug");
		GetHeroesAndBandits().SaveAllPlayers();
	}
		
	override void OnInit()
	{
		super.OnInit();
		GetHeroesAndBanditsConfig();
		GetHeroesAndBandits();
		GetRPCManager().AddRPC( "HaB", "RPCSendHumanityNotification", this, SingeplayerExecutionType.Both );
		GetRPCManager().AddRPC( "HaB", "RPCSendStatNotification", this, SingeplayerExecutionType.Both );
		GetRPCManager().AddRPC( "HaB", "RPCRequestHABPlayerData", this, SingeplayerExecutionType.Both );
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLaterByName(this, "UpdateAllPlayersSettings", 600000, true);
		
	}
	
	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		habPrint("InvokeOnConnect Player Connected", "Debug");
		GetHeroesAndBandits().OnPlayerConnect(identity);
		super.InvokeOnConnect(player, identity);
		if ( identity )
		{
			string playerID = identity.GetPlainId();
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLaterByName(this, "SendHeroesAndBanditsSettings", 4000, false, new Param1<ref PlayerIdentity>(identity));
		}
	}
	
	override void InvokeOnDisconnect( PlayerBase player )
	{
		habPrint("InvokeOnDisconnect Player Disconneted", "Debug");
		if ( player.GetIdentity() ){
			GetHeroesAndBandits().OnPlayerDisconnect(player);
		}
		
		super.InvokeOnDisconnect(player);
	}
	
	
	
	void SendHeroesAndBanditsSettings( PlayerIdentity identity ){
		string playerID = identity.GetPlainId();
		habPrint("Sending Settings to Player: " + playerID + ", ShowLevelIcon: " +  GetHeroesAndBanditsConfig().ShowLevelIcon + ", CommandPrefix: " +  GetHeroesAndBanditsConfig().CommandPrefix + ", AllowHumanityCommand " + GetHeroesAndBanditsConfig().AllowHumanityCommand + ", AllowStatCommand: " + GetHeroesAndBanditsConfig().AllowStatCommand + ", LevelImage: " + GetHeroesAndBandits().GetPlayerLevel(playerID).LevelImage + ", LevelIconLocation: " + GetHeroesAndBanditsConfig().LevelIconLocation, "Debug");
		GetRPCManager().SendRPC("HaB", "RPCUpdateHABSettings", new Param6< bool, string, bool, bool, string, int >(GetHeroesAndBanditsConfig().ShowLevelIcon, GetHeroesAndBanditsConfig().CommandPrefix, GetHeroesAndBanditsConfig().AllowHumanityCommand, GetHeroesAndBanditsConfig().AllowStatCommand, GetHeroesAndBandits().GetPlayerLevel(playerID).LevelImage, GetHeroesAndBanditsConfig().LevelIconLocation), true, identity);
		HeroesAndBanditsPlayer playerData = GetHeroesAndBandits().GetPlayer(playerID);
		habLevel playerLevel = playerData.getLevel();
		
		GetRPCManager().SendRPC("HaB", "RPCUpdateHABPlayerData", new Param4< bool, bool, HeroesAndBanditsPlayer, habLevel >( GetHeroesAndBanditsConfig().AllowGUI, GetHeroesAndBanditsConfig().HideKillsInGUI, playerData, playerLevel ), true, identity);
	}
	
	
	void RPCSendHumanityNotification( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
		if ( GetHeroesAndBanditsConfig().AllowHumanityCommand )
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
		GetRPCManager().SendRPC("HaB", "RPCUpdateHABPlayerData", new Param4< bool, bool, HeroesAndBanditsPlayer, habLevel >( GetHeroesAndBanditsConfig().AllowGUI,  GetHeroesAndBanditsConfig().HideKillsInGUI, playerData, playerLevel ), true, sender);
	}
	
	
	void RPCSendStatNotification( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
		if ( GetHeroesAndBanditsConfig().AllowStatCommand )
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
				statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, "Kills");
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
				if (GetHeroesAndBanditsConfig().getAction(statname).Name != "Null")
				{
					statExsit = true;
					statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, GetHeroesAndBanditsConfig().getAction(statname).Name);
					statDisplayName = GetHeroesAndBanditsConfig().getAction(statname).Name;
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
				GetRPCManager().SendRPC("HaB", "RPCUpdateHABSettings", new Param6< bool, string, bool, bool, string, int >(GetHeroesAndBanditsConfig().ShowLevelIcon, GetHeroesAndBanditsConfig().CommandPrefix, GetHeroesAndBanditsConfig().AllowHumanityCommand, GetHeroesAndBanditsConfig().AllowStatCommand, GetHeroesAndBandits().GetPlayerLevel(playerID).LevelImage, GetHeroesAndBanditsConfig().LevelIconLocation), false, p.GetIdentity());
			}
		}
	}
		
}


