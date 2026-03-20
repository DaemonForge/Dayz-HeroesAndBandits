class HAB_MainPanel extends UIScriptedMenu {
	protected static string m_LayoutPath = "HeroesAndBandits/gui/layouts/HumanityPanel.layout";
	
	protected autoptr HAB_PageBase m_CurPage;
	
	protected autoptr ButtonWidget m_LeaderboardsButton;
	protected autoptr ButtonWidget m_StatsButton;
	protected autoptr ButtonWidget m_InfoButton;
	protected Widget m_StatsButton_Active;
	protected Widget m_LeaderboardsButton_Active;
	protected Widget m_InfoButton_Active;
	protected autoptr Widget m_Page;
	protected PlayerBase m_player;
	
	// Tab state: 0=Stats, 1=Leaderboards, 2=Info
	protected int m_ActiveTab;
	
	// Config accent color (cached)
	protected int m_AccentColor;
	
	override Widget Init() {
		layoutRoot = Widget.Cast(g_Game.GetWorkspace().CreateWidgets(m_LayoutPath));
		m_LeaderboardsButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("LeaderboardsButton"));
		m_StatsButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("StatsButton"));
		m_InfoButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("InfoButton"));
		m_StatsButton_Active = layoutRoot.FindAnyWidget("StatsButton_Active");
		m_LeaderboardsButton_Active = layoutRoot.FindAnyWidget("LeaderboardsButton_Active");
		m_InfoButton_Active = layoutRoot.FindAnyWidget("InfoButton_Active");
		m_Page = Widget.Cast(layoutRoot.FindAnyWidget("Page"));
        g_Game.GetMission().PlayerControlDisable(INPUT_EXCLUDE_MOUSE_ALL);
        g_Game.GetUIManager().ShowUICursor(true);
		g_Game.GetMission().GetHud().Show(false);
    	PPEffects.SetBlurInventory(0.5);
		if (Class.CastTo(m_player,g_Game.GetPlayer())){
			m_player.RefreshHABData();		
			m_player.RequestHABControllerData();
		}
		
		// Cache accent color from config
		HABPanelConfig config = HABPanelConfig.Get();
		if (config)
		{
			m_AccentColor = config.GetAccentColor();
		}
		else
		{
			m_AccentColor = ARGB(255, 217, 166, 77); // Default gold
		}
		
		// Apply accent color to panel widgets
		ApplyAccentColor(layoutRoot);
		
		m_CurPage = new HAB_StatsPage(m_Page, this, m_player);
		m_ActiveTab = 0;
		UpdateTabState();
		return layoutRoot;
	}
	
	int GetAccentColor()
	{
		return m_AccentColor;
	}
	
	void ApplyAccentColor(Widget root)
	{
		if (!root) return;
		
		// Apply to specific accent widgets in the panel
		Widget panelAccent = root.FindAnyWidget("PanelAccent");
		if (panelAccent) panelAccent.SetColor(m_AccentColor);
		
		// Tab active indicators
		if (m_StatsButton_Active) m_StatsButton_Active.SetColor(m_AccentColor);
		if (m_LeaderboardsButton_Active) m_LeaderboardsButton_Active.SetColor(m_AccentColor);
		if (m_InfoButton_Active) m_InfoButton_Active.SetColor(m_AccentColor);
	}
	
	void UpdateTabState(){
		if (m_StatsButton_Active)
			m_StatsButton_Active.Show(m_ActiveTab == 0);
		if (m_LeaderboardsButton_Active)
			m_LeaderboardsButton_Active.Show(m_ActiveTab == 1);
		if (m_InfoButton_Active)
			m_InfoButton_Active.Show(m_ActiveTab == 2);
	}
	
	override bool OnClick(Widget w, int x, int y, int button){
		
		if (w == m_LeaderboardsButton && m_ActiveTab != 1){
			delete m_CurPage;
			m_CurPage = new HAB_LeaderboardsPage(m_Page,this,m_player);
			m_ActiveTab = 1;
			UpdateTabState();
		}
		if (w == m_StatsButton && m_ActiveTab != 0){
			delete m_CurPage;
			m_CurPage = new HAB_StatsPage(m_Page,this,m_player);
			m_ActiveTab = 0;
			UpdateTabState();
		}
		if (w == m_InfoButton && m_ActiveTab != 2){
			delete m_CurPage;
			m_CurPage = new HAB_InfoPage(m_Page,this,m_player);
			m_ActiveTab = 2;
			UpdateTabState();
		}
		
		return super.OnClick(w,x,y,button);
	}
	
	override void Update(float timeslice) {
		super.Update(timeslice);
		if (m_CurPage)
			m_CurPage.MyUpdate(timeslice);
	}
	
	
	
	void ~HAB_MainPanel() {
        g_Game.GetMission().PlayerControlEnable(false);
        g_Game.GetInput().ResetGameFocus();
        g_Game.GetUIManager().ShowUICursor(false);
		g_Game.GetMission().GetHud().Show(true);
    	PPEffects.SetBlurInventory(0);
	}
	
}

class HAB_PageBase extends ScriptedWidgetEventHandler {
	HAB_MainPanel m_panel;
	protected PlayerBase m_player;
	protected Widget layoutRoot;
	void HAB_PageBase(Widget parent, HAB_MainPanel panel, PlayerBase player){
		
	}
	
	void ~HAB_PageBase(){
		layoutRoot.Show(false);
		layoutRoot.Unlink();
	}
	
	int GetAccentColor()
	{
		if (m_panel)
			return m_panel.GetAccentColor();
		return ARGB(255, 217, 166, 77); // Default gold
	}
	
	void ApplyAccentToWidget(string widgetName)
	{
		Widget w = layoutRoot.FindAnyWidget(widgetName);
		if (w) w.SetColor(GetAccentColor());
	}
	
	void MyUpdate(float timeslice){
		
	}
}

class HAB_StatsPage extends HAB_PageBase {
	protected static string m_LayoutPath = "HeroesAndBandits/gui/layouts/HumanityPage.layout";
	
	protected ImageWidget m_Icon;
	protected Widget m_IconBackground;
	protected TextWidget m_LevelText;
	protected TextWidget m_HumanityText;
	
	protected TextWidget m_LowText;
	protected TextWidget m_HighText;
	protected ProgressBarWidget m_PrimaryProgress;
	protected ProgressBarWidget m_BanditProgress;
	protected ProgressBarWidget m_HeroProgress;
	protected Widget m_ProgressCenter;
	
	protected Widget m_PathContainer;
	protected Widget m_PathAccent;
	protected Widget m_PathGlow;
	protected TextWidget m_PathText;
	protected ButtonWidget m_PathDiscordBtn;
	protected int m_PathState; // 0=hidden, 1=no discord, 2=no path, 3=hero, 4=bandit
	protected string m_DiscordLink;
	
	protected Widget m_StatsGrid;
	protected autoptr array<autoptr HAB_StatWidget> m_StatWidgets;
	protected bool m_StatsLoaded;
	
	void HAB_StatsPage(Widget parent, HAB_MainPanel panel, PlayerBase player){
		m_panel = panel;
		layoutRoot = Widget.Cast(g_Game.GetWorkspace().CreateWidgets(m_LayoutPath,parent));
		Class.CastTo(m_player,player);
		
		// Initialize stat widgets array
		m_StatWidgets = new array<autoptr HAB_StatWidget>;
		
		m_Icon = ImageWidget.Cast(layoutRoot.FindAnyWidget("Icon"));
		m_IconBackground = layoutRoot.FindAnyWidget("IconBackground");
		m_LevelText = TextWidget.Cast(layoutRoot.FindAnyWidget("LevelText"));
		m_HumanityText = TextWidget.Cast(layoutRoot.FindAnyWidget("HumanityText"));
	
		m_LowText = TextWidget.Cast(layoutRoot.FindAnyWidget("LowText"));
		m_HighText = TextWidget.Cast(layoutRoot.FindAnyWidget("HighText"));
		m_PrimaryProgress = ProgressBarWidget.Cast(layoutRoot.FindAnyWidget("PrimaryProgress"));
		m_BanditProgress = ProgressBarWidget.Cast(layoutRoot.FindAnyWidget("BanditProgress"));
		m_HeroProgress = ProgressBarWidget.Cast(layoutRoot.FindAnyWidget("HeroProgress"));
		m_ProgressCenter = layoutRoot.FindAnyWidget("ProgressCenter");
		
		m_PathContainer = layoutRoot.FindAnyWidget("PathContainer");
		m_PathAccent = layoutRoot.FindAnyWidget("PathAccent");
		m_PathGlow = layoutRoot.FindAnyWidget("PathGlow");
		m_PathText = TextWidget.Cast(layoutRoot.FindAnyWidget("PathText"));
		m_PathDiscordBtn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("PathDiscordBtn"));
		
		// Get Discord link from config
		HABPanelConfig panelConfig = HABPanelConfig.Get();
		if (panelConfig && panelConfig.DiscordLink != ""){
			m_DiscordLink = panelConfig.DiscordLink;
		}
	
		m_StatsGrid = Widget.Cast(layoutRoot.FindAnyWidget("StatsGrid"));
		layoutRoot.SetHandler(this);
		
		// Apply accent colors from config
		ApplyAccentToWidget("CardAccent");
		ApplyAccentToWidget("StatsAccent");
		
		UpdateData();
		UpdatePathDisplay();
	}
	
	override bool OnClick(Widget w, int x, int y, int button){
		// Handle Discord button click
		if (w == m_PathDiscordBtn && m_DiscordLink != ""){
			GetGame().OpenURL(m_DiscordLink);
			return true;
		}
		return false;
	}
	
	protected float m_time;
	protected static const float UPDATE_INTERVAL = 2.5; // Seconds between UI updates
	
	override void MyUpdate(float timeslice){
		super.MyUpdate(timeslice);
		m_time += timeslice;
		
		if (m_time > UPDATE_INTERVAL){
			m_time = 0;
			UpdateData();
		}
	}
	
	void UpdateData(){
		string clientIcon = m_player.GetClientIcon();
		if (clientIcon != "")
		{
			m_Icon.LoadImageFile(0, clientIcon);
		}
		if (Math.AbsInt(m_player.HABLevel()) > 0){
			m_LevelText.SetText(m_player.GetClientAffinityName() + " #HAB_LEVEL " + Math.AbsInt(m_player.HABLevel()).ToString());
		} else {
			m_LevelText.SetText(m_player.GetClientAffinityName());
		}
		int humanity = m_player.Humanity();
		UpdateIconBackground(humanity);
		UpdateStats();
		m_HumanityText.SetText("#HAB_HUMANITY: " + UUtil.ConvertIntToNiceString(humanity));
		HABControllerData cData = m_player.GetHABMetaData();
		if (cData){
			m_LowText.SetText(UUtil.ConvertIntToNiceString(cData.param1));
			m_HighText.SetText(UUtil.ConvertIntToNiceString(cData.param2));
			float percent = 0;
			if (m_player.HABLevel() == 0){
				m_PrimaryProgress.Show(false);
				m_BanditProgress.Show(true);
				m_HeroProgress.Show(true);
				if (m_ProgressCenter) m_ProgressCenter.Show(true);
				if (m_player.Humanity() < 0){
					percent = Math.AbsFloat(m_player.Humanity()) / Math.AbsFloat(cData.param2) * 100;
					m_BanditProgress.SetCurrent(percent);
				} else {
					percent = Math.AbsFloat(m_player.Humanity()) / Math.AbsFloat(cData.param1) * 100;
					m_HeroProgress.SetCurrent(percent);
				}
			} else {
				m_PrimaryProgress.Show(true);
				m_BanditProgress.Show(false);
				m_HeroProgress.Show(false);
				if (m_ProgressCenter) m_ProgressCenter.Show(false);
				float value = Math.AbsFloat(m_player.Humanity()) - Math.AbsFloat(cData.param1);
				percent = value / (Math.AbsFloat(cData.param2) - Math.AbsFloat(cData.param1)) * 100;
				m_PrimaryProgress.SetCurrent(percent);
			}
		}
	}
	
	void UpdateIconBackground(int humanity)
	{
		if (!m_IconBackground)
			return;
		
		int level = m_player.HABLevel();
		int color;
		int progressColor;
		
		if (humanity < 0)
		{
			// Bandit - soft red
			color = ARGB(77, 204, 64, 51);
			progressColor = ARGB(255, 204, 64, 51);
		}
		else if (humanity > 0 && level > 0)
		{
			// Hero - soft blue
			color = ARGB(77, 51, 115, 204);
			progressColor = ARGB(255, 51, 115, 204);
		}
		else
		{
			// Bambi/Neutral - soft green
			color = ARGB(77, 64, 128, 64);
			progressColor = ARGB(255, 64, 128, 64);
		}
		
		m_IconBackground.SetColor(color);
		
		if (m_PrimaryProgress)
			m_PrimaryProgress.SetColor(progressColor);
	}
	
	void UpdateStats()
	{
		if (m_StatsLoaded)
			return;
			
		if (!m_player || !m_player.HABData())
			return;
			
		array<autoptr HeroesAndBanditsStats> allStats = m_player.HABData().GetAllStats();
		if (!allStats || allStats.Count() == 0)
			return;
		
		if (!m_StatWidgets)
			m_StatWidgets = new array<autoptr HAB_StatWidget>;
		
		int accentColor = GetAccentColor();
		for (int i = 0; i < allStats.Count(); i++)
		{
			HeroesAndBanditsStats stat = HeroesAndBanditsStats.Cast(allStats.Get(i));
			if (stat)
			{
				m_StatWidgets.Insert(new HAB_StatWidget(m_StatsGrid, stat.m_Stat, stat.m_Value, accentColor));
			}
		}
		
		m_StatsLoaded = true;
	}
	
	void UpdatePathDisplay()
	{
		if (!m_PathContainer || !m_PathText) return;
		
		// Only show path section for Bambis (level 0)
		if (m_player.HABLevel() != 0){
			m_PathContainer.Show(false);
			return;
		}
		
		// Check if path roles are configured (not default "DISCORDROLEID" or blank)
		bool pathsConfigured = false;
		if (HEROPATHROLE != "" && HEROPATHROLE != "DISCORDROLEID"){
			pathsConfigured = true;
		}
		if (BANDITPATHROLE != "" && BANDITPATHROLE != "DISCORDROLEID"){
			pathsConfigured = true;
		}
		
		// If paths not configured, hide the section
		if (!pathsConfigured){
			m_PathContainer.Show(false);
			m_PathState = 0;
			return;
		}
		
		// Show the path container
		m_PathContainer.Show(true);
		
		// Check Discord status
		UDiscordUser dsuser = m_player.DiscordUser();
		if (!dsuser){
			// Discord not connected - state 1 (show Discord button)
			m_PathState = 1;
			m_PathText.SetText("#HAB_PATH_CONNECT_DISCORD");
			m_PathText.SetColor(ARGB(255, 230, 180, 80));
			if (m_PathAccent) m_PathAccent.SetColor(ARGB(255, 230, 180, 80));
			if (m_PathGlow) m_PathGlow.SetColor(ARGB(40, 230, 180, 80));
			if (m_PathDiscordBtn) m_PathDiscordBtn.Show(true);
			return;
		}
		
		// Check which path they have
		if (dsuser.HasRole(HEROPATHROLE)){
			// Hero path - state 3 (hide Discord button)
			m_PathState = 3;
			m_PathText.SetText("#HAB_PATH_HERO");
			m_PathText.SetColor(ARGB(255, 80, 140, 220));
			if (m_PathAccent) m_PathAccent.SetColor(ARGB(255, 80, 140, 220));
			if (m_PathGlow) m_PathGlow.SetColor(ARGB(50, 80, 140, 220));
			if (m_PathDiscordBtn) m_PathDiscordBtn.Show(false);
			return;
		}
		
		if (dsuser.HasRole(BANDITPATHROLE)){
			// Bandit path - state 4 (hide Discord button)
			m_PathState = 4;
			m_PathText.SetText("#HAB_PATH_BANDIT");
			m_PathText.SetColor(ARGB(255, 220, 80, 70));
			if (m_PathAccent) m_PathAccent.SetColor(ARGB(255, 220, 80, 70));
			if (m_PathGlow) m_PathGlow.SetColor(ARGB(50, 220, 80, 70));
			if (m_PathDiscordBtn) m_PathDiscordBtn.Show(false);
			return;
		}
		
		// No path chosen yet - state 2 (show Discord button)
		m_PathState = 2;
		m_PathText.SetText("#HAB_PATH_NONE");
		m_PathText.SetColor(ARGB(255, 255, 200, 80));
		if (m_PathAccent) m_PathAccent.SetColor(ARGB(255, 255, 200, 80));
		if (m_PathGlow) m_PathGlow.SetColor(ARGB(40, 255, 200, 80));
		if (m_PathDiscordBtn) m_PathDiscordBtn.Show(true);
	}
	void ~HAB_StatsPage(){
		if (m_StatWidgets){
			m_StatWidgets.Clear();
		}
	}
}

class HAB_LeaderboardsPage extends HAB_PageBase {
	protected static string m_LayoutPath = "HeroesAndBandits/gui/layouts/LeaderboardsPage.layout";
	
	protected Widget m_HeroGrid;
	protected Widget m_BanditGrid;
	protected TextWidget m_HeroRankText;
	protected TextWidget m_BanditRankText;
	
	protected int m_HEROLeaderboardID;
	protected int m_BANDITLeaderboardID;
	
	protected autoptr array<autoptr HAB_LBWidget> m_lbwidgets;
	
	// Leaderboard query limit - players beyond this won't show their exact rank
	protected static const int LEADERBOARD_LIMIT = 100;
	
	// Build the leaderboard query with optional blacklist filter
	// isHero: true for hero leaderboard (humanity > 1000), false for bandit (humanity < -1000)
	protected string BuildLeaderboardQuery(bool isHero)
	{
		string query;
		
		// Check if blacklist exists and has entries
		if (m_HaBGeneralConfig && m_HaBGeneralConfig.LeaderboardBlacklist && m_HaBGeneralConfig.LeaderboardBlacklist.Count() > 0)
		{
			// Build the $nin array for blacklisted GUIDs
			string blacklistArray = "[";
			for (int i = 0; i < m_HaBGeneralConfig.LeaderboardBlacklist.Count(); i++)
			{
				if (i > 0)
					blacklistArray += ",";
				blacklistArray += "\"" + m_HaBGeneralConfig.LeaderboardBlacklist.Get(i) + "\"";
			}
			blacklistArray += "]";
			
			// Build query with both humanity filter and blacklist
			if (isHero)
				query = "{ \"Humanity\": {\"$gt\": 1000 }, \"GUID\": {\"$nin\": " + blacklistArray + "} }";
			else
				query = "{ \"Humanity\": {\"$lt\": -1000 }, \"GUID\": {\"$nin\": " + blacklistArray + "} }";
		}
		else
		{
			// No blacklist, use simple query
			if (isHero)
				query = "{ \"Humanity\": {\"$gt\": 1000 } }";
			else
				query = "{ \"Humanity\": {\"$lt\": -1000 } }";
		}
		
		return query;
	}
	
	void HAB_LeaderboardsPage(Widget parent, HAB_MainPanel panel, PlayerBase player){
		m_panel = panel;
		layoutRoot = Widget.Cast(g_Game.GetWorkspace().CreateWidgets(m_LayoutPath,parent));
		Class.CastTo(m_player,player);
		
		m_HeroGrid = Widget.Cast(layoutRoot.FindAnyWidget("HeroGrid"));
		m_BanditGrid = Widget.Cast(layoutRoot.FindAnyWidget("BanditGrid"));
		m_HeroRankText = TextWidget.Cast(layoutRoot.FindAnyWidget("HeroRankText"));
		m_BanditRankText = TextWidget.Cast(layoutRoot.FindAnyWidget("BanditRankText"));
		
		// Initialize rank text based on player's current affinity
		InitializeRankDisplay();
		
		// Guard against offline UFramework
		if (!U() || !U().IsOnline()){
			Print("[HaB] [Warn] Leaderboard queries skipped - UFramework is offline");
			if (m_HeroRankText) m_HeroRankText.SetText("#HAB_UNAVAILABLE");
			if (m_BanditRankText) m_BanditRankText.SetText("#HAB_UNAVAILABLE");
			layoutRoot.SetHandler(this);
			return;
		}
		
		// Build queries with blacklist filter if configured
		string heroQuery = BuildLeaderboardQuery(true);
		string banditQuery = BuildLeaderboardQuery(false);
		
		m_HEROLeaderboardID = HABPlayerDataHandler.Query(new UDBQuery(heroQuery,"{ \"Humanity\": -1 }",true, LEADERBOARD_LIMIT), this, "CBLoadData");
		m_BANDITLeaderboardID = HABPlayerDataHandler.Query(new UDBQuery(banditQuery,"{ \"Humanity\": 1 }",true, LEADERBOARD_LIMIT), this, "CBLoadData");
	
		layoutRoot.SetHandler(this);
	}
	
	protected void InitializeRankDisplay(){
		// Get player's current affinity
		int playerAffinity = HAB_BAMBI;
		if (m_player)
			playerAffinity = m_player.HABAffinity();
		
		// Hero rank text - only relevant for heroes
		if (m_HeroRankText){
			if (playerAffinity == HAB_HERO){
				m_HeroRankText.SetText("#HAB_LOADING");
				m_HeroRankText.SetColor(ARGB(255, 138, 138, 138));
			} else {
				// Bambis and Bandits don't show on hero leaderboard
				m_HeroRankText.SetText("");
			}
		}
		
		// Bandit rank text - only relevant for bandits
		if (m_BanditRankText){
			if (playerAffinity == HAB_BANDIT){
				m_BanditRankText.SetText("#HAB_LOADING");
				m_BanditRankText.SetColor(ARGB(255, 138, 138, 138));
			} else {
				// Bambis and Heroes don't show on bandit leaderboard
				m_BanditRankText.SetText("");
			}
		}
	}
	
	void CBLoadData(int cid, int status, string oid, UDBQueryResultHABPlayer data){
		if (status == UF_SUCCESS){
			array<autoptr HeroesAndBanditsPlayerBase> dataarray;
			Class.CastTo( dataarray, data.GetResults() );
			Widget grid;
			TextWidget rankText;
			bool isHeroBoard = false;
			
			if (cid == m_HEROLeaderboardID){
				Class.CastTo(grid,m_HeroGrid);
				rankText = m_HeroRankText;
				isHeroBoard = true;
			}
			if (cid == m_BANDITLeaderboardID){
				Class.CastTo(grid,m_BanditGrid);
				rankText = m_BanditRankText;
				isHeroBoard = false;
			}
			if (!grid) return;
			if (!m_lbwidgets) m_lbwidgets = new array<autoptr HAB_LBWidget>;
			
			// Get the current player's GUID and affinity for comparison
			string playerGUID = "";
			int playerAffinity = HAB_BAMBI;
			if (m_player){
				playerGUID = m_player.GetHABGUIDCache();
				playerAffinity = m_player.HABAffinity();
			}
			
			int playerRank = -1;
			int totalResults = dataarray.Count();
			
			for (int i = 0; i < totalResults; i++){
				HeroesAndBanditsPlayerBase player = HeroesAndBanditsPlayerBase.Cast(dataarray.Get(i));
				m_lbwidgets.Insert(new HAB_LBWidget(grid, player, (i + 1)));
				
				// Check if this is the current player
				if (playerGUID != "" && player.GetGUID() == playerGUID){
					playerRank = i + 1;
				}
			}
			
			// Update the rank display based on affinity relevance
			if (rankText){
				// Only show rank info if this leaderboard is relevant to the player
				bool isRelevantBoard = (isHeroBoard && playerAffinity == HAB_HERO) || (!isHeroBoard && playerAffinity == HAB_BANDIT);
				
				if (!isRelevantBoard){
					// Player is Bambi, or viewing the opposite affinity board - hide rank text
					rankText.SetText("");
				}
				else if (playerRank > 0){
					// Player found in top results
					rankText.SetText("#HAB_YOUR_RANK: #" + playerRank.ToString());
					if (isHeroBoard)
						rankText.SetColor(ARGB(255, 51, 115, 204)); // Hero blue
					else
						rankText.SetColor(ARGB(255, 204, 64, 51)); // Bandit red
				}
				else {
					// Player not in top results - they're ranked beyond the limit
					rankText.SetText("#HAB_RANK_BEYOND " + LEADERBOARD_LIMIT.ToString());
					rankText.SetColor(ARGB(255, 138, 138, 138)); // Muted gray
				}
			}
		}
	}
	
	void ~HAB_LeaderboardsPage(){
		U().RequestCallCancel(m_HEROLeaderboardID);
		U().RequestCallCancel(m_BANDITLeaderboardID);
		if (m_lbwidgets){
			m_lbwidgets.Clear();
		}
	}
	
}

class HAB_StatWidget extends ScriptedWidgetEventHandler {
	protected static string m_LayoutPath = "HeroesAndBandits/gui/layouts/StatWidget.layout";
	
	protected Widget layoutRoot;
	protected TextWidget m_ActionsName;
	protected TextWidget m_StatCount;
	
	void HAB_StatWidget(Widget parent, string action, int count, int accentColor){
		layoutRoot = Widget.Cast(g_Game.GetWorkspace().CreateWidgets(m_LayoutPath,parent));
		m_ActionsName = TextWidget.Cast(layoutRoot.FindAnyWidget("ActionsName"));
		m_StatCount = TextWidget.Cast(layoutRoot.FindAnyWidget("StatCount"));
		m_ActionsName.SetText(FormatStatName(action));
		m_StatCount.SetText(count.ToString());
		
		// Apply accent color to the stat bar
		Widget statAccent = layoutRoot.FindAnyWidget("StatAccent");
		if (statAccent){
			// Use accent color with reduced alpha for subtle effect
			int r = (accentColor >> 16) & 0xFF;
			int g = (accentColor >> 8) & 0xFF;
			int b = accentColor & 0xFF;
			statAccent.SetColor(ARGB(128, r, g, b));
		}
	}
	
	protected string FormatStatName(string action){
		// Handle specific full stat names first
		if (action == "zombiekill") return "Zombie Kills";
		if (action == "zombiedeath") return "Zombie Deaths";
		if (action == "herokill") return "Hero Kills";
		if (action == "banditkill") return "Bandit Kills";
		if (action == "bambikill") return "Bambi Kills";
		if (action == "playerdeath") return "Player Deaths";
		if (action == "death") return "Deaths";
		if (action == "suicide") return "Suicides";
		if (action == "hungerdeath") return "Hunger Deaths";
		if (action == "toxicdeath") return "Toxic Deaths";
		if (action == "huntanimal") return "Animals Hunted";
		if (action == "fencepartraid") return "Fences Raided";
		if (action == "watchtowerpartraid") return "Watchtowers Raided";
		if (action == "medicbandageplayer") return "Bandages Given";
		if (action == "medicgiveblood") return "Blood Given";
		if (action == "medicgivesaline") return "Saline Given";
		if (action == "medicgivecpr") return "CPR Given";
		if (action == "medicfeedtetracycline") return "Tetracycline Given";
		if (action == "medicfeedpainkiller") return "Painkillers Given";
		if (action == "medicfeedcharcoal") return "Charcoal Given";
		if (action == "medicfeedvitamin") return "Vitamins Given";
		if (action == "medicsplintplayer") return "Splints Applied";
		
		// Handle hunted animals
		if (action == "huntwolf") return "Wolves Hunted";
		if (action == "huntchicken") return "Chickens Hunted";
		if (action == "huntdeer") return "Deer Hunted";
		if (action == "huntboar") return "Boars Hunted";
		if (action == "huntbear") return "Bears Hunted";
		if (action == "huntcow") return "Cows Hunted";
		if (action == "huntgoat") return "Goats Hunted";
		if (action == "huntsheep") return "Sheep Hunted";
		if (action == "huntpig") return "Pigs Hunted";
		if (action == "huntrabbit") return "Rabbits Hunted";
		if (action == "huntfox") return "Foxes Hunted";
		if (action == "huntroe") return "Roe Deer Hunted";
		if (action == "hunthen") return "Hens Hunted";
		if (action == "huntrooster") return "Roosters Hunted";
		
		// Fallback for unknown hunt actions
		if (action.Contains("hunt")){
			string result = action;
			result.Replace("hunt", "");
			return result + " Hunted";
		}
		
		// Return action as-is for unknown stats
		return action;
	}
}
class HAB_LBWidget extends ScriptedWidgetEventHandler {
	protected static string m_LayoutPath = "HeroesAndBandits/gui/layouts/LeaderboardWidget.layout";
	
	protected Widget layoutRoot;
	protected Widget m_Frame;
	protected TextWidget m_Rank;
	protected TextWidget m_PlayerName;
	protected TextWidget m_Humanity;
	
	void HAB_LBWidget(Widget parent, HeroesAndBanditsPlayerBase data, int rank){
		layoutRoot = Widget.Cast(g_Game.GetWorkspace().CreateWidgets(m_LayoutPath,parent));
		
		m_Frame = Widget.Cast(layoutRoot.FindAnyWidget("Frame"));
		m_Rank  = TextWidget.Cast(layoutRoot.FindAnyWidget("Rank"));
		m_PlayerName = TextWidget.Cast(layoutRoot.FindAnyWidget("PlayerName"));
		m_Humanity = TextWidget.Cast(layoutRoot.FindAnyWidget("Humanity"));
		m_PlayerName.SetText(data.PlayerName());
		
		// Display humanity with proper formatting - negative for bandits
		int humanity = data.GetHumanity();
		m_Humanity.SetText(UUtil.ConvertIntToNiceString(humanity));
		
		m_Rank.SetText(rank.ToString() + ".");
	}
	
}

class HAB_InfoPage extends HAB_PageBase {
	protected static string m_LayoutPath = "HeroesAndBandits/gui/layouts/InfoPage.layout";
	
	protected TextWidget m_InfoTitle;
	protected Widget m_SectionsGrid;
	protected ButtonWidget m_DiscordButton;
	protected string m_DiscordLink;
	
	protected autoptr array<autoptr HAB_CollapsibleSection> m_Sections;
	
	void HAB_InfoPage(Widget parent, HAB_MainPanel panel, PlayerBase player){
		m_panel = panel;
		layoutRoot = Widget.Cast(g_Game.GetWorkspace().CreateWidgets(m_LayoutPath, parent));
		Class.CastTo(m_player, player);
		
		m_InfoTitle = TextWidget.Cast(layoutRoot.FindAnyWidget("InfoTitle"));
		m_SectionsGrid = layoutRoot.FindAnyWidget("SectionsGrid");
		m_DiscordButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("DiscordButton"));
		
		m_Sections = new array<autoptr HAB_CollapsibleSection>;
		
		// Apply accent colors from config
		ApplyAccentToWidget("InfoAccent");
		if (m_InfoTitle) m_InfoTitle.SetColor(GetAccentColor());
		
		// Setup Discord button
		SetupDiscordButton();
		
		layoutRoot.SetHandler(this);
		LoadSections();
	}
	
	protected void SetupDiscordButton(){
		HABPanelConfig config = HABPanelConfig.Get();
		if (!config) {
			if (m_DiscordButton) m_DiscordButton.Show(false);
			return;
		}
		
		m_DiscordLink = config.DiscordLink;
		
		// Hide button if no link configured
		if (m_DiscordLink == "" || !m_DiscordLink) {
			if (m_DiscordButton) m_DiscordButton.Show(false);
		} else {
			if (m_DiscordButton) m_DiscordButton.Show(true);
		}
	}
	
	override bool OnClick(Widget w, int x, int y, int button){
		if (w == m_DiscordButton && m_DiscordLink != ""){
			GetGame().OpenURL(m_DiscordLink);
			return true;
		}
		return false;
	}
	
	void LoadSections(){
		if (!m_player) return;
		
		int affinity = m_player.HABAffinity();
		
		HABPanelConfig config = HABPanelConfig.Get();
		if (!config) return;
		
		HABPanelAffinityConfig affinityConfig = config.GetAffinityConfig(affinity);
		if (!affinityConfig || !affinityConfig.Sections) return;
		
		for (int i = 0; i < affinityConfig.Sections.Count(); i++){
			HABPanelSection section = affinityConfig.Sections.Get(i);
			if (section){
				HAB_CollapsibleSection sectionWidget = new HAB_CollapsibleSection(m_SectionsGrid, section, this);
				m_Sections.Insert(sectionWidget);
			}
		}
	}
	
	void RefreshSectionsGrid(){
		// Called when a section is collapsed/expanded to refresh the grid
	}
	
	void ~HAB_InfoPage(){
		if (m_Sections){
			m_Sections.Clear();
		}
	}
}

class HAB_CollapsibleSection extends ScriptedWidgetEventHandler {
	protected static string m_LayoutPath = "HeroesAndBandits/gui/layouts/CollapsibleSection.layout";
	protected static string m_ContentLinePath = "HeroesAndBandits/gui/layouts/ContentLine.layout";
	
	protected Widget layoutRoot;
	protected ButtonWidget m_Header;
	protected TextWidget m_CollapseIcon;
	protected TextWidget m_SectionTitle;
	protected Widget m_ContentFrame;
	protected Widget m_ContentGrid;
	
	protected HAB_InfoPage m_Page;
	protected bool m_IsExpanded;
	
	protected autoptr array<Widget> m_ContentLines;
	
	void HAB_CollapsibleSection(Widget parent, HABPanelSection section, HAB_InfoPage page){
		m_Page = page;
		m_IsExpanded = true;
		m_ContentLines = new array<Widget>;
		
		layoutRoot = Widget.Cast(g_Game.GetWorkspace().CreateWidgets(m_LayoutPath, parent));
		
		m_Header = ButtonWidget.Cast(layoutRoot.FindAnyWidget("SectionHeader"));
		m_CollapseIcon = TextWidget.Cast(layoutRoot.FindAnyWidget("CollapseIcon"));
		m_SectionTitle = TextWidget.Cast(layoutRoot.FindAnyWidget("SectionTitle"));
		m_ContentFrame = layoutRoot.FindAnyWidget("ContentFrame");
		m_ContentGrid = layoutRoot.FindAnyWidget("ContentGrid");
		
		// Apply accent color from config
		int accentColor = page.GetAccentColor();
		Widget headerAccent = layoutRoot.FindAnyWidget("HeaderAccent");
		if (headerAccent) headerAccent.SetColor(accentColor);
		
		if (m_SectionTitle && section.Title)
			m_SectionTitle.SetText(section.Title);
		
		// Add content lines
		if (section.Content){
			for (int i = 0; i < section.Content.Count(); i++){
				string lineText = section.Content.Get(i);
				TextWidget lineWidget = TextWidget.Cast(g_Game.GetWorkspace().CreateWidgets(m_ContentLinePath, m_ContentGrid));
				if (lineWidget){
					lineWidget.SetText(lineText);
					m_ContentLines.Insert(lineWidget);
				}
			}
		}
		
		// Deferred update to let layout calculate sizes after text is set
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(UpdateExpandState, 10, false);
		layoutRoot.SetHandler(this);
	}
	
	void UpdateExpandState(){
		// With GridSpacer "Size To Content V", just show/hide content and layout auto-sizes
		if (m_IsExpanded){
			m_CollapseIcon.SetText("-");
			m_ContentFrame.Show(true);
		} else {
			m_CollapseIcon.SetText("+");
			m_ContentFrame.Show(false);
		}
		
		// Force layout update on parent grid
		if (m_Page)
			m_Page.RefreshSectionsGrid();
	}
	
	override bool OnClick(Widget w, int x, int y, int button){
		if (w == m_Header){
			m_IsExpanded = !m_IsExpanded;
			UpdateExpandState();
			return true;
		}
		return false;
	}
	
	void ~HAB_CollapsibleSection(){
		if (m_ContentLines){
			m_ContentLines.Clear();
		}
	}
}