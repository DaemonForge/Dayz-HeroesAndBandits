class HeroesAndBanditsControllerBase extends Managed {
	protected PlayerBase m_player;
	
	void HeroesAndBanditsControllerBase(PlayerBase player){
		Class.CastTo(m_player,player);
		OnInit();
	}
	
	void OnInit(){
		
	}
	
	PlayerBase GetPlayer(){
		return m_player;
	}
	
	bool GetBaseActionGain(string Action, out string ActionName, inout float humanity, inout bool notify){
		switch (Action){
			case "ZombieKill":
				
			break;
			case "DeathBySucide":
				humanity = 0 - (GetPlayer().Humanity() * 0.2);
			break;
			case "DeathByZombie":
				humanity = 0 - (GetPlayer().Humanity() * 0.05);
			break;
			case "DeathByEnviroment":
				
			break;
			case "ZombieKill":
				
			break;
			case "ZombieKill":
				
			break;			
		}
		return false;
	}
	
	bool AdjustActionGain(string Action, EntityAI other, inout float humanity, out bool notify){
		
		return false;
	}
	bool AdjustKillGain(EntityAI other, inout float humanity, out bool notify){
		
		return false;
	}
	
	void NewAction(string Action, EntityAI other){
		float gain = 0;
		string ActionName;
		bool notify = true;
		if (!GetBaseActionGain(Action,ActionName,gain,notify)){
			Print("[HAB] Action: " + Action + " has no base humanity defined");
		}
		AdjustActionGain(Action,other,gain,notify);
		MissionBaseWorld.Cast(GetGame().GetMission()).NewHABAction(m_player,Action,other,gain,notify);
		
		m_player.IncermentHumanity(gain);
		if (notify && Math.AbsFloat(gain) > 0){
			
			
		}
	}
	
	void NewKillAction(EntityAI other){
		float gain = 0;
		bool notify = true;
		AdjustKillGain(other,gain,notify);
		MissionBaseWorld.Cast(GetGame().GetMission()).NewHABKillAction(m_player,other,gain,notify);
		
		m_player.IncermentHumanity(gain);
	}
	
	void OnAffinityChange(int oldAffinity, int newAffinity, bool isFirst){
		bool notify = true;
		
		MissionBaseWorld.Cast(GetGame().GetMission()).OnHABAffinityChange(m_player,oldAffinity,newAffinity,isFirst,notify);
		if (notify){
			
		}
	}
	
	void OnLevelChange(int oldLevel, int newLevel, bool isFirst){
		bool notify = true;
		
		MissionBaseWorld.Cast(GetGame().GetMission()).OnLevelChange(m_player,oldLevel,newLevel,isFirst,notify);
		
		if (notify){
			
		}
	}


}
class BambiController extends HeroesAndBanditsControllerBase {
	
	override void OnInit(){
		super.OnInit();
		
	}
	

	override bool AdjustActionGain(string Action, EntityAI other, inout float humanity, out bool notify){
		if (super.AdjustActionGain(Action, other, humanity, notify)){
			return true;
		}
		return false;
	}
	
	
}

class HeroController extends HeroesAndBanditsControllerBase {

	override bool AdjustActionGain(string Action, EntityAI other, inout float humanity, out bool notify){
		if (super.AdjustActionGain(Action, other, humanity, notify)){
			return true;
		}
		return false;
	}

}

class BanditController extends HeroesAndBanditsControllerBase {


	override bool AdjustActionGain(string Action, EntityAI other, inout float humanity, out bool notify){
		bool rValue = false;
		if (Action == "Sucide"){
			humanity = humanity * -1;;
			rValue = true;
		}
		if (super.AdjustActionGain(Action, other, humanity, notify)){
			return true;
		}
		return rValue;
	}
}