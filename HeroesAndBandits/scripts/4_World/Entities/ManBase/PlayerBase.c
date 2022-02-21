static autoptr UApiDBHandler<HeroesAndBanditsPlayerBase> HABPlayerDataHandler = new UApiDBHandler<HeroesAndBanditsPlayerBase>("Humanity", OBJECT_DB);
modded class PlayerBase extends ManBase
{
	
	protected autoptr HeroesAndBanditsPlayerBase m_HABData;
	protected autoptr HeroesAndBanditsControllerBase m_HABControllerBase;
	
	protected string m_HABGUIDCache;
	protected string m_HABNameCache;
	
	protected float m_Humanity = 0;
	protected int m_HaBLevel = 0;
	
	protected bool m_HeroesAndBandits_CanRaiseWeapon = true;
	
	protected int m_hab_LastDataCall = -1;
	
	protected bool m_HeroesAndBandits_Killed = false;
	
	protected int m_HeroesAndBandits_LastBleedingSourceType = -1;
	protected string m_HeroesAndBandits_LastBleedingSourceID;
	protected string m_HeroesAndBandits_Icon;
	
	override void Init()
	{
		super.Init();
		RegisterNetSyncVariableFloat("m_Humanity");
		RegisterNetSyncVariableInt("m_HaBLevel",HAB_BANDIT_MAXLEVEL,HAB_HERO_MAXLEVEL);
	}
	
	void ~PlayerBase(){
		if (HABPlayerDataHandler){
			HABPlayerDataHandler.Cancel(m_hab_LastDataCall);
		}
	}
	override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
		StatUpdateByTime( AnalyticsManagerServer.STAT_PLAYTIME );
		if (!GetGame().IsClient() && HABData()){
			HABPlayerDataHandler.Save(GetHABGUIDCache(), HABData());
		}
    }
	
	string GetHABGUIDCache(){
		return m_HABGUIDCache;
	}
	
	protected void UpdateHumanity(float humanity){
		int oldLevel = HeroesAndBandits.GetLevel(m_Humanity);
		int newLevel = HeroesAndBandits.GetLevel(humanity);
		int oldAffinity = HeroesAndBandits.GetAffinity(m_Humanity);
		int newAffinity = HeroesAndBandits.GetAffinity(humanity);
		m_Humanity = humanity;
		HABData().UpdateHumanity(humanity);
		if (oldLevel != newLevel){
			HABControler().OnLevelChange(oldLevel, newLevel, (HABData().UpdateLevel(newLevel) != 0));
		}
		if (oldAffinity != newAffinity){
			HABControler().OnAffinityChange(oldAffinity,newAffinity,((newAffinity == HAB_HERO && HABData().Max() < 1) || (newAffinity == HAB_BANDIT && HABData().Min() > -1)));
		}
		SetSynchDirty();
	}
	
	void IncermentHumanity(float humanity){
		UpdateHumanity(Humanity() + humanity);
	}
	
	HeroesAndBanditsPlayerBase HABData(){
		return m_HABData;
	}
	
	string GetClientIcon(){
		return m_HeroesAndBandits_Icon;
	}
	
	override void OnPlayerLoaded()
	{
		super.OnPlayerLoaded();
		Print("OnPlayerLoaded");
		if ( GetIdentity() ){ 
			m_HABGUIDCache = GetIdentity().GetId();
			m_HABNameCache = GetIdentity().GetName();
			m_hab_LastDataCall = HABPlayerDataHandler.Load(GetHABGUIDCache(),this,"CBHABData");
		}
		SetSynchDirty();
		if (GetGame().IsClient()){
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this.habSyncIcon); 
		}
	}
	
	void CBHABData(int cid, int status, string oid, HeroesAndBanditsPlayerBase data){
		if (status == UAPI_SUCCESS){
			Class.CastTo(m_HABData,data);
			if (GetGame().IsDedicatedServer()){
				m_Humanity = m_HABData.GetHumanity();
				SetSynchDirty();
				InitHABController();
			}
		} else if (status == UAPI_EMPTY && GetIdentity()){
			m_HABData = new HeroesAndBanditsPlayerBase( GetIdentity().GetId() );
			if (GetGame().IsDedicatedServer()){
				InitHABController();
			}
		}
	}
	
	HeroesAndBanditsControllerBase HABControler(){
		return m_HABControllerBase;
	}
	
	void InitHABController(){
		if (!GetGame().IsDedicatedServer()) Error2("Heroes and Bandits", "Trying to Init Controller on client");
		
		m_HABControllerBase = HeroesAndBandits.Controller(Humanity(),this);
	}
	
	void OnHABLevelChange(int oldLevel, int newLevel, bool isFirst){
		
	}
	
	
	void OnHABAffinityChange( int oldAffinity, int newAffinity, bool isFirst ){
		
	}
	
	float Humanity(){
		return m_Humanity;
	}
	
	
	void NewHABAction(string Action, EntityAI other = NULL){
		if (!HABControler()){
			return;
		}
		Action.ToLower();
		HABControler().NewAction(Action,other);
	}
	void NewHABKillAction(EntityAI other = NULL){
		if (!HABControler()){
			return;
		}
		HABControler().NewKillAction(other);
	}

	bool habCheckGodMod(){
		bool PlayerHasGodMode = false;
		#ifdef JM_COT
			if ( GetGame().IsServer() && m_JMHasGodMode ){
				PlayerHasGodMode = true;
			}
		#endif
		#ifdef VPPADMINTOOLS
			if ( GetGame().IsServer() && hasGodmode ){
				PlayerHasGodMode = true;
			}
		#endif
		#ifdef ZOMBERRY_AT
			if ( GetGame().IsServer() && ZBGodMode ){
				PlayerHasGodMode = true;
			}
		#endif
		return PlayerHasGodMode;
	}
	

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
	}
	
	void HABPreventRaiseWeapon(){
		m_HeroesAndBandits_CanRaiseWeapon = false;
		GetInputController().OverrideRaise( true, false );
		GetInputController().OverrideMeleeEvade( true, false );
	}
	
	void HABAllowRaiseWeapon(){
		m_HeroesAndBandits_CanRaiseWeapon = true;
		GetInputController().OverrideRaise( false, false );
		GetInputController().OverrideMeleeEvade( false, false );
	}
		
	void OnhabEnteredZone(int zoneID, int index = 0)
	{
		
	}
	
	void OnHABLeftZone(int zoneID, int index){
		
	}
	
	void OnHABDeath(){
		
	}
	
	bool IsInHABZone(){
		return false;
	}
	
	override void EEKilled(Object killer)
	{
		if (GetIdentity()){
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this.OnHABDeath);
			/*if (habIsInZone()){
				habLeftZone(-1,0); 
			}*/
		}
		super.EEKilled(killer);
		if (!GetIdentity()){ 
			return;
		} 
		PlayerBase targetPlayer = PlayerBase.Cast(this);
		PlayerBase sourcePlayer;
		if (Class.CastTo(sourcePlayer, killer) || Class.CastTo(sourcePlayer, EntityAI.Cast(killer).GetHierarchyRootPlayer())){
			if ( sourcePlayer == this){
				NewHABAction("Sucide");
			}
			if (!sourcePlayer.GetIdentity() || !GetIdentity()){
				return;
			}
		} else {
			return;
		}
		string weaponName = "";
		bool killedByObject = false;
		bool killedByZombie = false;
		int deathType = habDeathType.Unknown;
		string sourcePlayerID = "";
		m_HeroesAndBandits_Killed = true; //Pervent kills gettting counted twice with Explosions
		if (GetGame().IsServer() && (GetIdentity())){
			if (killer.IsMan())	{
				sourcePlayer = PlayerBase.Cast(killer);
				weaponName = "#HAB_KILLFEED_FISTS";
			} else if (killer.IsWeapon()) {
				sourcePlayer = PlayerBase.Cast(EntityAI.Cast(killer).GetHierarchyParent());
				weaponName = "#HAB_KILLFEED_PRE "+ killer.GetDisplayName();
			} else if (killer.IsMeleeWeapon()) {
				sourcePlayer = PlayerBase.Cast(EntityAI.Cast(killer).GetHierarchyParent());
				weaponName = "#HAB_KILLFEED_PRE "+ killer.GetDisplayName();
			} else if (killer.IsTransport()){
				CarScript vehicle;
				if (Class.CastTo(vehicle, killer))
				{
					weaponName = "#HAB_KILLFEED_PRE "+ vehicle.GetDisplayName();
					if ( vehicle.CrewSize() > 0 )
					{
						PlayerBase driver = PlayerBase.Cast(vehicle.CrewMember( 0 ));
						if ( driver ){
							sourcePlayer = PlayerBase.Cast(driver);
						}
					}
				}
			} else if (killer.IsInherited(TrapBase)){
				TrapBase trap = TrapBase.Cast(killer);
				killedByObject = true;
				sourcePlayerID = trap.habGetActivatedBy();
				weaponName =  "#HAB_KILLFEED_PRE " + trap.GetDisplayName();
			} 
			#ifdef EXPANSIONMOD
				else if ( killer.IsInherited(Expansion_C4_Explosion)){
					Expansion_C4_Explosion expansionExplosive = Expansion_C4_Explosion.Cast(killer);
					if ( expansionExplosive ){
						killedByObject = true;
						sourcePlayerID = expansionExplosive.habGetActivatedBy();
						weaponName = "#HAB_KILLFEED_PRE " + "Home Made Explosive"; //TODO

					}
				}
			#endif
			else if (killer.IsInherited(ZombieBase)){
				killedByZombie = true;
				deathType = habDeathType.Zombie;
			}
			else if (GetBleedingManagerServer()){
				if (GetBleedingManagerServer().GetBleedingSourcesCount() > 0){
					deathType = habDeathType.Bleeding;
					if (m_HeroesAndBandits_LastBleedingSourceType == habDeathType.Zombie){
						killedByZombie = true;
						deathType = habDeathType.ZombieBleeding;
					}
					sourcePlayerID = m_HeroesAndBandits_LastBleedingSourceID;
				}
			}
			else {
				if ( killer )
				{
					
				}
				return;
			}
			
			if (!sourcePlayer && killedByObject){
				//TODO get sourcePlayer if Killed by object
			}
			
			if ( sourcePlayer ) {//Make sure Players are valid
				sourcePlayer.NewHABKillAction(this);
			}
		}
	}
	
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		
		int beforeHitBleedingSources = 0;
		
		string targetPlayerID = "";
		string sourcePlayerID = "";
		bool hitByObject = false;
		bool hitByZombie = false;
		PlayerBase targetPlayer = PlayerBase.Cast(this);
		PlayerBase sourcePlayer;
		
		if (GetGame().IsServer() && GetIdentity() && GetBleedingManagerServer()){
			beforeHitBleedingSources = GetBleedingManagerServer().GetBleedingSourcesCount();
		}
		if ( damageType == DT_EXPLOSION && source && !this.IsAlive() && !m_HeroesAndBandits_Killed && GetGame().IsServer() && GetIdentity()) {
			m_HeroesAndBandits_Killed = true; //Pervent kills gettting counted twice with Explosions
			string weaponName;
			if (source.IsInherited(Grenade_Base)){
				Grenade_Base grenade = Grenade_Base.Cast(source);
				string objectActivatedByID = grenade.habGetActivatedBy();
				weaponName =  "#HAB_KILLFEED_PRE " + grenade.GetType();
				sourcePlayerID = objectActivatedByID;
				targetPlayerID = GetIdentity().GetPlainId();
				int deathType = habDeathType.Unknown;
				string killerAffinity = GetHeroesAndBandits().GetPlayerAffinity(sourcePlayerID);
				if ( sourcePlayerID != "null" )
				{
					if (sourcePlayerID == targetPlayerID){
						deathType = habDeathType.Sucide;
					} else if ( killerAffinity == "hero") {
						deathType = habDeathType.Hero;
					} else if (killerAffinity == "bandit") {
						deathType = habDeathType.Bandit;
					} else if (killerAffinity == "bambi") {
						deathType = habDeathType.Bambi;
					}
					if (sourcePlayerID == targetPlayerID){ //Sucide
						//GetHeroesAndBandits().NewPlayerAction(sourcePlayerID, GetHeroesAndBandits().GetPlayerHeroOrBandit(sourcePlayerID)+"Sucide");
					}else {
						//GetHeroesAndBandits().NewPlayerAction(sourcePlayerID, GetHeroesAndBandits().GetPlayerHeroOrBandit(sourcePlayerID)+"Vs"+GetHeroesAndBandits().GetPlayerHeroOrBandit(targetPlayerID));
					}
				}
			} else {
			}
		} else  if ( damageType == DT_EXPLOSION && !source && !this.IsAlive() && GetGame().IsServer() && GetIdentity() ) {
		}
		
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		int afterHitBleedingSources = 0;
		if (GetGame().IsServer() && GetIdentity() && GetBleedingManagerServer()){
			afterHitBleedingSources = GetBleedingManagerServer().GetBleedingSourcesCount();
		}
				
		if (GetGame().IsServer() && hitByZombie && IsAlive() ){//Heal back after Zombie Hits if guard unless it was a killing blow
			this.AddHealth("","Health", damageResult.GetDamage( "", "Health" ));
			if (beforeHitBleedingSources < afterHitBleedingSources && GetBleedingManagerServer()){
				this.GetBleedingManagerServer().RemoveMostSignificantBleedingSource();
			} 
		} 
		
		if (GetGame().IsServer() && beforeHitBleedingSources < afterHitBleedingSources){
			if (source.IsMan())	{
				sourcePlayer = PlayerBase.Cast(source);
			} else if (source.IsWeapon()) {
				sourcePlayer = PlayerBase.Cast(EntityAI.Cast(source).GetHierarchyParent());
			} else if (source.IsMeleeWeapon()) {
				sourcePlayer = PlayerBase.Cast(EntityAI.Cast(source).GetHierarchyParent());
			} else if (source.IsTransport()){
				CarScript vehicle;
				if (Class.CastTo(vehicle, source))
				{
					if ( vehicle.CrewSize() > 0 )
					{
						PlayerBase driver = PlayerBase.Cast(vehicle.CrewMember( 0 ));
						if ( driver ){
							sourcePlayer = PlayerBase.Cast(driver);
						}
					}
				}
			} else if (source.IsInherited(TrapBase)){
				TrapBase trap = TrapBase.Cast(source);
				hitByObject = true;
				sourcePlayerID = trap.habGetActivatedBy();
			} 
			#ifdef EXPANSIONMOD
				else if ( source.IsInherited(Expansion_C4_Explosion)){
					Expansion_C4_Explosion expansionExplosive = Expansion_C4_Explosion.Cast(source);
					if ( expansionExplosive ){
						hitByObject = true;
						sourcePlayerID = expansionExplosive.habGetActivatedBy();
					}
				}
			#endif
			else if (source.IsInherited(ZombieBase)){
				hitByZombie = true;
				m_HeroesAndBandits_LastBleedingSourceType = habDeathType.Zombie;
			}
			else {
				if ( source )
				{
					
				}
				return;
			}
						
			if (sourcePlayer){
				
				string hitByAffinity = "";
				if (sourcePlayer.GetIdentity()){
					sourcePlayerID = sourcePlayer.GetIdentity().GetId();
					hitByAffinity = GetHeroesAndBandits().GetPlayerAffinity(sourcePlayerID);
				}
				if (sourcePlayerID == targetPlayerID){
					m_HeroesAndBandits_LastBleedingSourceType = habDeathType.Sucide;
					m_HeroesAndBandits_LastBleedingSourceID = sourcePlayerID;
				} else if ( hitByAffinity == "hero") {
					m_HeroesAndBandits_LastBleedingSourceType = habDeathType.Hero;
					m_HeroesAndBandits_LastBleedingSourceID = sourcePlayerID;
				} else if (hitByAffinity == "bandit") {
					m_HeroesAndBandits_LastBleedingSourceType = habDeathType.Bandit;
					m_HeroesAndBandits_LastBleedingSourceID = sourcePlayerID;
				} else if (hitByAffinity == "bambi") {
					m_HeroesAndBandits_LastBleedingSourceType = habDeathType.Bambi;
					m_HeroesAndBandits_LastBleedingSourceID = sourcePlayerID;
				} 
			}
		}
	}

	void habResetBleedingSource(){
	
	}
	
	void habSyncIcon(){
		if (GetGame().IsClient()){
			Print("[HAB] Requesting Icon Sync");
			RPCSingleParam(HAB_SYNCICON, new Param1<bool>(true),true, NULL);
		} else {
			Print("[HAB] Syncing Icon");
			RPCSingleParam(HAB_SYNCICON, new Param1<string>(HABControler().Icon()),true, NULL);
		}
	}
	
	
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		
		if (rpc_type == HAB_SYNCICON && GetGame().IsClient()) {
			Param1<string> iconData;
			if (ctx.Read(iconData)){
				m_HeroesAndBandits_Icon = iconData.param1;
				Print("[HAB] Reciving Icon " + m_HeroesAndBandits_Icon);
			}
		}
		if (rpc_type == HAB_SYNCICON && GetGame().IsDedicatedServer() && sender && HABControler()) {
			Print("[HAB] Syncing Icon with " + sender.GetId());
			RPCSingleParam(HAB_SYNCICON, new Param1<string>(HABControler().Icon()),true, sender);
		}
	}
	
	
	override bool CanReceiveItemIntoHands(EntityAI item_to_hands)
	{
		return super.CanReceiveItemIntoHands(item_to_hands);
	}
	
	override bool CanReleaseAttachment(EntityAI attachment)
	{
		return super.CanReleaseAttachment(attachment);
	}
	
	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);
	}
	
	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		return super.CanReceiveAttachment(attachment, slotId);
	}

	void HaBReplaceDogtag(string tagType){
		
		#ifdef WRDG_DOGTAGS
			Dogtag_Base theCurrentTag = Dogtag_Base.Cast(GetDogtag());
			if (!theCurrentTag || theCurrentTag.GetType() == tagType){
				return;
			}
			bool DogtagKilled = theCurrentTag.IsKilled();
			string DogtagNickName = theCurrentTag.GetNickName();
			int DogtagBirthday = theCurrentTag.GetBirthday();
			int DogtagBloodType = theCurrentTag.GetBloodType();
			string DogtagAffinity = theCurrentTag.GetHaBAffinity();
			float DogtagHumanity = theCurrentTag.GetHaBHumanity();
			theCurrentTag.Delete();
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.HaBSpawnNewDogTags, 600, false, tagType, DogtagKilled, DogtagNickName, DogtagBirthday, DogtagBloodType, DogtagAffinity, DogtagHumanity);
		#endif
	}
	
	void HaBSpawnNewDogTags(string tagType, bool DogtagKilled, string DogtagNickName, int DogtagBirthday, int DogtagBloodType, string DogtagAffinity, float DogtagHumanity){
		
		#ifdef WRDG_DOGTAGS
			int slotId = InventorySlots.GetSlotIdFromString("Dogtag");
			Dogtag_Base theNewTag = Dogtag_Base.Cast(this.GetInventory().CreateAttachmentEx(tagType, slotId));
			
			if (theNewTag){
				theNewTag.SetKilled(DogtagKilled);
				theNewTag.SetNickName(DogtagNickName);
				theNewTag.SetBloodType(DogtagBloodType);
				theNewTag.SetBirthday(DogtagBirthday);
				theNewTag.SetHaBHumanity(DogtagHumanity);
				theNewTag.SetHaBAffinity(DogtagAffinity);
			}
		#endif
	}
};
