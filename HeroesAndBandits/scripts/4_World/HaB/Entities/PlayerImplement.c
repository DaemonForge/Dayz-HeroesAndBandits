modded class DayZPlayerImplement extends DayZPlayer
{
	
	
	protected string m_HABGUIDCache;
	protected string m_HABNameCache;
	
	protected float m_Humanity = 0;
	protected int m_HaBLevel = 0;
	
	protected autoptr HeroesAndBanditsPlayerBase m_HABData;
	protected autoptr HeroesAndBanditsControllerBase m_HABControllerBase;
	
	void DayZPlayerImplement()
	{
		RegisterNetSyncVariableFloat("m_Humanity");
		RegisterNetSyncVariableInt("m_HaBLevel",HAB_BANDIT_MAXLEVEL,HAB_HERO_MAXLEVEL);
	}
	
	void ~DayZPlayerImplement(){
		if (m_HABData) delete m_HABData;
		if (m_HABControllerBase) delete m_HABControllerBase;
	}
	
	string GetHABGUIDCache(){
		return m_HABGUIDCache;
	}
	
	string GetHABNameCache(){
		return m_HABNameCache;
	}
	
	HeroesAndBanditsPlayerBase HABData(){
		return m_HABData;
	}
	
	void SaveHABData(){
		if (g_Game.IsDedicatedServer() && HABData()){
			// Guard against offline UFramework
			if (!UF() || !UF().IsOnline()){
				Print("[HaB] [Warn] SaveHABData skipped - UFramework offline");
				return;
			}
			HABPlayerDataHandler.Save(GetHABGUIDCache(), HABData());
		}
	}
	
	int HABAffinity(){
		// Server: use controller if available
		if (HABContoller())
			return HABContoller().Affinity();
		
		// Client or no controller: calculate from synced humanity value
		return HeroesAndBandits.GetAffinity(Humanity());
	}
	
	HeroesAndBanditsControllerBase HABContoller(){
		return m_HABControllerBase;
	}
	
	
	void InitHABController(){
		if (!g_Game.IsDedicatedServer()) Error2("Heroes and Bandits", "Trying to Init Controller on client");
		if (g_Game.GetMission() && g_Game.GetMission().ClassName() == "MissionMainMenu") Error2("Heroes and Bandits", "Trying to Init Controller on Main Menu");
		m_HABControllerBase = HeroesAndBandits.Controller(Humanity(),PlayerBase.Cast(this));
	}
	void UpdateHABController(){
		if (!g_Game.IsDedicatedServer()) Error2("Heroes and Bandits", "Trying to Init Controller on client");
		
		m_HABControllerBase = HeroesAndBandits.Controller(Humanity(),PlayerBase.Cast(this));
	}
	
	void OnHABLevelChange(int oldLevel, int newLevel, bool isFirst){
		HABContoller().OnLevelChange(oldLevel, newLevel,isFirst);
		m_HaBLevel = newLevel;
	}
	
	
	void OnHABAffinityChange( int oldAffinity, int newAffinity, bool isFirst ){
		UpdateHABController();
		HABContoller().OnAffinityChange(oldAffinity,newAffinity,isFirst);
	}
	
	float Humanity(){
		return m_Humanity;
	}
	
	int HABLevel(){
		return m_HaBLevel;
	}
	
	protected void UpdateHumanity(float humanity){
		
	}
	
	override void SetSuicide(bool state)
	{
		super.SetSuicide(state);
		if (state && g_Game.IsServer()){
			SetAllowDamage(true);
		}
	}
	
	
};