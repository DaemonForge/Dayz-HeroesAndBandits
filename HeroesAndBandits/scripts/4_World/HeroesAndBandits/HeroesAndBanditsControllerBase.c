class HeroesAndBanditsControllerBase extends Managed {
	protected PlayerBase m_player;
	
	protected autoptr map<string,autoptr HaBActionBase> Actions;
	
	protected autoptr map<int,string> m_Icons;
	
		
	
	void HeroesAndBanditsControllerBase(PlayerBase player){
		Class.CastTo(m_player,player);
		Actions = new map<string,autoptr HaBActionBase>;
		OnInit();
	}
	
	void OnInit(){
		HABActionConfigs.UpdateActionMap("HAB_ACTIONS",Actions);
		m_Icons = new map<int,string>;
		m_Icons.Set(0,"set:hab_icons image:bambi");
	}
	
	PlayerBase GetPlayer(){
		return m_player;
	}
	
	bool GetBaseActionGain(string Action, out string ActionName, inout float humanity, inout bool notify){
		HaBActionBase action;
		if (Actions.Find(Action, action)){
			ActionName = action.DisplayName();
			humanity = action.GetGain(GetPlayer().Humanity());
			notify = action.Notify();
			return true;
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
			string message = "";
			if (gain > 0){
				message = "+";
			} 
			message = message + gain + " Humanity";
			UUtil.SendNotification(ActionName, message, GetPlayer().GetIdentity(), Icon());
		}
	}
	
	void NewKillAction(EntityAI other){
		float gain = 0;
		string ActionName;
		bool notify = true;
		PlayerBase otherPlayer = PlayerBase.Cast(other);
		string Action;
		if (!otherPlayer){
			int affinity = HeroesAndBandits.GetAffinity(otherPlayer.Humanity());
			if (affinity == HAB_HERO){
				Action = "herokill";
			} else if (affinity == HAB_BANDIT){
				Action = "banditkill";
			} else if (affinity == HAB_BAMBI){
				Action = "bambikill";
			}
		}
		if (!GetBaseActionGain(Action,ActionName,gain,notify)){
			Print("[HAB] Action: " + Action + " has no base humanity defined");
		}
		AdjustKillGain(other,gain,notify);
		MissionBaseWorld.Cast(GetGame().GetMission()).NewHABKillAction(m_player,other,gain,notify);
		
		m_player.IncermentHumanity(gain);
		if (notify && Math.AbsFloat(gain) > 0){
			string message = "";
			if (gain > 0){
				message = "+";
			} 
			message = message + gain + " Humanity";
			UUtil.SendNotification(ActionName, message, GetPlayer().GetIdentity(), Icon());
		}
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

	string Icon(){
		return "set:hab_icons image:bambi";
	}
	

}
class BambiController extends HeroesAndBanditsControllerBase {
	
	override void OnInit(){
		super.OnInit();
		HABActionConfigs.UpdateActionMap("HAB_ACTIONS_BAMBI",Actions);
		Print("Init BambiController");
	}
	

	override bool AdjustActionGain(string Action, EntityAI other, inout float humanity, out bool notify){
		if (super.AdjustActionGain(Action, other, humanity, notify)){
			return true;
		}
		return false;
	}
	
	
	override string Icon(){
		return "set:hab_icons image:bambi";
	}
}

class HeroController extends HeroesAndBanditsControllerBase {

	override void OnInit(){
		super.OnInit();
		Print("Init HeroController");
		HABActionConfigs.UpdateActionMap("HAB_ACTIONS_HERO",Actions);
		m_Icons.Set(1,"set:hab_icons image:herolv1");
		m_Icons.Set(2,"set:hab_icons image:herolv2");
		m_Icons.Set(3,"set:hab_icons image:herolv3");
		m_Icons.Set(4,"set:hab_icons image:herolv4");
		m_Icons.Set(5,"set:hab_icons image:herolv5");
	}
	
	override bool AdjustActionGain(string Action, EntityAI other, inout float humanity, out bool notify){
		if (super.AdjustActionGain(Action, other, humanity, notify)){
			return true;
		}
		return false;
	}
	
	override string Icon(){
		return "set:hab_icons image:hero";
	}

}

class BanditController extends HeroesAndBanditsControllerBase {

	override void OnInit(){
		super.OnInit();
		Print("Init BanditController");
		HABActionConfigs.UpdateActionMap("HAB_ACTIONS_BANDIT",Actions);
		m_Icons.Set(1,"set:hab_icons image:banditlv1");
		m_Icons.Set(2,"set:hab_icons image:banditlv2");
		m_Icons.Set(3,"set:hab_icons image:banditlv3");
		m_Icons.Set(4,"set:hab_icons image:banditlv4");
		m_Icons.Set(5,"set:hab_icons image:banditlv5");
	}

	override bool AdjustActionGain(string Action, EntityAI other, inout float humanity, out bool notify){
		if (super.AdjustActionGain(Action, other, humanity, notify)){
			return true;
		}
		return false;
	}
	
	override string Icon(){
		return "set:hab_icons image:bandit";
	}
}