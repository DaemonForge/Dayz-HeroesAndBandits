modded class MissionGameplay
{
	ref HeroesAndBanditsIconUI				m_HeroesAndBanditsIconUI;
	string									m_HeroesAndBanditsCurrentIcon;
	bool 									m_HeroesAndBanditsShowLevelIcon;
	bool 									m_HeroesAndBanditsAllowHumanityCommand;
	bool 									m_HeroesAndBanditsAllowStatCommand;
	
	override void OnInit()
	{
		super.OnInit();

		GetRPCManager().AddRPC( "HaB", "RPCUpdateHABIcon", this, SingeplayerExecutionType.Both );
		GetRPCManager().AddRPC( "HaB", "RPCUpdateHABSettings", this, SingeplayerExecutionType.Both );
	}
	
	
	void InitHabIcon(){
		m_HeroesAndBanditsIconUI = new HeroesAndBanditsIconUI;
		m_HeroesAndBanditsIconUI.Init();
		if ( !m_HeroesAndBanditsCurrentIcon ) {
			m_HeroesAndBanditsCurrentIcon = "set:HeroesAndBandits image:Bambi";
		} else {
			UpdateHABIcon();
		}
	}
	
	
	void RPCUpdateHABSettings( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
		Param3<bool, bool, bool> data;
		if ( !ctx.Read( data ) ) return;
		m_HeroesAndBanditsShowLevelIcon = data.param1;
		m_HeroesAndBanditsAllowHumanityCommand = data.param2;
		m_HeroesAndBanditsAllowStatCommand = data.param3;
		if ( m_HeroesAndBanditsShowLevelIcon )
		{
			InitHabIcon();
		}

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
			if ( !newIcon || !playerID || !m_HeroesAndBanditsShowLevelIcon)
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
		if ( m_HeroesAndBanditsShowLevelIcon && m_HeroesAndBanditsIconUI ) 
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
			super.OnEvent(eventTypeId,params); 
			 return; 
		}
		
		ChatMessageEventParams chat_params = ChatMessageEventParams.Cast( params );
		if(chat_params.param1 != 0 || chat_params.param2 == "") 
		{ 
			super.OnEvent(eventTypeId,params);
			 return; 
		}
		
		PlayerBase player = GetGame().GetPlayer();		
		
		if (!player) { 
			super.OnEvent(eventTypeId,params); 
			 return; 
		}
		
		string message = chat_params.param3;
		string prefix;
		string param0;
		string command;
		
		TStringArray tokens = new TStringArray;
		message.Replace("` ", "&SPCESC!");
		message.Split(" ", tokens); 
		int count = tokens.Count();

		string statname = message;
		statname.Replace("&SPCESC!", " ");
		statname.Replace("/stat", " ");
		statname.Replace(" ", "");
		
		string cmd_prefix = "/";
		param0 = tokens.Get(0);
		param0.ParseStringEx(prefix); 
		if(prefix != cmd_prefix) {
			super.OnEvent(eventTypeId,params); 
			 return; 
		};
		param0.ParseStringEx(command);
		string playerID = player.GetIdentity().GetPlainId();
		PlayerIdentity identity = player.GetIdentity();
		bool statExsit = false;
		
		switch(command) {
			case "humanity": {
				if (m_HeroesAndBanditsAllowHumanityCommand){
					GetRPCManager().SendRPC("HaB", "RPCSendHumanityNotification", new Param2< string, string >(playerID, command), false, identity);
				} else {
					super.OnEvent(eventTypeId,params); 
				}
				break;
			}
			case "stat": {
				if (m_HeroesAndBanditsAllowStatCommand){
					GetRPCManager().SendRPC("HaB", "RPCSendStatNotification", new Param3< string, string, string >(playerID, command, statname), false, identity);
				} else {
					super.OnEvent(eventTypeId,params); 
				}
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