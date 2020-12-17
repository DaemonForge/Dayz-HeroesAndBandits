ref habLevel 						g_HeroesAndBanditsLevel;

modded class MissionGameplay extends MissionBase
{
	ref HeroesAndBanditsIconUI				m_HeroesAndBanditsIconUI;
	ref HeroesAndBanditsStatusBarIconUI		m_HeroesAndBanditsStatusBarIconUI;
	ref HeroesAndBanditsPanelUI				m_HeroesAndBanditsPanelUI;
	bool									m_HeroesAndBanditsIconsInitialized;
	bool									m_HeroesAndBanditsPanelOpening = false;
	string									m_HeroesAndBanditsCurrentIcon;
	
	void MissionGameplay()
	{
		m_HeroesAndBanditsIconsInitialized = false;
		GetRPCManager().AddRPC( "HaB", "RPCUpdateHABIcon", this, SingeplayerExecutionType.Both );
		GetRPCManager().AddRPC( "HaB", "RPCUpdateHABSettings", this, SingeplayerExecutionType.Both );
		GetRPCManager().AddRPC( "HaB", "RPCUpdateHABPlayerData", this, SingeplayerExecutionType.Both );
		GetRPCManager().AddRPC( "HaB", "RPCPlayGunShotSound", this, SingeplayerExecutionType.Both );
	}
	
	void InitHabIcon(){
		if ( GetHeroesAndBanditsLevels().LevelIconLocation == 1 ||  GetHeroesAndBanditsLevels().LevelIconLocation == 3  )
		{
			m_HeroesAndBanditsIconsInitialized = true;
			m_HeroesAndBanditsIconUI = new ref HeroesAndBanditsIconUI;
			m_HeroesAndBanditsIconUI.Init();
		} 
		if ( GetHeroesAndBanditsLevels().LevelIconLocation == 2 ||  GetHeroesAndBanditsLevels().LevelIconLocation == 3  ) {
			m_HeroesAndBanditsIconsInitialized = true;
			m_HeroesAndBanditsStatusBarIconUI = new ref HeroesAndBanditsStatusBarIconUI;
			m_HeroesAndBanditsStatusBarIconUI.Init();
		}
		UpdateHABIcon();
	}
	
	void RPCUpdateHABSettings( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
		Print("[HeroesAndBandits] [DebugClient] Received Settings");
		Param4< HeroesAndBanditsSettings, HeroesAndBanditsConfigActions, HeroesAndBanditsConfigLevels, HeroesAndBanditsPlayer> data;
		if ( !ctx.Read( data ) ) return;
		g_HeroesAndBanditsSettings = data.param1;
		g_HeroesAndBanditsConfigActions = data.param2;
		g_HeroesAndBanditsConfigLevels = data.param3;
		g_HeroesAndBanditsPlayer = data.param4;
		g_HeroesAndBanditsLevel = g_HeroesAndBanditsPlayer.getLevel();
		m_HeroesAndBanditsCurrentIcon = g_HeroesAndBanditsPlayer.getLevel().LevelImage;
		Print("[HeroesAndBandits] [DebugClient] Settings Proccessed");
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
	
	void RPCPlayGunShotSound( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
		Param2< string, vector > data;
		if ( !ctx.Read( data ) ) return;
		string GunShot_Sound = data.param1;	
		vector GunShot_Position = data.param2;
		SEffectManager.PlaySound( GunShot_Sound, GunShot_Position);
	}
	
	void RPCUpdateHABPlayerData( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
		Print("[HeroesAndBandits] [DebugClient] Received Player Data");
		Param1< HeroesAndBanditsPlayer > data;
		if ( !ctx.Read( data ) ) return;
            g_HeroesAndBanditsPlayer = data.param1;
			g_HeroesAndBanditsLevel = g_HeroesAndBanditsPlayer.getLevel();		
			string newIcon = g_HeroesAndBanditsLevel.LevelImage;
			string playerID = g_HeroesAndBanditsPlayer.PlayerID;
			Print("[HeroesAndBandits] [DebugClient] Player Data Proccessed Icon: " + m_HeroesAndBanditsCurrentIcon);
			if ( !newIcon || !playerID )
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
			if ( !newIcon || !playerID || !GetHeroesAndBanditsLevels().ShowLevelIcon)
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
		
		if ( !GetHeroesAndBanditsLevels().ShowLevelIcon || !m_HeroesAndBanditsIconUI ) 
		{	
		} else {
			m_HeroesAndBanditsIconUI.updateIcon(m_HeroesAndBanditsCurrentIcon);
		}
		if ( !GetHeroesAndBanditsLevels().ShowLevelIcon || !m_HeroesAndBanditsStatusBarIconUI ) 
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
		string ammount;
		int ammountInt;
		
		Print("[HeroesAndBandits] [DebugClient] Message: " + message);
		
		TStringArray tokens = new TStringArray;
		message.Replace("  ", " ");
		message.Split(" ", tokens);

		Print("[HeroesAndBandits] [DebugClient] Message: " + message);
		string cmd_prefix = "/";
		if ( !GetHeroesAndBanditsSettings().CommandPrefix ){
		} else {
			cmd_prefix = GetHeroesAndBanditsSettings().CommandPrefix;
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
		
		if (tokens.Count() > 2)
		{
			ammount = tokens.Get(2);
			ammount.Replace(" ", "");
			ammount.ToLower();
			ammountInt = ammount.ToInt();
		}
		bool commandNotSentToServer = true;
		Print("[HeroesAndBandits] [DebugClient] Command: " + command);
		switch(command) {
			case "humanity":
			case "humanité":
			case "pепутация":
			case "menschheit":
			case "humanitarność": {
					if (GetHeroesAndBanditsSettings().AllowHumanityCommand){
						commandNotSentToServer = false;
						habPrint("Requesting Humanity from server", "Debug");
						GetRPCManager().SendRPC("HaB", "RPCSendHumanityNotification", new Param1< string >(command), false);
					}
					break;
				}
			case "stat": {
				if (GetHeroesAndBanditsSettings().AllowStatCommand){
					commandNotSentToServer = false;
					habPrint("Requesting Stat from server", "Debug");
					GetRPCManager().SendRPC("HaB", "RPCSendStatNotification", new Param2< string, string >( command, statname), false);
				}
				break;
			}
			case "add": {//Adding so debuggin is easier :)
				if ( GetHeroesAndBanditsSettings().DebugCommand && ammountInt){
					commandNotSentToServer = false;
					habPrint("Requesting to add " + statname + " from server", "Debug");
					GetRPCManager().SendRPC("HaB", "RPCSendAffinityUpdate", new Param3< string, string, int >( command, statname, ammountInt), false);
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
        super.OnUpdate(timeslice);

        Input input = GetGame().GetInput();
        if (input.LocalPress("UAUIBack", false)) {
            if (m_HeroesAndBanditsPanelUI != NULL && GetGame().GetUIManager().GetMenu() == m_HeroesAndBanditsPanelUI) {
                HeroesAndBanditsClosePanel();
				m_HeroesAndBanditsPanelOpening = false;
            }
        }

        if (input.LocalPress("UAHeroesAndBanditsPanel", false)) {
            if (m_HeroesAndBanditsPanelUI) {
                if (m_HeroesAndBanditsPanelUI.IsOpen()) {
					HeroesAndBanditsClosePanel();
					m_HeroesAndBanditsPanelOpening = false;
                } else if (GetGame().GetUIManager().GetMenu() == NULL) {
					GetRPCManager().SendRPC("HaB", "RPCRequestHABPlayerData", NULL, true);
					//Wait a bit before opening so that way player data is received
					if (!m_HeroesAndBanditsPanelOpening){
						GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLaterByName(this, "HeroesAndBanditsOpenPanel", 400, false);
					}
					m_HeroesAndBanditsPanelOpening = true;
                }
            } else if (GetGame().GetUIManager().GetMenu() == NULL && m_HeroesAndBanditsPanelUI == null) {
				GetRPCManager().SendRPC("HaB", "RPCRequestHABPlayerData", NULL, true);
				//Wait a bit before opening so that way player data is received
				if (!m_HeroesAndBanditsPanelOpening){
					GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLaterByName(this, "HeroesAndBanditsCreatePanel", 400, false);
				}
				m_HeroesAndBanditsPanelOpening = true;
            }
        }
    }
		
	void HeroesAndBanditsCreatePanel()
	{
		if(GetHeroesAndBanditsSettings().AllowGUI)
		{
	    	HeroesAndBanditsLockControls();
	        m_HeroesAndBanditsPanelUI = HeroesAndBanditsPanelUI.Cast(GetUIManager().EnterScriptedMenu(HEROESANDBANDITS_PANEL_MENU, null));
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLaterByName(m_HeroesAndBanditsPanelUI, "updateData", 500, false);
	        m_HeroesAndBanditsPanelUI.SetOpen(true);
			m_HeroesAndBanditsPanelOpening = false;
		}
	}
	
	void HeroesAndBanditsOpenPanel()
	{	
		if(GetHeroesAndBanditsSettings().AllowGUI)
		{
			GetGame().GetUIManager().ShowScriptedMenu(m_HeroesAndBanditsPanelUI, NULL);
	        m_HeroesAndBanditsPanelUI.SetOpen(true);
	    	HeroesAndBanditsLockControls();
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLaterByName(m_HeroesAndBanditsPanelUI, "updateData", 500, false);
			m_HeroesAndBanditsPanelOpening = false;
		}
	}
	
	void HeroesAndBanditsClosePanel()
	{
		m_HeroesAndBanditsPanelUI.SetOpen(false);
        GetGame().GetUIManager().HideScriptedMenu(m_HeroesAndBanditsPanelUI);
		HeroesAndBanditsUnLockControls();
		m_HeroesAndBanditsPanelOpening = false;
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
};
