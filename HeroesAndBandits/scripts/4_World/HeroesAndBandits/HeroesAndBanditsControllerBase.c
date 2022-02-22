class HeroesAndBanditsControllerBase extends Managed {
	protected PlayerBase m_player;
	
	protected autoptr map<string,autoptr HaBActionBase> Actions;
	
	protected autoptr map<int,string> m_Icons;
	
	static int MAXKILLSPERPLAYER = 5;
	
	void HeroesAndBanditsControllerBase(PlayerBase player){
		Class.CastTo(m_player,player);
		Actions = new map<string,autoptr HaBActionBase>;
		OnInit();
	}
	
	void OnInit(){
		HABActionConfigs.UpdateActionMap("HAB_ACTIONS",Actions);
		m_Icons = new map<int,string>;
		m_Icons.Set(0,"set:hab_icons image:bambi");
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(GetPlayer().habSyncIcon);
	}
	
	PlayerBase GetPlayer(){
		return m_player;
	}
	
	bool GetBaseActionGain(string Action, out string ActionName, inout float gain, inout bool notify){
		HaBActionBase action;
		if (Actions.Find(Action, action)){
			ActionName = action.DisplayName();
			gain = action.GetGain(GetPlayer().Humanity());
			notify = action.Notify();
			int dailyLimit = action.DailyLimit();
			if (!GetPlayer().HABData().IncermentAction(Action, dailyLimit)){
				Print("[HAB] Reached Daily Limit for action " + Action + " Limit: " + dailyLimit );
				gain = 0;
			}
			return true;
		}
		return false;
	}
	
	bool AdjustActionGain(string Action, EntityAI other, inout float gain, out bool notify){
		return false;
	}
	bool AdjustKillGain(EntityAI other, inout float gain, out bool notify){
		
		return false;
	}
	
	void NewAction(string Action, EntityAI other){
		float gain = 0;
		string ActionName;
		bool notify = true;
		if (!GetBaseActionGain(Action,ActionName,gain,notify)){
			Print("[HAB] Action: " + Action + " has no base humanity defined");
			return;
		}
		
		Print("[HAB] New Action " + ActionName + " gain: " + gain + " notify" +  notify);
		AdjustActionGain(Action,other,gain,notify);
		MissionBaseWorld.Cast(GetGame().GetMission()).NewHABAction(m_player,Action,other,gain,notify);
		
		m_player.IncermentHumanity(gain);
		if (notify && Math.AbsFloat(gain) > 0){
			string message = " ";
			if (gain > 0){
				message = " +";
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
		string guid;
		string Action;
		if (!otherPlayer){
			if (otherPlayer.GetIdentity()){
				guid = otherPlayer.GetIdentity().GetId();
			}
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
		
		if (guid != "" && !GetPlayer().HABData().IncermentAction("kill." + guid, MAXKILLSPERPLAYER)){
			Print("[HAB] Reached Daily Limit for action " + Action + " Limit: " + MAXKILLSPERPLAYER );
			gain = 0;
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
		
		GetPlayer().OnHABAffinityChange(oldAffinity,newAffinity,isFirst);
		if (notify){
			
		}
	}
	
	void OnLevelChange(int oldLevel, int newLevel, bool isFirst){
		bool notify = true;
		
		MissionBaseWorld.Cast(GetGame().GetMission()).OnLevelChange(m_player,oldLevel,newLevel,isFirst,notify);
		
		GetPlayer().OnHABLevelChange(oldLevel,newLevel,isFirst);
		if (notify){
			
		}
	}

	string Icon(){
		return "set:hab_icons image:bambi";
	}
	
	
	int Affinity(){
		return HAB_BAMBI;
	}
	
	string Name(){
		return "#HAB_BAMBI";
	}
	
	void OverrideIcons(autoptr map<int, string> icons){
		m_Icons = icons;
	}

}
class BambiController extends HeroesAndBanditsControllerBase {
	
	override void OnInit(){
		super.OnInit();
		HABActionConfigs.UpdateActionMap("HAB_ACTIONS_BAMBI",Actions);
		Print("Init BambiController");
	}
	

	override bool AdjustActionGain(string Action, EntityAI other, inout float gain, out bool notify){
		if (super.AdjustActionGain(Action, other, gain, notify)){
			return true;
		}
		return false;
	}
	
	
	override string Icon(){
		return "set:hab_icons image:bambi";
	}
	
	override int Affinity(){
		return HAB_BAMBI;
	}
	
	
	override string Name(){
		return "#HAB_BAMBI";
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
	
	override bool AdjustActionGain(string Action, EntityAI other, inout float gain, out bool notify){
		if (super.AdjustActionGain(Action, other, gain, notify)){
			return true;
		}
		return false;
	}
	
	override string Icon(){
		string icon;
		if (m_Icons.Find(HeroesAndBandits.GetLevel(GetPlayer().Humanity()),icon)){
			return icon;
		}
		return "set:hab_icons image:hero";
	}
	override int Affinity(){
		return HAB_HERO;
	}
	override string Name(){
		return "#HAB_HERO";
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

	override bool AdjustActionGain(string Action, EntityAI other, inout float gain, out bool notify){
		if (super.AdjustActionGain(Action, other, gain, notify)){
			return true;
		}
		return false;
	}
	
	override string Icon(){
		string icon;
		if (m_Icons.Find(HeroesAndBandits.GetLevel(GetPlayer().Humanity()),icon)){
			return icon;
		}
		return "set:hab_icons image:bandit";
	}
	override int Affinity(){
		return HAB_BANDIT;
	}
	override string Name(){
		return "#HAB_BANDIT";
	}
}