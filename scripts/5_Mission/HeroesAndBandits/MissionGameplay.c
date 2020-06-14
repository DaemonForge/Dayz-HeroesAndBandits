modded class MissionGameplay
{
	ref HeroesAndBanditsIconUI				m_HeroesAndBanditsIconUI;
	string									m_HeroesAndBanditsCurrentIcon;
	
	override void OnInit()
	{
		super.OnInit();
		m_HeroesAndBanditsIconUI = new HeroesAndBanditsIconUI;
		m_HeroesAndBanditsIconUI.Init();
		if ( !m_HeroesAndBanditsCurrentIcon ) {
			m_HeroesAndBanditsCurrentIcon = "set:HeroesAndBandits image:Bambi";
		}
		GetRPCManager().AddRPC( "HaB", "RPCUpdateHABIcon", this, SingeplayerExecutionType.Both );
	}
	
	
	void RPCUpdateHABIcon( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
		Param2< string, string > data  //Player ID, Icon
		if ( !ctx.Read( data ) ) return;
		string newIcon = data.param2;
		string playerID = data.param1;
		Print("[HeroesAndBandits] [DebugClient] " + playerID + " send icon " + newIcon);
		if ( type == CallType.Server )
    	{	
			if ( !newIcon || !playerID )
			{
				return;
			}
			if ( newIcon != m_HeroesAndBanditsCurrentIcon)
			{
				m_HeroesAndBanditsCurrentIcon = newIcon;
				GetGame().GetCallQueue( CALL_CATEGORY_GUI ).CallLaterByName(this, "UpdateHABIcon", 1000, false);
			} 
			else 
			{
				return;
			}
		}
	}
	
	void UpdateHABIcon(){
		if ( m_HeroesAndBanditsIconUI ) 
		{
			m_HeroesAndBanditsIconUI.updateIcon(m_HeroesAndBanditsCurrentIcon);
		}
	}
	
	
	
	
	
		
	
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
			Print("[HeroesAndBandits] eventTypeId != ChatMessageEventTypeID");
			
			super.OnEvent(eventTypeId,params); 
			 return; 
		}
		
		ChatMessageEventParams chat_params = ChatMessageEventParams.Cast( params );
		if(chat_params.param1 != 0 || chat_params.param2 == "") 
		{ 
			Print("[HeroesAndBandits] chat_params.param1 != 0 || chat_params.param2 == ''");
			
			super.OnEvent(eventTypeId,params);
			 return; 
		}
		
		Print("[HeroesAndBandits] chat_prams 1: " + chat_params.param1);
		Print("[HeroesAndBandits] chat_prams 2: " + chat_params.param2);
		Print("[HeroesAndBandits] chat_prams 3: " + chat_params.param3);
		Print("[HeroesAndBandits] chat_prams 4: " + chat_params.param4);
		PlayerBase player = GetGame().GetPlayer();		
		
		if (!player) { 
			Print("[HeroesAndBandits] Player is Not found: " + chat_params.param2);
			super.OnEvent(eventTypeId,params); 
			return; 
		}
		
		string message = chat_params.param3;
		string prefix;
		string param0;
		string command;
		
		Print("[HeroesAndBandits] chat_prams 3: " + chat_params.param3);
		Print("[HeroesAndBandits] name: " + chat_params.param2);
		Print("[HeroesAndBandits] prefix: " + prefix);
		Print("[HeroesAndBandits] param0: " + param0);
		Print("[HeroesAndBandits] command: " + command);
		Print("[HeroesAndBandits] Message: " + message);
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
			Print("[HeroesAndBandits] prefix != cmd_prefix");
			super.OnEvent(eventTypeId,params); 
			 return; 
		};
		param0.ParseStringEx(command);
		string playerID = player.GetIdentity().GetPlainId();
		PlayerIdentity identity = player.GetIdentity();
		bool statExsit = false;
		
		switch(command) {
			case "humanity": {
				GetRPCManager().SendRPC("HaB", "RPCSendHumanityNotification", new Param2< string, string >(playerID, command), false, identity);
				break;
			}
			case "stat": {
				GetRPCManager().SendRPC("HaB", "RPCSendStatNotification", new Param3< string, string, string >(playerID, command, statname), false, identity);
				break;
			}
			default: {
				super.OnEvent(eventTypeId,params); 
				return; 
				break;
			}
		}
	}
}