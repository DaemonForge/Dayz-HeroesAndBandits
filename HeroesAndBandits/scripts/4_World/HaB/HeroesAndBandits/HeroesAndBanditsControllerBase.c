static int HAB_MAXKILLSPERPLAYER = 3;
static string HEROROLE = "1398513026088439888";
static string BANDITROLE = "1398513095617548328";
static string HEROPATHROLE = "1398446621309538427";
static string BANDITPATHROLE = "1398446456381374545";

class HeroesAndBanditsControllerBase extends Managed {
	protected PlayerBase m_player;
	
	protected autoptr map<string,autoptr HaBActionBase> Actions;
	
	protected autoptr map<int,string> m_Icons;
	
	private bool m_isAwaitingDelayedInit = false;
	
	protected autoptr TStringArray BLOCKEDITEMS = {};
	
	void HeroesAndBanditsControllerBase(PlayerBase player){
		Class.CastTo(m_player,player);
		Actions = new map<string,autoptr HaBActionBase>;
		OnInit();
		m_isAwaitingDelayedInit = true;
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.DelayedInitDo,2000);
	}
	
	void ~HeroesAndBanditsControllerBase(){
		if (m_isAwaitingDelayedInit){
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.DelayedInitDo);
		}
	}
	
	private void DelayedInitDo(){
		m_isAwaitingDelayedInit = false;
		DelayedInit();
	}
	//so you don't have to call super
	protected void DelayedInit(){
		
	}
	
	protected void OnInit(){
		HABActionConfigs.UpdateActionMap("HAB_ACTIONS",Actions);
		m_Icons = new map<int,string>;
		m_Icons.Set(0,"set:hab_newicons image:bambi");
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(GetPlayer().habSyncIcon);
	}
	
	PlayerBase GetPlayer(){
		return m_player;
	}
	
	HABControllerData GetMetaData(){
		return new HABControllerData(-1000,1000,Name(),Icon(),Actions);
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
		Print("[HAB] New Action " + ActionName + " gain: " + gain + " notify" +  notify);
		AdjustActionGain(Action,other,gain,notify,ignoreLimit);
		MissionBaseWorld.Cast(g_Game.GetMission()).NewHABAction(m_player, Action, ActionName, other, gain, notify, ignoreLimit);
		
		if (!GetPlayer().HABData().IncermentAction(Action, dailyLimit) && !ignoreLimit){
			Print("[HAB] Reached Daily Limit for action " + Action + " Limit: " + dailyLimit );
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
		PlayerBase otherPlayer;
		if (Class.CastTo(otherPlayer,other) || Class.CastTo(otherPlayer,other.GetHierarchyRootPlayer())){
		
		}
		string guid;
		string Action = "bambikill";
		int dailyLimit;
		bool ignoreLimit = false;
		if (otherPlayer){
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
		Print(Action);
		if (!GetBaseActionGain(Action, ActionName,gain,notify, dailyLimit)){
			Print("[HAB] Action: " + Action + " has no base humanity defined (Kill)");
		}
		Print(gain);
		Print(notify);

		AdjustKillGain(other,gain,notify,ignoreLimit);
		MissionBaseWorld.Cast(g_Game.GetMission()).NewHABKillAction(m_player,other,gain,notify,ignoreLimit);
		
		if ( guid != "" && !GetPlayer().HABData().IncermentAction("kill|" + guid, HAB_MAXKILLSPERPLAYER, false) && !ignoreLimit){
			Print("[HAB] Reached Daily Limit for action " + "kill|" + guid + " Limit: " + HAB_MAXKILLSPERPLAYER );
			gain = 0;
		}
		if ( !GetPlayer().HABData().IncermentAction(Action, dailyLimit) && !ignoreLimit){
			Print("[HAB] Reached Daily Limit for action " + Action + " Limit: " + dailyLimit );
			gain = 0;
		}
		
		Print(gain);
		m_player.IncermentHumanity(gain);
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
	
	void OnAffinityChange(int oldAffinity, int newAffinity, bool isFirst){
		bool notify = false;
		
		MissionBaseWorld.Cast(g_Game.GetMission()).OnHABAffinityChange(m_player,oldAffinity,newAffinity,isFirst,notify);
		
		if (notify){
			UUtil.SendNotification("#HAB_TITLE", "Change Affinty", GetPlayer().GetIdentity(), Icon());
		}
	}
	
	void OnLevelChange(int oldLevel, int newLevel, bool isFirst){
		bool notify = true;
		
		MissionBaseWorld.Cast(g_Game.GetMission()).OnHABLevelChange(m_player,oldLevel,newLevel,isFirst,notify);
		
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
	
	void OverrideIcons( map<int, string> icons){
		m_Icons = icons;
	}
	
	int ABSLevel(){
		return HeroesAndBandits.GetABSLevel(GetPlayer().Humanity());
	}
	
	int Level(){
		return HeroesAndBandits.GetLevel(GetPlayer().Humanity());
	}

	
	bool CanEquipItem(EntityAI item){
		foreach (string listitem : BLOCKEDITEMS){
			if (listitem && listitem.ToType() && item.IsInherited(listitem.ToType())){
				return false;
			}
		}
		return true;
	}
}
class BambiController extends HeroesAndBanditsControllerBase {
	
	
	protected autoptr TStringArray FLEXACTIONS = {"zombiekill", "huntanimal", "catchfish", "mineore"};
	
	override void OnInit(){
		super.OnInit();
		Print("Init BambiController");
		HABActionConfigs.UpdateActionMap("HAB_ACTIONS_BAMBI",Actions);
		BLOCKEDITEMS = {"Shemagh_Bandit_ColorBase", "Shemagh_Scarf_ColorBase", "Shemagh_Facemask_ColorBase"};
	}
	
	override void DelayedInit(){
		FLEXACTIONS = m_HaBGeneralConfig.FlexActions;
		UDiscordUser dsuser;
		if (Class.CastTo(dsuser, GetPlayer().DiscordUser()) && g_Game.IsDedicatedServer()){
			if (dsuser.HasRole(HEROROLE)){
				U().ds().RemoveRole(GetPlayer().GetHABGUIDCache(), HEROROLE);
			}
			if (dsuser.HasRole(BANDITROLE)){
				U().ds().RemoveRole(GetPlayer().GetHABGUIDCache(), BANDITROLE);
			}
		}
	}

	override HABControllerData GetMetaData(){
		return new HABControllerData(HeroesAndBandits.Levels[0] * -1,HeroesAndBandits.Levels[0],Name(),Icon(),Actions);
	}
	
	override bool AdjustActionGain(string Action, EntityAI other, inout float gain, inout bool notify, inout bool ignoreLimit){
		if (super.AdjustActionGain(Action, other, gain, notify, ignoreLimit)){
			return true;
		}
		UDiscordUser dsuser;
		if (Class.CastTo(dsuser, GetPlayer().DiscordUser()) && g_Game.IsDedicatedServer()){
			if (dsuser.HasRole(HEROPATHROLE)){
				if (FLEXACTIONS.Find(Action) != -1){
					gain = Math.AbsFloat(gain);
					return true;
				}
			}
			if (dsuser.HasRole(BANDITPATHROLE)){
				if (FLEXACTIONS.Find(Action) != -1){
					gain = Math.AbsFloat(gain) * -1;
					return true;
				}
			}
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
		BLOCKEDITEMS = {"Shemagh_Bandit_ColorBase", "Shemagh_Facemask_ColorBase"};
	}
	
	override void DelayedInit(){
		UDiscordUser dsuser;
		if (Class.CastTo(dsuser, GetPlayer().DiscordUser()) && g_Game.IsDedicatedServer()){
			if (!dsuser.HasRole(HEROROLE)){
				U().ds().AddRole(GetPlayer().GetHABGUIDCache(), HEROROLE);
			}
			if (dsuser.HasRole(BANDITROLE)){
				U().ds().RemoveRole(GetPlayer().GetHABGUIDCache(), BANDITROLE);
			}
		}
	}
	
	override bool AdjustActionGain(string Action, EntityAI other, inout float gain, inout bool notify, inout bool ignoreLimit){
		if (super.AdjustActionGain(Action, other, gain, notify, ignoreLimit)){
			return true;
		}
		return false;
	}
	
	override HABControllerData GetMetaData(){
		int max = HeroesAndBandits.Levels.Count() - 1;
		int last = ABSLevel() - 1;
		if (ABSLevel() < max && ABSLevel() >= 1){
			return new HABControllerData(HeroesAndBandits.Levels[last],HeroesAndBandits.Levels[ABSLevel()],Name(),Icon(),Actions);
		} else {
			return new HABControllerData(HeroesAndBandits.Levels[last],int.MAX,Name(),Icon(),Actions);
		}
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
		HABActionConfigs.UpdateActionMap("HAB_ACTIONS_BANDIT", Actions);
		BLOCKEDITEMS = {"Shemagh_Scarf_ColorBase"};
	}
	
	override void DelayedInit(){
		UDiscordUser dsuser;
		if (Class.CastTo(dsuser, GetPlayer().DiscordUser()) && g_Game.IsDedicatedServer()){
			if (dsuser.HasRole(HEROROLE)){
				U().ds().RemoveRole(GetPlayer().GetHABGUIDCache(), HEROROLE);
			}
			if (!dsuser.HasRole(BANDITROLE)){
				U().ds().AddRole(GetPlayer().GetHABGUIDCache(), BANDITROLE);
			}
		}
	}

	override bool AdjustActionGain(string Action, EntityAI other, inout float gain, inout bool notify, inout bool ignoreLimit){
		if (super.AdjustActionGain(Action, other, gain, notify, ignoreLimit)){
			return true;
		}
		return false;
	}
	
	override HABControllerData GetMetaData(){
		int max = HeroesAndBandits.Levels.Count() - 1;
		int last = ABSLevel() - 1;
		if (ABSLevel() < max && ABSLevel() >= 1){
			return new HABControllerData(HeroesAndBandits.Levels[last] * -1,HeroesAndBandits.Levels[ABSLevel()] * -1,Name(),Icon(),Actions);
		} else {
			return new HABControllerData(HeroesAndBandits.Levels[last] * -1,int.MAX,Name(),Icon(),Actions);
		}
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
