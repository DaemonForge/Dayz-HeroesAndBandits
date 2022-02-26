class HAB_OG_Panel extends UIScriptedMenu {
	
	
	protected static string m_LayoutPath = "HeroesAndBandits/gui/layouts/HeroesAndBanditsPanel.layout";
	
	protected ImageWidget m_LevelImage;
	
		
	protected TextWidget m_Affinity;
	protected TextWidget m_LevelName;
	protected TextWidget m_Humanity;
	protected TextWidget m_Deaths;
	protected TextWidget m_Hunt;
	protected TextWidget m_Raid;
	protected TextWidget m_Medic;
	protected TextWidget m_ZombieKills;
	protected TextWidget m_PlayerKills;
	protected PlayerBase m_player;
	
	protected float m_time;
	
	
	override Widget Init()
    {
		layoutRoot = Widget.Cast(GetGame().GetWorkspace().CreateWidgets(m_LayoutPath));
		m_LevelImage = ImageWidget.Cast(layoutRoot.FindAnyWidget("LevelImage"));
		m_Affinity = TextWidget.Cast(layoutRoot.FindAnyWidget("Affinity"));
		m_LevelName = TextWidget.Cast(layoutRoot.FindAnyWidget("LevelName"));
		m_Humanity = TextWidget.Cast(layoutRoot.FindAnyWidget("Humanity"));
		m_PlayerKills = TextWidget.Cast(layoutRoot.FindAnyWidget("PlayerKills"));
		m_Deaths = TextWidget.Cast(layoutRoot.FindAnyWidget("Deaths"));
		m_Hunt = TextWidget.Cast(layoutRoot.FindAnyWidget("Hunt"));
		m_Raid = TextWidget.Cast(layoutRoot.FindAnyWidget("Raid"));
		m_Medic = TextWidget.Cast(layoutRoot.FindAnyWidget("Medic"));
		m_ZombieKills = TextWidget.Cast(layoutRoot.FindAnyWidget("ZombieKills"));
		Class.CastTo(m_player,GetGame().GetPlayer());
		
        GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_MOUSE_ALL);
        GetGame().GetUIManager().ShowUICursor(true);
		GetGame().GetMission().GetHud().Show(false);
    	PPEffects.SetBlurInventory(0.5);
		UpdateData();
		return layoutRoot;
	} 
	
	void Update(float timeslice)
	{
		m_time+= timeslice;
		if (m_time > 0.7){
			m_time = 0;
			if (m_player){
				UpdateData();
			}
		}
	}
	
	void UpdateData(){
		m_LevelImage.LoadImageFile(0,m_player.GetClientIcon());
		m_Affinity.SetText(m_player.GetClientAffinityName());
		if (Math.AbsInt(m_player.HABLevel()) > 0){
			m_LevelName.SetText("#HAB_LEVEL " + Math.AbsInt(m_player.HABLevel()).ToString());
		} else {
			m_LevelName.SetText("");
		}
		int humanity = m_player.Humanity();
		m_Humanity.SetText("#HAB_HUMANITY: " + UUtil.ConvertIntToNiceString(humanity));
		UpdateDeaths();
		UpdateHunt();
		UpdateRaid();
		UpdatePlayerKills();
		UpdateZombieKills();
		UpdateMedic();
	}
	
	autoptr TStringArray m_DeathActions = {"zombiedeath","toxicdeath","hungerdeath","playerdeath","death","sucide"};
	void UpdateDeaths(){
		int statTotal = 0;
		foreach (string action: m_DeathActions){
			int stat = 0;
			m_player.HABData().GetStat(action,stat);
			statTotal+=stat;
		}
		m_Deaths.SetText("#HAB_DEATHS: " + statTotal.ToString() );
	}
	void UpdateHunt(){
		int stat = 0;
		m_player.HABData().GetStat("huntanimal",stat);
		m_Hunt.SetText("#HAB_HUNTS: " + stat.ToString());
	}
	autoptr TStringArray m_RaidActions = {"watchtowerpartraid","fencepartraid"};
	void UpdateRaid(){
		int statTotal = 0;
		foreach (string action: m_RaidActions){
			int stat = 0;
			m_player.HABData().GetStat(action,stat);
			statTotal+=stat;
		}
		m_Raid.SetText("#HAB_RAID: " + statTotal.ToString());
	}
	autoptr TStringArray m_KillActions = {"banditkill","bambikill","herokill"};
	void UpdatePlayerKills(){
		int statTotal = 0;
		foreach (string action: m_KillActions){
			int stat = 0;
			m_player.HABData().GetStat(action,stat);
			statTotal+=stat;
		}
		m_PlayerKills.SetText("#HAB_PLAYERKILLS: " + statTotal.ToString());
	}
	void UpdateZombieKills(){
		int stat = 0;
		m_player.HABData().GetStat("zombiekill",stat);
		m_ZombieKills.SetText("#HAB_ZOMBIEKILLS: " + stat.ToString());
	}
	autoptr TStringArray m_MedicActions = {"medicbandageplayer", "medicgiveblood","medicgivesaline","medicgivecpr","medicfeedtetracycline","medicfeedpainkiller","medicfeedcharcoal","medicfeedvitamin","medicsplintplayer"};
	void UpdateMedic(){
		int statTotal = 0;
		foreach (string action: m_MedicActions){
			int stat = 0;
			m_player.HABData().GetStat(action,stat);
			statTotal+=stat;
		}
		m_Medic.SetText("#HAB_MEDIC: " + statTotal.ToString());
	}
	
	void ~HAB_OG_Panel(){
        GetGame().GetMission().PlayerControlEnable(false);
        GetGame().GetInput().ResetGameFocus();
        GetGame().GetUIManager().ShowUICursor(false);
		GetGame().GetMission().GetHud().Show(true);
    	PPEffects.SetBlurInventory(0);
	}
	
	override bool OnKeyPress(Widget w,int x, int y, int key){
		
		
	}
	
}