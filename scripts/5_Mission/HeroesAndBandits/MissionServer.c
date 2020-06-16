

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
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLaterByName(this, "UpdateAllPlayersSettings", 240 * 1000, true);
		
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
		habPrint("Sending Settings to Player: " + playerID + ", ShowLevelIcon: " +  GetHeroesAndBanditsConfig().ShowLevelIcon + ", AllowHumanityCommand " + GetHeroesAndBanditsConfig().AllowHumanityCommand + ", AllowStatCommand: " + GetHeroesAndBanditsConfig().AllowStatCommand + ", LevelImage: " + GetHeroesAndBandits().GetPlayerLevel(playerID).LevelImage + ", LevelIconLocation: " + GetHeroesAndBanditsConfig().LevelIconLocation, "Debug");
		GetRPCManager().SendRPC("HaB", "RPCUpdateHABSettings", new Param5< bool, bool, bool, string, int >(GetHeroesAndBanditsConfig().ShowLevelIcon, GetHeroesAndBanditsConfig().AllowHumanityCommand, GetHeroesAndBanditsConfig().AllowStatCommand, GetHeroesAndBandits().GetPlayerLevel(playerID).LevelImage, GetHeroesAndBanditsConfig().LevelIconLocation), true, identity);
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
			string killsStat = "Kills";
			string killStat = "Kill";
			if (statname.ToLower().ToString() == killsStat.ToLower().ToString() || statname.ToLower().ToString() == killStat.ToLower().ToString()){
				statExsit = true;
				statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, "heroVshero");
				statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, "heroVsbambi"); 
				statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, "heroVsbandit");
				statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, "banditVshero"); 
				statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, "banditVsbambi"); 
				statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, "banditVsbandit"); 
				statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, "bambiVshero"); 
				statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, "bambiVsbambi"); 
				statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, "bambiVsbandit");
			} else {
				if (GetHeroesAndBanditsConfig().getAction(statname).Name != "Null")
				{
					statExsit = true;
					statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, GetHeroesAndBanditsConfig().getAction(statname).Name);
				}
			}
			if (statExsit){
				habMessage = "#HAB_CHECK_PRESTAT '" + statname + "' #HAB_CHECK_IS " + statTotal;
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
			playerID = p.GetIdentity().GetPlainId();
			GetRPCManager().SendRPC("HaB", "RPCUpdateHABSettings", new Param5< bool, bool, bool, string, int >(GetHeroesAndBanditsConfig().ShowLevelIcon, GetHeroesAndBanditsConfig().AllowHumanityCommand, GetHeroesAndBanditsConfig().AllowStatCommand, GetHeroesAndBandits().GetPlayerLevel(playerID).LevelImage, GetHeroesAndBanditsConfig().LevelIconLocation), false, p.GetIdentity());
		}
	}
	
	/*
	override void OnEvent(EventType eventTypeId, Param params)  {
	
		ref array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		if(eventTypeId != ChatMessageEventTypeID && eventTypeId != ChatChannelEventTypeID) 
		{ 
			super.OnEvent(eventTypeId,params); 
			 return; 
		}
		
		if(eventTypeId != ChatMessageEventTypeID) 
		{ 
			habPrint("eventTypeId != ChatMessageEventTypeID", "Debug");
			
			super.OnEvent(eventTypeId,params); 
			 return; 
		}
		
		ChatMessageEventParams chat_params = ChatMessageEventParams.Cast( params );
		if(chat_params.param1 != 0 || chat_params.param2 == "") 
		{ 
			habPrint("chat_params.param1 != 0 || chat_params.param2 == ''", "Debug");
			
			super.OnEvent(eventTypeId,params);
			 return; 
		}
		
		habPrint("chat_prams 1: " + chat_params.param1, "Debug");
		habPrint("chat_prams 2: " + chat_params.param2, "Debug");
		habPrint("chat_prams 3: " + chat_params.param3, "Debug");
		habPrint("chat_prams 4: " + chat_params.param4, "Debug");
		PlayerBase player =  PlayerBase.Cast(GetHeroesAndBandits().GetPlayerBaseByName( chat_params.param2 ));
		
		if (!player) { 
			habPrint("Player is Not found: " + chat_params.param2, "Verbose");
			super.OnEvent(eventTypeId,params); 
			return; 
		}
		
		string message = chat_params.param3;
		string prefix;
		string param0;
		string command;
		
		habPrint("chat_prams 3: " + chat_params.param3, "Debug");
		habPrint("name: " + chat_params.param2, "Debug");
		habPrint("prefix: " + prefix, "Debug");
		habPrint("param0: " + param0, "Debug");
		habPrint("command: " + command, "Debug");
		habPrint("Message: " + message, "Debug");
		TStringArray tokens = new TStringArray;
		message.Replace("` ", "&SPCESC!");
		message.Split(" ", tokens); 
		int count = tokens.Count();

		string statname = message;
		statname.Replace("&SPCESC!", " ");
		statname.Replace("/stat", " ");
		statname.Replace(" ", "");
		
		habPrint("statname: " + statname, "Debug");
		string cmd_prefix = "/";
		param0 = tokens.Get(0);
		param0.ParseStringEx(prefix); 
		if(prefix != cmd_prefix) {
			habPrint("prefix != cmd_prefix", "Debug");
			super.OnEvent(eventTypeId,params); 
			 return; 
		};
		param0.ParseStringEx(command);
		string playerID = player.GetIdentity().GetPlainId();
		string habMessage = "";
		bool statExsit = false;
		habPrint("After Parse: ", "Debug");
		habPrint("chat_prams 3: " + chat_params.param3, "Debug");
		habPrint("name: " + chat_params.param2, "Debug");
		habPrint("playerID: " + playerID, "Debug");
		habPrint("prefix: " + prefix, "Debug");
		habPrint("param0: " + param0, "Debug");
		habPrint("command: " + command, "Debug");
		habPrint("Message: " + message, "Debug");
		
		switch(command) {
			case "humanity": {
				habMessage = "#HAB_CHECK_PREHUMANITY " + GetHeroesAndBandits().GetPlayerHumanity(playerID);
				GetHeroesAndBandits().NotifyPlayer( playerID, GetHeroesAndBandits().GetPlayerLevel(playerID).LevelImage , habMessage, GetHeroesAndBandits().GetPlayerLevel(playerID).Name);
				break;
			}
			case "stat": {
				int statTotal = 0;
				if (statname == "Kills"){
					statExsit = true;
					statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, "heroVshero");
					statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, "heroVsbambi"); 
					statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, "heroVsbandit");
					statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, "banditVshero"); 
					statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, "banditVsbambi"); 
					statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, "banditVsbandit"); 
					statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, "bambiVshero"); 
					statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, "bambiVsbambi"); 
					statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, "bambiVsbandit");
				} else {
					if (GetHeroesAndBanditsConfig().getAction(statname).Name != "Null")
					{
						statExsit = true;
						statTotal = statTotal + GetHeroesAndBandits().GetPlayerStat(playerID, statname);
					}
				}
				if (statExsit){
					habMessage = "#HAB_CHECK_PRESTAT '" + statname + "' #HAB_CHECK_IS " + statTotal;
					GetHeroesAndBandits().NotifyPlayer( playerID, GetHeroesAndBandits().GetPlayerLevel(playerID).LevelImage , habMessage, GetHeroesAndBandits().GetPlayerLevel(playerID).Name);
				} else {
					habMessage = "#HAB_CHECK_NOTFOUND '" + statname + "'";
					GetHeroesAndBandits().NotifyPlayer( playerID, GetHeroesAndBandits().GetPlayerLevel(playerID).LevelImage , habMessage, GetHeroesAndBandits().GetPlayerLevel(playerID).Name);
				}
				break;
			}
			default: {
				super.OnEvent(eventTypeId,params); 
				return; 
				break;
			}
		}
	}*/
	
}


