modded class MissionGameplay
{
	ref HeroesAndBanditsIconUI				m_HeroesAndBanditsIconUI;
	ref HeroesAndBanditsStatusBarIconUI		m_HeroesAndBanditsStatusBarIconUI;
	bool									m_HeroesAndBanditsIconsInitialized;
	string									m_HeroesAndBanditsCurrentIcon;
	bool 									m_HeroesAndBanditsShowLevelIcon;
	int 									m_HeroesAndBanditsIconLocation;
	bool 									m_HeroesAndBanditsAllowHumanityCommand;
	bool 									m_HeroesAndBanditsAllowStatCommand;
	
	override void OnInit()
	{
		super.OnInit();
		m_HeroesAndBanditsIconsInitialized = false;
		GetRPCManager().AddRPC( "HaB", "RPCUpdateHABIcon", this, SingeplayerExecutionType.Both );
		GetRPCManager().AddRPC( "HaB", "RPCUpdateHABSettings", this, SingeplayerExecutionType.Both );
	}
	
	
	void InitHabIcon(){
		if ( m_HeroesAndBanditsIconLocation == 1 ||  m_HeroesAndBanditsIconLocation == 3  )
		{
			m_HeroesAndBanditsIconUI = new ref HeroesAndBanditsIconUI;
			m_HeroesAndBanditsIconUI.Init();
		} 
		if ( m_HeroesAndBanditsIconLocation == 2 ||  m_HeroesAndBanditsIconLocation == 3  ) {
			m_HeroesAndBanditsStatusBarIconUI = new ref HeroesAndBanditsStatusBarIconUI;
			m_HeroesAndBanditsStatusBarIconUI.Init();
		}
		UpdateHABIcon();
	}
	
	
	
	void RPCUpdateHABSettings( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
		//Print("[HeroesAndBandits] [DebugClient] Received Settings");
		Param5< bool, bool, bool, string, int > data;
		if ( !ctx.Read( data ) ) return;
		m_HeroesAndBanditsShowLevelIcon = data.param1;
		m_HeroesAndBanditsAllowHumanityCommand = data.param2;
		m_HeroesAndBanditsAllowStatCommand = data.param3;
		m_HeroesAndBanditsCurrentIcon = data.param4;
		m_HeroesAndBanditsIconLocation = data.param5;
		//Print("[HeroesAndBandits] [DebugClient] Settings Proccessed - ShowLevelIcon:" + m_HeroesAndBanditsShowLevelIcon + " AllowHumanityCommand:" + m_HeroesAndBanditsAllowHumanityCommand + " AllowStatCommand:" + m_HeroesAndBanditsAllowStatCommand + " CurrentIcon:" + m_HeroesAndBanditsCurrentIcon + " IconLocation:" + m_HeroesAndBanditsIconLocation);
		if ( m_HeroesAndBanditsShowLevelIcon )
		{
			if ( !m_HeroesAndBanditsIconsInitialized )
			{
				m_HeroesAndBanditsIconsInitialized = true;
				//Print("[HeroesAndBandits] [DebugClient] Settings Icons Initialized");
				InitHabIcon();
			} else {
				//Print("[HeroesAndBandits] [DebugClient] Settings Icons Icon Updated");
				UpdateHABIcon();
			}
		}

	}
	
	
	void RPCUpdateHABIcon( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
		Param2< string, string > data  //Player ID, Icon
		if ( !ctx.Read( data ) ) return;
		string newIcon = data.param2;
		string playerID = data.param1;
		//Print("[HeroesAndBandits] [DebugClient] " + playerID + " received icon " + newIcon);
			if ( !newIcon || !playerID || !m_HeroesAndBanditsShowLevelIcon)
			{
				return;
			}
			if ( newIcon != m_HeroesAndBanditsCurrentIcon)
			{
				m_HeroesAndBanditsCurrentIcon = newIcon;
				GetGame().GetCallQueue( CALL_CATEGORY_GUI ).CallLaterByName(this, "UpdateHABIcon", 500, false);
			}
	}
	
	void UpdateHABIcon(){
		//Print("[HeroesAndBandits] [DebugClient] Updating Icon");
		
		if ( !m_HeroesAndBanditsShowLevelIcon || !m_HeroesAndBanditsIconUI ) 
		{	
		} else {
			m_HeroesAndBanditsIconUI.updateIcon(m_HeroesAndBanditsCurrentIcon);
		}
		if ( !m_HeroesAndBanditsShowLevelIcon || !m_HeroesAndBanditsStatusBarIconUI ) 
		{
		} else {
			m_HeroesAndBanditsStatusBarIconUI.updateIcon(m_HeroesAndBanditsCurrentIcon);
		}
	}
	
	
	
	
	
		
	
	override void OnEvent(EventType eventTypeId, Param params) 
	{
		
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
		
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());		
		
		if (!player) { 
			super.OnEvent(eventTypeId,params); 
			 return; 
		}
		
		string message = chat_params.param3;
		string prefix;
		string param0;
		string command;
		
		//Print("[HeroesAndBandits] [DebugClient] Message: " + message);
		
		TStringArray tokens = new TStringArray;
		message.Split(" ", tokens); 
		int count = tokens.Count();

		//Print("[HeroesAndBandits] [DebugClient] Message: " + message);
		
		string statname = message;
		statname.Replace("/stat", " ");
		statname.Replace(" ", "");
		
		string cmd_prefix = "/";
		param0 = tokens.Get(0);
		param0.ParseStringEx(prefix); 
		if(prefix != cmd_prefix) {
			super.OnEvent(eventTypeId,params); 
			 return; 
		} else {
			command = message;
			command.Replace(cmd_prefix, "");
			command.Replace(" ", "");
		}
		bool commandNotSentToServer = true;
		//Print("[HeroesAndBandits] [DebugClient] Command: " + command);
		switch(command) {
			case "humanity":
			case "humanité":
			case "pепутация":
			case "menschheit":
			case "humanitarność":
			case "Humanity":
			case "Humanité":
			case "Pепутация":
			case "Menschheit":
			case "Humanitarność": {
					if (m_HeroesAndBanditsAllowHumanityCommand){
						commandNotSentToServer = false;
						Print("[HeroesAndBandits] [DebugClient] Requesting Humanity from server");
						GetRPCManager().SendRPC("HaB", "RPCSendHumanityNotification", new Param1< string >(command), false);
					}
					break;
				}
			case "stat": {
				if (m_HeroesAndBanditsAllowStatCommand){
					commandNotSentToServer = false;
					Print("[HeroesAndBandits] [DebugClient] Requesting Stat from server");
					GetRPCManager().SendRPC("HaB", "RPCSendStatNotification", new Param2< string, string >( command, statname), false);
				}
				break;
			}
			default: {
				commandNotSentToServer = true;
				break;
			}
		}
		if ( commandNotSentToServer )
		{
			super.OnEvent(eventTypeId,params); 
		}
	}
}