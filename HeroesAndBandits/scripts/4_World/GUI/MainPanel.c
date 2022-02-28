class HAB_MainPanel extends UIScriptedMenu {
	protected static string m_LayoutPath = "HeroesAndBandits/gui/layouts/HumanityPanel.layout";
	
	protected autoptr HAB_PageBase m_CurPage;
	
	protected autoptr ButtonWidget m_LeaderboardsButton;
	protected autoptr ButtonWidget m_StatsButton;
	protected autoptr Widget m_Page;
	protected PlayerBase m_player;
	
	override Widget Init() {
		layoutRoot = Widget.Cast(GetGame().GetWorkspace().CreateWidgets(m_LayoutPath));
		m_LeaderboardsButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("LeaderboardsButton"));
		m_StatsButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("StatsButton"));
		m_Page = Widget.Cast(layoutRoot.FindAnyWidget("Page"));
        GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_MOUSE_ALL);
        GetGame().GetUIManager().ShowUICursor(true);
		GetGame().GetMission().GetHud().Show(false);
    	PPEffects.SetBlurInventory(0.5);
		if (Class.CastTo(m_player,GetGame().GetPlayer())){
			m_player.RefreshHABData();		
			m_player.RequestHABControllerData();
		}
		m_CurPage = new HAB_StatsPage(m_Page,this,m_player);
		return layoutRoot;
	}
	
	
	override bool OnClick(Widget w, int x, int y, int button){
		
		if (w == m_LeaderboardsButton && !m_CurPage.IsInherited(HAB_LeaderboardsPage)){
			delete m_CurPage;
			m_CurPage = new HAB_LeaderboardsPage(m_Page,this,m_player);
		}
		if (w == m_StatsButton && !m_CurPage.IsInherited(HAB_StatsPage)){
			delete m_CurPage;
			m_CurPage = new HAB_StatsPage(m_Page,this,m_player);
		}
		
		return super.OnClick(w,x,y,button);
	}
	
	override void Update(float timeslice) {
		super.Update(timeslice);
		if (m_CurPage)
			m_CurPage.MyUpdate(timeslice);
	}
	
	
	
	void ~HAB_MainPanel() {
        GetGame().GetMission().PlayerControlEnable(false);
        GetGame().GetInput().ResetGameFocus();
        GetGame().GetUIManager().ShowUICursor(false);
		GetGame().GetMission().GetHud().Show(true);
    	PPEffects.SetBlurInventory(0);
	}
	
}

class HAB_PageBase extends ScriptedWidgetEventHandler {
	HAB_MainPanel m_panel
	protected PlayerBase m_player;
	protected Widget layoutRoot;
	void HAB_PageBase(Widget parent, HAB_MainPanel panel, PlayerBase player){
		
	}
	
	void ~HAB_PageBase(){
		layoutRoot.Show(false);
		layoutRoot.Unlink();
	}
	
	void MyUpdate(float timeslice){
		
	}
}

class HAB_StatsPage extends HAB_PageBase {
	protected static string m_LayoutPath = "HeroesAndBandits/gui/layouts/HumanityPage.layout";
	
	protected ImageWidget m_Icon;
	protected TextWidget m_LevelText;
	protected TextWidget m_HumanityText
	
	protected TextWidget m_LowText;
	protected TextWidget m_HighText;
	protected ProgressBarWidget m_PrimaryProgress;
	protected ProgressBarWidget m_BanditProgress;
	protected ProgressBarWidget m_HeroProgress;
	
	protected Widget m_StatsGrid;
	
	
	void HAB_StatsPage(Widget parent, HAB_MainPanel panel, PlayerBase player){
		m_panel = panel;
		layoutRoot = Widget.Cast(GetGame().GetWorkspace().CreateWidgets(m_LayoutPath,parent));
		Class.CastTo(m_player,player);
		
		m_Icon = ImageWidget.Cast(layoutRoot.FindAnyWidget("Icon"));
		m_LevelText = TextWidget.Cast(layoutRoot.FindAnyWidget("LevelText"));
		m_HumanityText = TextWidget.Cast(layoutRoot.FindAnyWidget("HumanityText"));
	
		m_LowText = TextWidget.Cast(layoutRoot.FindAnyWidget("LowText"));
		m_HighText = TextWidget.Cast(layoutRoot.FindAnyWidget("HighText"));
		m_PrimaryProgress = ProgressBarWidget.Cast(layoutRoot.FindAnyWidget("PrimaryProgress"));
		m_BanditProgress = ProgressBarWidget.Cast(layoutRoot.FindAnyWidget("BanditProgress"));
		m_HeroProgress = ProgressBarWidget.Cast(layoutRoot.FindAnyWidget("HeroProgress"));
	
		m_StatsGrid = Widget.Cast(layoutRoot.FindAnyWidget("StatsGrid"));
		layoutRoot.SetHandler(this);
		UpdateData();
	}
	
	protected float m_time;
	override void MyUpdate(float timeslice){
		super.MyUpdate(timeslice);
		m_time+= timeslice;
		
		if (m_time > 0.7){
			m_time = 0;
			UpdateData();
		}
	}
	
	void UpdateData(){
		m_Icon.LoadImageFile(0,m_player.GetClientIcon());
		if (Math.AbsInt(m_player.HABLevel()) > 0){
			m_LevelText.SetText(m_player.GetClientAffinityName() + " #HAB_LEVEL " + Math.AbsInt(m_player.HABLevel()).ToString());
		} else {
			m_LevelText.SetText(m_player.GetClientAffinityName());
		}
		int humanity = m_player.Humanity();
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
				float value = m_player.Humanity() - cData.param1;
				percent = value / Math.AbsFloat(cData.param2) * 100;
				m_PrimaryProgress.SetCurrent(percent);
			}
		}
	}
}

class HAB_LeaderboardsPage extends HAB_PageBase {
	protected static string m_LayoutPath = "HeroesAndBandits/gui/layouts/LeaderboardsPage.layout";
	
	protected Widget m_HeroGrid;
	protected Widget m_BanditGrid;
	
	protected int m_HEROLeaderboardID;
	protected int m_BANDITLeaderboardID;
	
	protected autoptr array<autoptr HAB_LBWidget> m_lbwidgets;
	
	void HAB_LeaderboardsPage(Widget parent, HAB_MainPanel panel, PlayerBase player){
		m_panel = panel;
		layoutRoot = Widget.Cast(GetGame().GetWorkspace().CreateWidgets(m_LayoutPath,parent));
		Class.CastTo(m_player,player);
		
		m_HeroGrid = Widget.Cast(layoutRoot.FindAnyWidget("HeroGrid"));
		m_BanditGrid = Widget.Cast(layoutRoot.FindAnyWidget("BanditGrid"));
		m_HEROLeaderboardID = HABPlayerDataHandler.Query(new UApiDBQuery("{ \"Humanity\": {\"$gt\": 1000 } }","{ \"Humanity\": -1 }",true,100), this, "CBLoadData");
		m_BANDITLeaderboardID = HABPlayerDataHandler.Query(new UApiDBQuery("{ \"Humanity\": {\"$lt\": -1000 } }","{ \"Humanity\": 1 }",true,100), this, "CBLoadData");
	
		layoutRoot.SetHandler(this);
	}
	
	void CBLoadData(int cid, int status, string oid, autoptr UApiQueryResultHABPlayer data){
		if (status == UAPI_SUCCESS){
			array<autoptr HeroesAndBanditsPlayerBase> dataarray;
			Class.CastTo(dataarray, data.GetResults() );
			Widget grid;
			if (cid == m_HEROLeaderboardID){
				Class.CastTo(grid,m_HeroGrid);
			}
			if (cid == m_BANDITLeaderboardID){
				Class.CastTo(grid,m_BanditGrid);
			}
			if (!grid) return;
			if (!m_lbwidgets) m_lbwidgets = new array<autoptr HAB_LBWidget>;
			for (int i = 0; i< dataarray.Count(); i++){
				HeroesAndBanditsPlayerBase player = HeroesAndBanditsPlayerBase.Cast(dataarray.Get(i));
				m_lbwidgets.Insert(new HAB_LBWidget(grid,player, (i + 1)));
			}
		}
	}
	
	void ~HAB_LeaderboardsPage(){
		UApi().RequestCallCancel(m_HEROLeaderboardID);
		UApi().RequestCallCancel(m_BANDITLeaderboardID);
	}
	
}

class HAB_StatWidget extends ScriptedWidgetEventHandler {
	protected static string m_LayoutPath = "HeroesAndBandits/gui/layouts/StatWidget.layout";
	
	
	protected Widget layoutRoot;
	protected TextWidget m_ActionsName;
	protected TextWidget m_StatCount;
	void HAB_LBWidget(Widget parent, string action, int count){
		layoutRoot = Widget.Cast(GetGame().GetWorkspace().CreateWidgets(m_LayoutPath,parent));
		m_ActionsName = TextWidget.Cast(layoutRoot.FindAnyWidget("ActionsName"));
		m_StatCount = TextWidget.Cast(layoutRoot.FindAnyWidget("StatCount"));
		m_ActionsName.SetText(action);
		m_StatCount.SetText(count.ToString());
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
		layoutRoot = Widget.Cast(GetGame().GetWorkspace().CreateWidgets(m_LayoutPath,parent));
		
		m_Frame = Widget.Cast(layoutRoot.FindAnyWidget("Frame"));
		m_Rank  = TextWidget.Cast(layoutRoot.FindAnyWidget("Rank"));
		m_PlayerName = TextWidget.Cast(layoutRoot.FindAnyWidget("PlayerName"));
		m_Humanity = TextWidget.Cast(layoutRoot.FindAnyWidget("Humanity"));
		m_PlayerName.SetText(data.PlayerName());
		m_Humanity.SetText(data.GetHumanity().ToString());
		m_Rank.SetText(rank.ToString() + ".");
	}
	
}