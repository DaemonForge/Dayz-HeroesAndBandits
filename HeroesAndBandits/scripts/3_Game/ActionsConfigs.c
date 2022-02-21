class HABActionConfigs extends Managed {
	static protected autoptr map<string, autoptr UApiGlobalHandler<array<autoptr HaBActionBase>>> UAPIHABActionLoaders;

	static protected ref HABActionConfigs Instance;
	
	static protected ref map<string, autoptr array<autoptr HaBActionBase>> Configs;
	
	protected bool AlreadyIsRetrying = false;
	
	private void HABActionConfigs(){};
	private void ~HABActionConfigs(){};
	
	
	static void Init(){
		Print("[HAB] ActionConfigs Init");
		Instance = new HABActionConfigs;
		Instance.OnInit();
	}
	
	void OnLoad(){
		array<UApiGlobalHandler<array<autoptr HaBActionBase>>> loaders = UAPIHABActionLoaders.GetValueArray();
		foreach (auto loader : loaders){
			if (loader){
				loader.Load(this, "CBLoadActions");
			}
		}
	}
	
	static bool GetActionArray(string affinity, out array<autoptr HaBActionBase> actions){
		return Configs.Find(affinity,actions);
	}
	
	static bool UpdateActionMap(string affinity, out map<string,autoptr HaBActionBase> actions){
		if (!actions){ 
			actions = new map<string, autoptr HaBActionBase>;
		}
		array<autoptr HaBActionBase> actionsArray;
		if (GetActionArray(affinity,actionsArray)){
			foreach (HaBActionBase action :  actionsArray){
				if (action){
					actions.Set(action.Name(),action);
				}
			}
			return true;
		}
		return false;
	}
	
	void InitLoaders(){
		AddNewLoader("HAB_ACTIONS_BANDIT");
		AddNewLoader("HAB_ACTIONS_BAMBI");
		AddNewLoader("HAB_ACTIONS_HERO");
		AddNewLoader("HAB_ACTIONS");
	}
	
	static void AddNewLoader(string name){
		if (!UAPIHABActionLoaders) {
			UAPIHABActionLoaders = new map<string, autoptr UApiGlobalHandler<array<autoptr HaBActionBase>>>;
		}
		UAPIHABActionLoaders.Set(name, new UApiGlobalHandler<array<autoptr HaBActionBase>>(name));
	}
	
	void OnInit(){
		InitLoaders();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.OnLoad);
	}
	
	void CBLoadActions(int cid, int status, string action, autoptr array<autoptr HaBActionBase> actions){
		if (status == UAPI_SUCCESS){
			if (!Configs){
				Configs = new map<string, autoptr array<autoptr HaBActionBase>>;
			}
			Configs.Set(action,actions);
		} else if (status == UAPI_EMPTY){
			LoadDefaults(action);
		} else {
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.Retry,4000,false,action);
		}
	}
	
	void LoadDefaults(string action){
		array<autoptr HaBActionBase> defaultActions = new array<autoptr HaBActionBase>;
		if (!Configs ){
			Configs = new map<string, autoptr array<autoptr HaBActionBase>>;
		}
		if (action == "HAB_ACTIONS"){
			defaultActions.Insert(new HaBActionBase("herokill", "Killed Hero", -350, false, HaBActionType.EXACT));
			defaultActions.Insert(new HaBActionBase("banditkill", "Killed Bandit", 350, false, HaBActionType.EXACT));
			defaultActions.Insert(new HaBActionBase("bambikill", "Killed Bambi", -300, false, HaBActionType.EXACT));
			
			defaultActions.Insert(new HaBActionBase("zombiekill", "Killed A Zombie", 5, true, HaBActionType.EXACT));
			defaultActions.Insert(new HaBActionBase("fencepartraid", "Raided A Fence", -50, true, HaBActionType.EXACT));
			defaultActions.Insert(new HaBActionBase("watchtowerpartraid", "Raided A Watchtower", -30, true, HaBActionType.EXACT));
			defaultActions.Insert(new HaBActionBase("medicbandageplayer", "Bandage Player", 25, true, HaBActionType.EXACT));
			defaultActions.Insert(new HaBActionBase("medicgiveblood", "Give Blood", 25, true, HaBActionType.EXACT));
			defaultActions.Insert(new HaBActionBase("medicgivesaline", "Give Saline", 25, true, HaBActionType.EXACT));
			defaultActions.Insert(new HaBActionBase("medicgivecpr", "Give CPR", 75, true, HaBActionType.EXACT));
			defaultActions.Insert(new HaBActionBase("medicfeedtetracycline", "Give Tetracycline", 15, true, HaBActionType.EXACT));
			defaultActions.Insert(new HaBActionBase("medicfeedpainkiller", "Give Painkiller", 15, true, HaBActionType.EXACT));
			defaultActions.Insert(new HaBActionBase("medicfeedcharcoal", "Give Charcoal", 15, true, HaBActionType.EXACT));
			defaultActions.Insert(new HaBActionBase("medicfeedvitamin", "Give Vitamin", 15, true, HaBActionType.EXACT));
			defaultActions.Insert(new HaBActionBase("medicsplintplayer", "Applied Splint", 15, true, HaBActionType.EXACT));
			defaultActions.Insert(new HaBActionBase("huntanimal", "Hunted Animal", 10, true, HaBActionType.ADD));
			
			defaultActions.Insert(new HaBActionBase("death", "Died", 0.1, true, HaBActionType.MULTIPLESUB));
			defaultActions.Insert(new HaBActionBase("playerdeath", "Died To Player", 0.03, true, HaBActionType.MULTIPLESUB));
			defaultActions.Insert(new HaBActionBase("hungerdeath", "Died", 0.1, true, HaBActionType.MULTIPLESUB));
			defaultActions.Insert(new HaBActionBase("toxicdeath", "Died to Contamination Zone", 0.05, true, HaBActionType.MULTIPLESUB));
			defaultActions.Insert(new HaBActionBase("sucide", "Sucided", 0.2, true, HaBActionType.MULTIPLESUB));
			defaultActions.Insert(new HaBActionBase("zombiedeath", "Died To Zombies", 0.07, true, HaBActionType.MULTIPLESUB));
		}
		if (action == "HAB_ACTIONS_HERO"){
			defaultActions.Insert(new HaBActionBase("killhero", "Killed Hero", -1000, true, HaBActionType.EXACT));
			defaultActions.Insert(new HaBActionBase("killbandit", "Killed Bandit", 400, true, HaBActionType.EXACT));
			defaultActions.Insert(new HaBActionBase("killbambi", "Killed Bambi", -200, true, HaBActionType.EXACT));
		}
		if (action == "HAB_ACTIONS_BANDIT"){
			defaultActions.Insert(new HaBActionBase("killhero", "Killed Hero", -350, true, HaBActionType.EXACT));
			defaultActions.Insert(new HaBActionBase("killbandit", "Killed Bandit", -250, true, HaBActionType.EXACT));
			defaultActions.Insert(new HaBActionBase("killbambi", "Killed Bambi", -200, true, HaBActionType.EXACT));
		}
		Configs.Set(action,defaultActions);
		if (UAPIHABActionLoaders.Get(action)){
			UAPIHABActionLoaders.Get(action).Save(defaultActions);
		}
	}
	
	void Retry(string loader){
		Print("[HAB] Retrying to Load " + loader + " Actions");
		if (loader && UAPIHABActionLoaders.Get(loader)){
			UAPIHABActionLoaders.Get(loader).Load(this, "CBLoadActions");
		}
	}
}