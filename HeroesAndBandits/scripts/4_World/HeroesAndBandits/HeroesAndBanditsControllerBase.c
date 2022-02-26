static int HAB_MAXKILLSPERPLAYER = 5;
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
		m_Icons.Set(0,"set:hab_newicons image:bambi");
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(GetPlayer().habSyncIcon);
	}
	
	PlayerBase GetPlayer(){
		return m_player;
	}
	
	protected bool GetBaseActionGain(string Action, out string ActionName, out float gain, out bool notify, out int dailyLimit){
		HaBActionBase action;
		if (Actions.Find(Action, action)){
			ActionName = action.DisplayName();
			gain = action.GetGain(GetPlayer().Humanity());
			notify = action.Notify();
			dailyLimit = action.DailyLimit();
			return true;
		}
		return false;
	}
	
	protected bool AdjustActionGain(string Action, EntityAI other, inout float gain, inout bool notify, inout bool ignoreLimit){
		return false;
	}
	protected bool AdjustKillGain(EntityAI other, inout float gain, inout bool notify, inout bool ignoreLimit){
		
		return false;
	}
	
	void NewAction(string Action, EntityAI other){
		float gain = 0;
		string ActionName;
		bool notify = true;
		int dailyLimit = -1;
		if (!GetBaseActionGain(Action,ActionName,gain,notify,dailyLimit)){
			Print("[HAB] Action: " + Action + " has no base humanity defined");
			return;
		}
		bool ignoreLimit = false;
		//Print("[HAB] New Action " + ActionName + " gain: " + gain + " notify" +  notify);
		AdjustActionGain(Action,other,gain,notify,ignoreLimit);
		MissionBaseWorld.Cast(GetGame().GetMission()).NewHABAction(m_player, Action, ActionName, other, gain, notify, ignoreLimit);
		
		if (!GetPlayer().HABData().IncermentAction(Action, dailyLimit) && !ignoreLimit){
			//Print("[HAB] Reached Daily Limit for action " + Action + " Limit: " + dailyLimit );
			gain = 0;
		}
		if (Math.AbsFloat(gain) > 0){
			m_player.IncermentHumanity(gain);
		}
		if (notify && Math.AbsFloat(gain) > 0){
			string message = " ";
			if (gain > 0){
				message = " +";
			} 
			message = message + gain;
			GetPlayer().SendHABNotification(message);
			if (!GetPlayer().IsAlive()){
				UUtil.SendNotification(ActionName, message, GetPlayer().GetIdentity(), Icon());
			}
		}
	}
	
	void NewKillAction(EntityAI other){
		float gain = 0;
		string ActionName;
		bool notify = true;
		PlayerBase otherPlayer = PlayerBase.Cast(other);
		string guid;
		string Action;
		int dailyLimit;
		bool ignoreLimit = false;
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
		if (!GetBaseActionGain(Action,ActionName,gain,notify, dailyLimit)){
			Print("[HAB] Action: " + Action + " has no base humanity defined");
		}

		AdjustKillGain(other,gain,notify,ignoreLimit);
		MissionBaseWorld.Cast(GetGame().GetMission()).NewHABKillAction(m_player,other,gain,notify,ignoreLimit);
		
		if ( guid != "" && !GetPlayer().HABData().IncermentAction("kill|" + guid, HAB_MAXKILLSPERPLAYER) && !ignoreLimit){
			//Print("[HAB] Reached Daily Limit for action " + Action + " Limit: " + HAB_MAXKILLSPERPLAYER );
			gain = 0;
		}
		
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
		bool notify = false;
		
		MissionBaseWorld.Cast(GetGame().GetMission()).OnHABAffinityChange(m_player,oldAffinity,newAffinity,isFirst,notify);
		
		if (notify){
			UUtil.SendNotification("#HAB_TITLE", "Change Affinty", GetPlayer().GetIdentity(), Icon());
		}
	}
	
	void OnLevelChange(int oldLevel, int newLevel, bool isFirst){
		bool notify = true;
		
		MissionBaseWorld.Cast(GetGame().GetMission()).OnHABLevelChange(m_player,oldLevel,newLevel,isFirst,notify);
		
		if (notify){
			string message = "#HAB_HUMANITY_LEVELUP_PRE " + Name();
			if (Math.AbsInt(newLevel) > 0)
				message = message + " #HAB_LEVEL " + Math.AbsInt(newLevel);
			
			UUtil.SendNotification("#HAB_TITLE", message, GetPlayer().GetIdentity(), Icon());
		}
	}

	string Icon(){
		return "set:hab_newicons image:bambi";
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
	

	override bool AdjustActionGain(string Action, EntityAI other, inout float gain, inout bool notify, inout bool ignoreLimit){
		if (super.AdjustActionGain(Action, other, gain, notify, ignoreLimit)){
			return true;
		}
		return false;
	}
	
	
	override string Icon(){
		return "set:hab_newicons image:bambi";
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
		m_Icons.Set(1,"set:hab_newicons image:herolv1");
		m_Icons.Set(2,"set:hab_newicons image:herolv2");
		m_Icons.Set(3,"set:hab_newicons image:herolv3");
		m_Icons.Set(4,"set:hab_newicons image:herolv4");
		m_Icons.Set(5,"set:hab_newicons image:herolv5");
		m_Icons.Set(6,"set:hab_newicons image:herolv6");
		m_Icons.Set(7,"set:hab_newicons image:herolv7");
		m_Icons.Set(8,"set:hab_newicons image:herolv8");
		m_Icons.Set(9,"set:hab_newicons image:herolv9");
		m_Icons.Set(10,"set:hab_newicons image:herolv10");
	}
	
	override bool AdjustActionGain(string Action, EntityAI other, inout float gain, inout bool notify, inout bool ignoreLimit){
		if (super.AdjustActionGain(Action, other, gain, notify, ignoreLimit)){
			return true;
		}
		return false;
	}
	
	override string Icon(){
		string icon;
		if (m_Icons.Find(HeroesAndBandits.GetABSLevel(GetPlayer().Humanity()),icon)){
			return icon;
		}
		return "set:hab_newicons image:hero";
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
		HABActionConfigs.UpdateActionMap("HAB_ACTIONS_BANDIT", Actions);
		m_Icons.Set(1,"set:hab_newicons image:banditlv1");
		m_Icons.Set(2,"set:hab_newicons image:banditlv2");
		m_Icons.Set(3,"set:hab_newicons image:banditlv3");
		m_Icons.Set(4,"set:hab_newicons image:banditlv4");
		m_Icons.Set(5,"set:hab_newicons image:banditlv5");
		m_Icons.Set(6,"set:hab_newicons image:banditlv6");
		m_Icons.Set(7,"set:hab_newicons image:banditlv7");
		m_Icons.Set(8,"set:hab_newicons image:banditlv8");
		m_Icons.Set(9,"set:hab_newicons image:banditlv9");
		m_Icons.Set(10,"set:hab_newicons image:banditlv10");
	}

	override bool AdjustActionGain(string Action, EntityAI other, inout float gain, inout bool notify, inout bool ignoreLimit){
		if (super.AdjustActionGain(Action, other, gain, notify, ignoreLimit)){
			return true;
		}
		return false;
	}
	
	override string Icon(){
		string icon;
		if (m_Icons.Find(HeroesAndBandits.GetABSLevel(GetPlayer().Humanity()),icon)){
			return icon;
		}
		return "set:hab_newicons image:bandit";
	}
	override int Affinity(){
		return HAB_BANDIT;
	}
	override string Name(){
		return "#HAB_BANDIT";
	}
}