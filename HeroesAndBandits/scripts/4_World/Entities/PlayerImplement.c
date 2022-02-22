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
	
	string GetHABGUIDCache(){
		return m_HABGUIDCache;
	}
	
	HeroesAndBanditsPlayerBase HABData(){
		return m_HABData;
	}
	
	void SaveHABData(){
		if (GetGame().IsDedicatedServer() && HABData()){
			HABPlayerDataHandler.Save(GetHABGUIDCache(), HABData());
		}
	}
	
	int HABAffinity(){
		if (!HABContoller()) return HAB_BAMBI;
		return HABContoller().Affinity();
	}
	
	HeroesAndBanditsControllerBase HABContoller(){
		return m_HABControllerBase;
	}
	
	
	void InitHABController(){
		if (!GetGame().IsDedicatedServer()) Error2("Heroes and Bandits", "Trying to Init Controller on client");
		
		m_HABControllerBase = HeroesAndBandits.Controller(Humanity(),PlayerBase.Cast(this));
	}
	void UpdateHABController(){
		if (!GetGame().IsDedicatedServer()) Error2("Heroes and Bandits", "Trying to Init Controller on client");
		
		m_HABControllerBase = HeroesAndBandits.Controller(Humanity(),PlayerBase.Cast(this));
	}
	
	void OnHABLevelChange(int oldLevel, int newLevel, bool isFirst){
		
	}
	
	
	void OnHABAffinityChange( int oldAffinity, int newAffinity, bool isFirst ){
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.UpdateHABController);
	}
	
	float Humanity(){
		return m_Humanity;
	}
	
	
	protected void UpdateHumanity(float humanity){
		
	}
	
	override void SetSuicide(bool state)
	{
		super.SetSuicide(state);
		if (state && GetGame().IsServer()){
			SetAllowDamage(true);
		}
	}
	
	
};