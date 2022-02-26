modded class PlayerBase extends ManBase
{	
	protected bool m_HeroesAndBandits_CanRaiseWeapon = true;
	
	protected int m_hab_LastDataCall = -1;
	
	protected bool m_HeroesAndBandits_Killed = false;
	
	protected int m_HeroesAndBandits_LastBleedingSourceType = -1;
	protected string m_HeroesAndBandits_LastBleedingSourceID;
	protected string m_HeroesAndBandits_Icon;
	protected string m_HeroesAndBandits_Name;
	
	
	void ~PlayerBase(){
		if (HABPlayerDataHandler){
			HABPlayerDataHandler.Cancel(m_hab_LastDataCall);
		}
	}
	override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
		//SaveHABData();
    }
	
	override protected void UpdateHumanity(float humanity){
		int oldLevel = HeroesAndBandits.GetLevel(m_Humanity);
		int newLevel = HeroesAndBandits.GetLevel(humanity);
		int oldAffinity = HeroesAndBandits.GetAffinity(m_Humanity);
		int newAffinity = HeroesAndBandits.GetAffinity(humanity);
		m_Humanity = humanity;
		HABData().UpdateHumanity(humanity);
		if (oldAffinity != newAffinity){
			bool HasBeenHero = (HABData().Max() < 1);
			bool HasBeenBandit = (HABData().Min() > -1);
			bool FirstTimeHero = (newAffinity == HAB_HERO && !HasBeenHero);
			bool FirstTimeBandit = (newAffinity == HAB_BANDIT && !HasBeenHero);
			bool isFirst = (FirstTimeBandit || FirstTimeHero);
			OnHABAffinityChange(oldLevel, newLevel,isFirst);
		}
		if (oldLevel != newLevel){
			OnHABLevelChange(oldLevel, newLevel, (HABData().UpdateLevel(newLevel) != 0));
		}
		SetSynchDirty();
	}
	
	void IncermentHumanity(float humanity){
		UpdateHumanity(Humanity() + humanity);
	}
	
	string GetClientIcon(){
		return m_HeroesAndBandits_Icon;
	}
	string GetClientAffinityName(){
		return m_HeroesAndBandits_Name;
	}
	
	
	override void OnPlayerLoaded()
	{
		super.OnPlayerLoaded();
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
	
	void RefreshHABData(){
		if ( GetIdentity() ){ 
			HABPlayerDataHandler.Load(GetHABGUIDCache(),this,"CBHABData");
		}
	}
	
	void CBHABData(int cid, int status, string oid, HeroesAndBanditsPlayerBase data){
		if (status == UAPI_SUCCESS){
			Class.CastTo(m_HABData,data);
			m_HABData.InitDailyGains();
			if (GetGame().IsDedicatedServer()){
				m_Humanity = m_HABData.GetHumanity();
				m_HaBLevel = HeroesAndBandits.GetLevel(m_Humanity);
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
	
	void NewHABAction(string Action, EntityAI other = NULL){
		if (!HABContoller()){
			return;
		}
		Action.ToLower();
		HABContoller().NewAction(Action,other);
	}
	
	void NewHABKillAction(EntityAI other = NULL){
		if (!HABContoller()){
			return;
		}
		HABContoller().NewKillAction(other);
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
	
	void OnHABDeath(int DeathType = habDeathType.Unknown, EntityAI other = NULL){
		if (DeathType == habDeathType.Unknown){
			if (GetStatEnergy().Get() < 1 || GetStatWater().Get() < 1){
				DeathType = habDeathType.Hunger;
			}
			if ((GetAgents() & eAgents.CHEMICAL_POISON) == eAgents.CHEMICAL_POISON){
				DeathType = habDeathType.ToxicZone;
			}
		}
		switch (DeathType){
			case habDeathType.Sucide:
				Print("[HAB] On Death: Sucide");
				NewHABAction("sucide",other);
			break;
			case habDeathType.Bambi:
				Print("[HAB] On Death: Bambi");
				NewHABAction("playerdeath",other);
			break;
			case habDeathType.Hero:
				Print("[HAB] On Death: Hero");
				NewHABAction("playerdeath",other);
			break;
			case habDeathType.Bandit:
				Print("[HAB] On Death: Bandit");
				NewHABAction("playerdeath",other);
			break;
			case habDeathType.AI:
				Print("[HAB] On Death: AI");
				NewHABAction("death",other);
			break;
			case habDeathType.Zombie:
				Print("[HAB] On Death: Zombie");
				NewHABAction("zombiedeath",other);
			break;
			case habDeathType.Animal:
				Print("[HAB] On Death: Animal");
				NewHABAction("death",other);
			break;
			case habDeathType.Other:
				Print("[HAB] On Death: Other");
				NewHABAction("death",other);
			break;
			case habDeathType.Bleeding:
				Print("[HAB] On Death: Bleeding");
				NewHABAction("death",other);
			break;
			case habDeathType.LegacyAI:
				Print("[HAB] On Death: LegacyAI");
				NewHABAction("death",other);
			break;
			case habDeathType.CarCrash:
				Print("[HAB] On Death: CarCrash");
				NewHABAction("death",other);
			break;
			case habDeathType.Falling:
				Print("[HAB] On Death: Falling");
				NewHABAction("death",other);
			break;
			case habDeathType.ZombieBleeding:
				Print("[HAB] On Death: ZombieBleeding");
				NewHABAction("zombiedeath",other);
			break;
			case habDeathType.Hunger:
				Print("[HAB] On Death: Hunger");
				NewHABAction("hungerdeath",other);
			break;
			case habDeathType.ToxicZone:
				Print("[HAB] On Death: ToxicZone");
				NewHABAction("toxicdeath",other);
			break;
			default:
				Print("[HAB] On Death: Default");
				NewHABAction("death",other);
			break;
		}
		SaveHABData();
	}
	
	bool IsInHABZone(){
		return false;
	}
	
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
		if (!HABContoller() || m_HeroesAndBandits_Killed){ 
			return;
		} 
		PlayerBase targetPlayer = PlayerBase.Cast(this);
		PlayerBase sourcePlayer;
		string weaponName = "";
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
				sourcePlayerID = trap.habGetActivatedBy();
				weaponName =  "#HAB_KILLFEED_PRE " + trap.GetDisplayName();
			} 
			else if (killer.IsInherited(ZombieBase)){
				deathType = habDeathType.Zombie;
			}
			else if (killer.IsInherited(AnimalBase)){
				deathType = habDeathType.Animal;
			}
			else {
				if ( killer )
				{
					
				}
				return;
			}
			
			float blood = GetHealth("","Blood");
			if (blood < 2){
				deathType = habDeathType.Bleeding;
				if (m_HeroesAndBandits_LastBleedingSourceType == habDeathType.Zombie){
					deathType = habDeathType.ZombieBleeding;
				}
				if (m_HeroesAndBandits_LastBleedingSourceType == habDeathType.Animal){
					deathType = habDeathType.Animal;
				}
				sourcePlayerID = m_HeroesAndBandits_LastBleedingSourceID;
			}
			if (!sourcePlayer && sourcePlayerID != ""){
				//TODO get sourcePlayer if Killed by object
				sourcePlayer = PlayerBase.Cast(UUtil.FindPlayer(sourcePlayerID));
			}
			
			if ( sourcePlayer ) {//Make sure Players are valid
				if (!sourcePlayer.GetIdentity() ){
					deathType = habDeathType.AI;
				} else if (sourcePlayer == this){
					deathType = habDeathType.Unknown;
				} else if ( deathType == habDeathType.Unknown ) {
					deathType = habDeathType.Bambi;
					sourcePlayer.NewHABKillAction(this);
					if (sourcePlayer.HABAffinity() == HAB_HERO){
						deathType = habDeathType.Hero;
					} else if (sourcePlayer.HABAffinity() == HAB_BANDIT){
						deathType = habDeathType.Bandit;
					}
				}
			}
			if (m_Suicide){
				deathType = habDeathType.Sucide;
			}
			OnHABDeath(deathType,sourcePlayer);
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
				weaponName =  "#HAB_KILLFEED_PRE " + grenade.GetType();
				sourcePlayerID = grenade.habGetActivatedBy();
				targetPlayerID = GetIdentity().GetId();
				int deathType = habDeathType.Unknown;
				if ( sourcePlayerID != "null" && sourcePlayerID != "") {
					if (sourcePlayerID == targetPlayerID) {
						OnHABDeath(habDeathType.Sucide);
					} else if (sourcePlayerID != targetPlayerID) {
						deathType = habDeathType.Bambi;
						sourcePlayer = PlayerBase.Cast( UUtil.FindPlayer( sourcePlayerID ));
						if ( sourcePlayer ){
							sourcePlayer.NewHABKillAction(this);
							if (sourcePlayer.HABAffinity() == HAB_HERO){
								deathType = habDeathType.Hero;
							} else if (sourcePlayer.HABAffinity() == HAB_BANDIT){
								deathType = habDeathType.Bandit;
							}
						}
						OnHABDeath(deathType, sourcePlayer);
					}
					
				}
			}
		}		
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		int afterHitBleedingSources = 0;
		if (GetGame().IsServer() && GetIdentity() && GetBleedingManagerServer()){
			afterHitBleedingSources = GetBleedingManagerServer().GetBleedingSourcesCount();
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
			else if (source.IsInherited(ZombieBase)){
				hitByZombie = true;
				m_HeroesAndBandits_LastBleedingSourceType = habDeathType.Zombie;
			}
			else if (source.IsInherited(AnimalBase)){
				hitByZombie = true;
				m_HeroesAndBandits_LastBleedingSourceType = habDeathType.Animal;
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
				} else {
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
			RPCSingleParam(HAB_SYNCICON, new Param1<bool>(true),true, NULL);
		} else {
			RPCSingleParam(HAB_SYNCICON, new Param2<string,string>(HABContoller().Icon(),HABContoller().Name()),true, NULL);
		}
	}
	
	
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		
		if (rpc_type == HAB_SYNCICON && GetGame().IsClient()) {
			Param2<string,string> iconData;
			if (ctx.Read(iconData)){
				m_HeroesAndBandits_Icon = iconData.param1;
				m_HeroesAndBandits_Name = iconData.param2;
			}
		}
		if (rpc_type == HAB_SYNCICON && GetGame().IsDedicatedServer() && sender && HABContoller()) {
			RPCSingleParam(HAB_SYNCICON, new Param2<string,string>(HABContoller().Icon(),HABContoller().Name()),true, sender);
		}
		if (rpc_type == HAB_NOTIFICATION && GetGame().IsClient()) {
			Param1<string> notificationText;
			if (ctx.Read(notificationText)){
				CreateHABNotification(notificationText.param1);
			}
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

	
	void SendHABNotification(string text){
		if (!GetIdentity()) return;
		
		RPCSingleParam(HAB_NOTIFICATION, new Param1<string>(text),true, GetIdentity());
	}
	
	protected void CreateHABNotification(string text){
		m_HABStatusBarIconWidget.CreateNotification(text);
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
