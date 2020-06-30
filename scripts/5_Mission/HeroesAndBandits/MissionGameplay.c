ref HeroesAndBanditsPlayer 			g_HeroesAndBanditsPlayer;

ref habLevel 				g_HeroesAndBanditsLevel;
string 						g_HeroesAndBanditsGUIHeading;
bool 						g_HeroesAndBanditsHideKillsInGUI;
modded class MissionGameplay
{
	ref HeroesAndBanditsIconUI				m_HeroesAndBanditsIconUI;
	ref HeroesAndBanditsStatusBarIconUI		m_HeroesAndBanditsStatusBarIconUI;
	ref HeroesAndBanditsPanelUI				m_HeroesAndBanditsPanelUI;
	bool									m_HeroesAndBanditsIconsInitialized;
	string									m_HeroesAndBanditsCurrentIcon;
	bool 									m_HeroesAndBanditsShowLevelIcon;
	int 									m_HeroesAndBanditsIconLocation;
	string									m_HeroesAndBanditsCommandPrefix;
	bool 									m_HeroesAndBanditsAllowHumanityCommand;
	bool 									m_HeroesAndBanditsAllowStatCommand;
	bool 									m_HeroesAndBanditsAllowGUI;
	
	override void OnInit()
	{
		super.OnInit();
		m_HeroesAndBanditsIconsInitialized = false;
		GetRPCManager().AddRPC( "HaB", "RPCUpdateHABIcon", this, SingeplayerExecutionType.Both );
		GetRPCManager().AddRPC( "HaB", "RPCUpdateHABSettings", this, SingeplayerExecutionType.Both );
		GetRPCManager().AddRPC( "HaB", "RPCUpdateHABPlayerData", this, SingeplayerExecutionType.Both );
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
		Print("[HeroesAndBandits] [DebugClient] Received Settings");
		Param3< HeroesAndBanditsSettings, HeroesAndBanditsConfigActions, HeroesAndBanditsConfigLevels> data;
		if ( !ctx.Read( data ) ) return;
		g_HeroesAndBanditsSettings = data.param1;
		g_HeroesAndBanditsConfigActions = data.param2;
		g_HeroesAndBanditsConfigLevels = data.param3;
		Print("[HeroesAndBandits] [DebugClient] Settings Proccessed");
		g_HeroesAndBanditsGUIHeading = GetHeroesAndBanditsSettings().GUIHeading; // Will remove and replace just being lazy
		g_HeroesAndBanditsHideKillsInGUI = GetHeroesAndBanditsSettings().HideKillsInGUI;
		m_HeroesAndBanditsShowLevelIcon = GetHeroesAndBanditsLevels().ShowLevelIcon;
		m_HeroesAndBanditsIconLocation = GetHeroesAndBanditsLevels().LevelIconLocation;
		m_HeroesAndBanditsCommandPrefix = GetHeroesAndBanditsSettings().CommandPrefix;
		m_HeroesAndBanditsAllowHumanityCommand = GetHeroesAndBanditsSettings().AllowHumanityCommand;
		m_HeroesAndBanditsAllowStatCommand = GetHeroesAndBanditsSettings().AllowStatCommand;
		m_HeroesAndBanditsAllowGUI = GetHeroesAndBanditsSettings().AllowGUI;
		if ( GetHeroesAndBanditsLevels().ShowLevelIcon )
		{
			if ( !m_HeroesAndBanditsIconsInitialized )
			{
				m_HeroesAndBanditsIconsInitialized = true;
				Print("[HeroesAndBandits] [DebugClient] Settings Icons Initialized");
				InitHabIcon();
			} else {
				Print("[HeroesAndBandits] [DebugClient] Settings Icons Icon Updated");
				UpdateHABIcon();
			}
		}

	}
	
	
	void RPCUpdateHABPlayerData( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
		Print("[HeroesAndBandits] [DebugClient] Received Player Data");
		Param5< bool, string, bool, HeroesAndBanditsPlayer, habLevel > data;
		if ( !ctx.Read( data ) ) return;
			m_HeroesAndBanditsAllowGUI  = data.param1;
			g_HeroesAndBanditsGUIHeading  = data.param2;
			g_HeroesAndBanditsHideKillsInGUI = data.param3;
            g_HeroesAndBanditsPlayer = data.param4;
			g_HeroesAndBanditsLevel = data.param5;		
			string newIcon = g_HeroesAndBanditsLevel.LevelImage;
			string playerID = g_HeroesAndBanditsPlayer.PlayerID;
			Print("[HeroesAndBandits] [DebugClient] Player Data Proccessed Icon: " + m_HeroesAndBanditsCurrentIcon);
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
	
	
	void RPCUpdateHABIcon( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
		Param2< string, string > data  //Player ID, Icon
		if ( !ctx.Read( data ) ) return;
		string newIcon = data.param2;
		string playerID = data.param1;
		Print("[HeroesAndBandits] [DebugClient] " + playerID + " received icon " + newIcon);
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
		Print("[HeroesAndBandits] [DebugClient] Updating Icon");
		
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
		string statname;
		
		Print("[HeroesAndBandits] [DebugClient] Message: " + message);
		
		TStringArray tokens = new TStringArray;
		message.Replace("  ", " ");
		message.Split(" ", tokens);

		Print("[HeroesAndBandits] [DebugClient] Message: " + message);
		string cmd_prefix = "/";
		if ( !m_HeroesAndBanditsCommandPrefix ){
		} else {
			cmd_prefix = m_HeroesAndBanditsCommandPrefix;
		}

		param0 = tokens.Get(0);
		param0.ParseStringEx(prefix); 
		if(prefix != cmd_prefix) {
			super.OnEvent(eventTypeId,params); 
			 return; 
		} else {
			command = param0;
			command.Replace(cmd_prefix, "");
			command.Replace(" ", "");
			command.ToLower();
		}
		
		if (tokens.Count() > 1 )
		{
			statname = tokens.Get(1);
			statname.Replace(" ", "");
			statname.ToLower();
		}
		bool commandNotSentToServer = true;
		Print("[HeroesAndBandits] [DebugClient] Command: " + command);
		switch(command) {
			case "humanity":
			case "humanité":
			case "pепутация":
			case "menschheit":
			case "humanitarność": {
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
	override void OnUpdate (float timeslice) {
        super.OnUpdate (timeslice);

        Input input = GetGame().GetInput();
        if (input.LocalPress("UAUIBack", false)) {
            if (m_HeroesAndBanditsPanelUI != NULL && GetGame().GetUIManager().GetMenu() == m_HeroesAndBanditsPanelUI) {
                HeroesAndBanditsClosePanel();
            }
        }

        if (input.LocalPress("UAHeroesAndBanditsPanel", false)) {
            if (m_HeroesAndBanditsPanelUI) {
                if (m_HeroesAndBanditsPanelUI.IsOpen()) {
					HeroesAndBanditsClosePanel();
                } else if (GetGame().GetUIManager().GetMenu() == NULL) {
					GetRPCManager().SendRPC("HaB", "RPCRequestHABPlayerData", NULL, true);
					//Wait a bit before opening so that way player data is received
					GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLaterByName(this, "HeroesAndBanditsOpenPanel", 400, false);
                }
            } else if (GetGame().GetUIManager().GetMenu() == NULL && m_HeroesAndBanditsPanelUI == null) {
				GetRPCManager().SendRPC("HaB", "RPCRequestHABPlayerData", NULL, true);
				//Wait a bit before opening so that way player data is received
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLaterByName(this, "HeroesAndBanditsCreatePanel", 400, false);
            }
        }
    }
		
	void HeroesAndBanditsCreatePanel()
	{
		if(m_HeroesAndBanditsAllowGUI)
		{
	    	HeroesAndBanditsLockControls();
	        m_HeroesAndBanditsPanelUI = HeroesAndBanditsPanelUI.Cast(GetUIManager().EnterScriptedMenu(HEROESANDBANDITS_PANEL_MENU, null));
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLaterByName(m_HeroesAndBanditsPanelUI, "updateData", 500, false);
	        m_HeroesAndBanditsPanelUI.SetOpen(true);
		}
	}
	
	void HeroesAndBanditsOpenPanel()
	{	
		if(m_HeroesAndBanditsAllowGUI)
		{
			GetGame().GetUIManager().ShowScriptedMenu(m_HeroesAndBanditsPanelUI, NULL);
	        m_HeroesAndBanditsPanelUI.SetOpen(true);
	    	HeroesAndBanditsLockControls();
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLaterByName(m_HeroesAndBanditsPanelUI, "updateData", 500, false);
		}
	}
	
	void HeroesAndBanditsClosePanel()
	{
		m_HeroesAndBanditsPanelUI.SetOpen(false);
        GetGame().GetUIManager().HideScriptedMenu(m_HeroesAndBanditsPanelUI);
		HeroesAndBanditsUnLockControls();
	}
	
	private void HeroesAndBanditsLockControls() {
        GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_MOUSE_ALL);
        GetGame().GetUIManager().ShowUICursor(true);
    }

    private void HeroesAndBanditsUnLockControls() {
        GetGame().GetMission().PlayerControlEnable(false);
        GetGame().GetInput().ResetGameFocus();
        GetGame().GetUIManager().ShowUICursor(false);
    }
}