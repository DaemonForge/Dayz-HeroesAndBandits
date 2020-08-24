modded class PlayerBase extends ManBase
{
	
	ref array< int > m_HeroesAndBandits_InZones = new ref array< int >; //For new Zones
	private bool  m_HeroesAndBandits_Killed = false;

	private bool  m_HeroesAndBandits_IsGuard = false;
	
	private int  m_HeroesAndBandits_CanRaiseWeaponIndex = -1;
	private bool  m_HeroesAndBandits_CanRaiseWeaponSync = true;
	private bool  m_HeroesAndBandits_CanRaiseWeapon = true;
	
	private int  m_HeroesAndBandits_AIHitSync = 0;
	private int  m_HeroesAndBandits_AIHit = 0;
	
	private bool  m_HeroesAndBandits_AIRaiseWeaponSync = false;
	private bool  m_HeroesAndBandits_AIRaiseWeapon = false;
	
	private int  m_HeroesAndBandits_AIFireWeaponSync = 0;
	private int  m_HeroesAndBandits_AIFireWeapon = 0;
	
	private int m_HeroesAndBandits_AffinityIndex = -1;
	private float m_HeroesAndBandits_AffinityPoints = 0;
	private bool m_HeroesAndBandits_DataLoaded = false;
	private int m_HeroesAndBandits_LevelIndex = -1;
	
	private bool m_HeroesAndBandits_KilledByZone = false;
	private string m_HeroesAndBandits_KilledByZoneName = "";
	private string m_HeroesAndBandits_KilledByGun = "";
	private float m_HeroesAndBandits_LatestZoneDmg = 0;
	
	private bool m_HeroesAndBandits_ChangeAimSync = false;
	private bool m_HeroesAndBandits_ChangeAim = false;
	private float m_HeroesAndBandits_ChangeAimX = 0;
	private float m_HeroesAndBandits_ChangeAimY = 0;

	private int  m_HeroesAndBandits_LastBleedingSourceType = -1;
	private string  m_HeroesAndBandits_LastBleedingSourceID = "";
	
	private bool m_HeroesAndBandits_TraderIsBlocked = false;
	private int m_HeroesAndBandits_TraderIsBlockedIndex = -1;
	
	private float m_HeroesAndBandits_Aggressor = 0;
	
	private bool m_HeroesAndBandits_OverrideItemBlocks = true;
	
	override void Init()
	{
		super.Init();
		RegisterNetSyncVariableBool("m_HeroesAndBandits_IsGuard");
		RegisterNetSyncVariableBool("m_HeroesAndBandits_Killed");
		RegisterNetSyncVariableBool("m_HeroesAndBandits_CanRaiseWeaponSync");
		RegisterNetSyncVariableInt("m_HeroesAndBandits_AffinityIndex");
		RegisterNetSyncVariableFloat("m_HeroesAndBandits_AffinityPoints");
		RegisterNetSyncVariableInt("m_HeroesAndBandits_LevelIndex");
		RegisterNetSyncVariableBool("m_HeroesAndBandits_DataLoaded");
		RegisterNetSyncVariableBool("m_HeroesAndBandits_AIRaiseWeaponSync");
		RegisterNetSyncVariableInt("m_HeroesAndBandits_AIHitSync");
		RegisterNetSyncVariableInt("m_HeroesAndBandits_KilledByZone");
		RegisterNetSyncVariableFloat("m_HeroesAndBandits_LatestZoneDmg");
		RegisterNetSyncVariableInt("m_HeroesAndBandits_AIFireWeaponSync");
		RegisterNetSyncVariableBool("m_HeroesAndBandits_ChangeAimSync");
		RegisterNetSyncVariableFloat("m_HeroesAndBandits_ChangeAimX");
		RegisterNetSyncVariableFloat("m_HeroesAndBandits_ChangeAimY");
		
		RegisterNetSyncVariableBool("m_HeroesAndBandits_TraderIsBlocked");
		
		RegisterNetSyncVariableBool("m_HeroesAndBandits_OverrideItemBlocks");
		
		
		RegisterNetSyncVariableFloat("m_HeroesAndBandits_Aggressor");
	}
	override void OnPlayerLoaded()
	{
		super.OnPlayerLoaded();
		
		if ( GetGame().IsServer() && GetIdentity() ){ 
			HeroesAndBanditsPlayer tempHABPlayer = GetHeroesAndBandits().GetPlayer(GetIdentity().GetPlainId());
			m_HeroesAndBandits_AffinityIndex = tempHABPlayer.getAffinityIndex();
			m_HeroesAndBandits_AffinityPoints = tempHABPlayer.getAffinityPoints(tempHABPlayer.getAffinityName());
			m_HeroesAndBandits_LevelIndex= tempHABPlayer.getLevelIndex();
			m_HeroesAndBandits_DataLoaded = true;
			habPrint("Player: " + GetIdentity().GetPlainId() + " Loaded with Affinty Index of " + m_HeroesAndBandits_AffinityIndex + " Points: " + m_HeroesAndBandits_AffinityPoints, "Debug");
			m_HeroesAndBandits_OverrideItemBlocks = true;
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.habAffinityChange, 1000, false, -1, m_HeroesAndBandits_AffinityIndex); //making sure the player is set up correctly encase settings have changed or new player
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.habResetOverrideItemBlocks, 1800, false);
		}
		SetSynchDirty();
	}
	
	bool habTraderIsBlocked(){
		return m_HeroesAndBandits_TraderIsBlocked;
	}
	
	void habSetTraderBlocked(bool blockTrader = true, int index = 0 ){
		m_HeroesAndBandits_TraderIsBlockedIndex = index;
		m_HeroesAndBandits_TraderIsBlocked = blockTrader;
		SetSynchDirty();
	}
	
	bool habIsGuard(){
		return m_HeroesAndBandits_IsGuard;
	}
	
	void habSetGuard( bool isGaurd = true){
		m_HeroesAndBandits_IsGuard = isGaurd;
		SetSynchDirty();	
	}
	
	int habTopZoneIndex(){
		if (m_HeroesAndBandits_InZones.Count() > 0){
			return m_HeroesAndBandits_InZones.Get(0);
		}
		return -1;
	}
	
	int habGetBleedingSourceType(){
		return m_HeroesAndBandits_LastBleedingSourceType;
	}
	
	string habGetLastBleedingSourceID(){
		return m_HeroesAndBandits_LastBleedingSourceID;
	}
	
	void habResetBleedingSource(){
		m_HeroesAndBandits_LastBleedingSourceType = -1;
		m_HeroesAndBandits_LastBleedingSourceID = "";
	}
	
	array< int > habGetInZones(){
		return m_HeroesAndBandits_InZones;
	}
	
	void habLevelChange( int affinityIndex, float affinityPoints, int levelIndex){
		int oldAffinity = m_HeroesAndBandits_AffinityIndex;
		m_HeroesAndBandits_AffinityIndex = affinityIndex;
		m_HeroesAndBandits_AffinityPoints = affinityPoints;
	
		m_HeroesAndBandits_LevelIndex = levelIndex;
		m_HeroesAndBandits_DataLoaded = true;
		if (oldAffinity != m_HeroesAndBandits_AffinityIndex){
			habPrint("habResetOverrideItemBlocks now true" , "Debug");
			m_HeroesAndBandits_OverrideItemBlocks = true;
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.habAffinityChange, 400, false, oldAffinity, affinityIndex);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.habResetOverrideItemBlocks, 1200, false); //Should Restore on its own this is a double check
		}
		SetSynchDirty();
	}
	
	void habResetOverrideItemBlocks(){
		habPrint("habResetOverrideItemBlocks now false Auto Reset", "Debug");
		m_HeroesAndBandits_OverrideItemBlocks = false;
		SetSynchDirty();
	}
	
	void habAffinityChange( int oldAffinity, int newAffinity ){
		habPrint("Affinity has Changed for player " + GetIdentity().GetPlainId() + " oldAffinity: " + oldAffinity + " newAffinity" + newAffinity, "Debug");
		if (oldAffinity == newAffinity || !GetGame().IsServer()){ 
			habPrint("habResetOverrideItemBlocks now false Affinity is the same", "Debug");
			m_HeroesAndBandits_OverrideItemBlocks = false;
			SetSynchDirty();
			return; 
		}
		bool createItem = false;
		habAffinity tempAffinity = GetHeroesAndBanditsLevels().DefaultAffinity;
		if (GetHeroesAndBanditsSettings().Mode != 1 && newAffinity != -1){
			tempAffinity = GetHeroesAndBanditsLevels().Affinities.Get(newAffinity);
			createItem = true;
		} else if (GetHeroesAndBanditsSettings().Mode != 1){
			createItem = true;
		}
		bool CheckMaskBandit = (!GetHeroesAndBanditsSettings().BanditCanRemoveMask && tempAffinity.Name == "bandit" );
		bool CheckMaskHero = (!GetHeroesAndBanditsSettings().HeroCanRemoveMask && tempAffinity.Name == "hero" );
		bool CheckArmbandBandit = (!GetHeroesAndBanditsSettings().BanditCanRemoveArmBand && tempAffinity.Name == "bandit" );
		bool CheckArmbandHero = (!GetHeroesAndBanditsSettings().HeroCanRemoveArmBand && tempAffinity.Name == "hero" );
		
		bool CheckMaskBambi = (!GetHeroesAndBanditsSettings().BanditCanRemoveMask && !GetHeroesAndBanditsSettings().HeroCanRemoveMask && tempAffinity.Name == "bambi" );
		bool CheckArmbandBambi = (!GetHeroesAndBanditsSettings().BanditCanRemoveArmBand && !GetHeroesAndBanditsSettings().HeroCanRemoveArmBand && tempAffinity.Name == "bambi" );
		int index = -1;
		bool FixMask = false;
		bool FixArmband = false;
		if ( createItem ){
			EntityAI mask = EntityAI.Cast(GetInventory().FindAttachment(InventorySlots.MASK));
			EntityAI armband = EntityAI.Cast(GetInventory().FindAttachment(InventorySlots.ARMBAND));
			if (CheckMaskBandit && GetHeroesAndBanditsSettings().BanditMasks.Count() > 0){ 
				if ( mask ){ 
					habPrint("Player is Bandit and is wearing a mask", "Debug");
					index = -1;
					index = GetHeroesAndBanditsSettings().BanditMasks.Find(mask.GetType());
					if (index == -1){
						habPrint("Dropping mask", "Debug");
						this.GetInventory().DropEntity(InventoryMode.SERVER, this, mask);
						FixMask = true;
					}
				} else { //Not Already wearing a mask
					habPrint("Player is Bandit and is not wearing a mask", "Debug");
					FixMask = true;
				}
			}
			if (CheckMaskHero && GetHeroesAndBanditsSettings().HeroMasks.Count() > 0){
				if ( mask ){
					habPrint("Player is Hero and is wearing a mask", "Debug");
					index = -1;
					index = GetHeroesAndBanditsSettings().HeroMasks.Find(mask.GetType());
					if (index == -1){
						habPrint("Dropping mask", "Debug");
						this.GetInventory().DropEntity(InventoryMode.SERVER, this, mask);
						FixMask = true;
					}
				} else { //Not Already wearing a mask
					habPrint("Player is Hero and is not wearing a mask", "Debug");
					FixMask = true;		
				}
			}
			if (CheckArmbandBandit && GetHeroesAndBanditsSettings().BanditArmBands.Count() > 0){
				if ( armband ){
					index = -1;
					index = GetHeroesAndBanditsSettings().HeroArmBands.Find(armband.GetType());
					if (index == -1){
						this.GetInventory().DropEntity(InventoryMode.SERVER, this, armband);
						FixArmband = true;
					}
				} else { //Not Already wearing an Armband
					FixArmband = true;
				}
			}
			if (CheckArmbandHero && GetHeroesAndBanditsSettings().HeroArmBands.Count() > 0){
				if ( armband ){
					index = -1;
					index = GetHeroesAndBanditsSettings().HeroArmBands.Find(armband.GetType());
					if (index == -1){
						this.GetInventory().DropEntity(InventoryMode.SERVER, this, armband);
						FixArmband = true;
					}
				} else { //Not Already wearing an Armband
					FixArmband = true;
				}
			}
			if (CheckMaskBambi && mask){
				habPrint("Player is Bambi and is wearing a mask", "Debug");
				index = -1;
				index = GetHeroesAndBanditsSettings().HeroMasks.Find(mask.GetType());
				if (index == -1){
					index = GetHeroesAndBanditsSettings().BanditMasks.Find(mask.GetType());
				}
				if (index != -1){
				habPrint("Dropping mask", "Debug");
					this.GetInventory().DropEntity(InventoryMode.SERVER, this, mask);
				}
			}
			if (CheckArmbandBambi && armband){
				index = -1;
				index = GetHeroesAndBanditsSettings().HeroArmBands.Find(armband.GetType());
				if (index == -1){
					index = GetHeroesAndBanditsSettings().BanditArmBands.Find(armband.GetType());
				}
				if (index != -1){
					this.GetInventory().DropEntity(InventoryMode.SERVER, this, armband);
				}
			}
			//Call Laters cause for some reason just does work at the same time as removing the items
			if (FixMask && tempAffinity.Name == "bandit"){
				habPrint("Player is Banidit fixing mask", "Debug");
				m_HeroesAndBandits_OverrideItemBlocks = true;
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.habFixClothing, 150, false, GetHeroesAndBanditsSettings().BanditMasks, InventorySlots.MASK);
			} else if (FixMask && tempAffinity.Name == "hero"){
				habPrint("Player is hero fixing mask", "Debug");
				m_HeroesAndBandits_OverrideItemBlocks = true;
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.habFixClothing, 150, false, GetHeroesAndBanditsSettings().HeroMasks, InventorySlots.MASK);
			}
			if (FixArmband && tempAffinity.Name == "bandit"){
				m_HeroesAndBandits_OverrideItemBlocks = true;
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.habFixClothing, 180, false, GetHeroesAndBanditsSettings().BanditArmBands, InventorySlots.ARMBAND);
			} else if (FixArmband && tempAffinity.Name == "hero"){
				m_HeroesAndBandits_OverrideItemBlocks = true;
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.habFixClothing, 180, false, GetHeroesAndBanditsSettings().HeroArmBands, InventorySlots.ARMBAND);
			}
			if ( (!FixMask && !FixArmband) || tempAffinity.Name == "bambi"){
				habPrint("habResetOverrideItemBlocks now false No fix was required", "Debug");
				m_HeroesAndBandits_OverrideItemBlocks = false;
				
			}
			SetSynchDirty();
		}
	}
	
	void habFixClothing(TStringArray clothingArray, int slotId){
		string itemType = clothingArray.GetRandomElement();
		EntityAI firstAttempt = EntityAI.Cast(this.GetInventory().CreateAttachment(itemType));
		if (!firstAttempt){
			habPrint("Item wasn't created on the first attempt trying second meathod", "Debug");
			EntityAI item = EntityAI.Cast(GetGame().CreateObjectEx(itemType, GetPosition(), ECE_PLACE_ON_SURFACE));
			if (item){
				habPrint("Item created trying to attach to the player", "Debug");
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.GetHumanInventory().TakeEntityAsAttachment, 100, false, InventoryMode.SERVER, item); //maybe this will help with desync?
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.habResetOverrideItemBlocks, 350, false); 
			} else {
				habPrint("Item couldn't get created", "Debug");
				habPrint("habResetOverrideItemBlocks now false", "Debug");
				m_HeroesAndBandits_OverrideItemBlocks = false;
				SetSynchDirty();
			}
		} else {
			habPrint("Item created on first Attempt ", "Debug");
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.habResetOverrideItemBlocks, 250, false); //if I resync to fast I get client desync
		}
	} 
		
	void habCurrentAffinityPointUpdate(float affinityPoints){
		m_HeroesAndBandits_AffinityPoints = affinityPoints;
		SetSynchDirty();
	}
	
	int habGetLevelIndex(){
		return m_HeroesAndBandits_LevelIndex;
	}
	
	int habGetAffinityIndex(){
		return m_HeroesAndBandits_AffinityIndex;
	}
	
	int habGetAffinityPoints(){
		return m_HeroesAndBandits_AffinityPoints;
	}
	
	bool habIsInZone(int zoneID = -1, int index = 0)
	{
		if ( !m_HeroesAndBandits_InZones ){
			return false;
		} else if (m_HeroesAndBandits_InZones.Count() == 0) {
			return false;
		} else if ( zoneID == -1 && m_HeroesAndBandits_InZones.Count() >= 1 ){
			return true;
		}
		return (m_HeroesAndBandits_InZones.Get(index) == zoneID);
	}

	bool habCheckGodMod(){
		bool PlayerHasGodMode = false;
		#ifdef JM_COT
			if ( GetGame().IsServer() && m_JMHasGodMode ){
				habPrint("CheckGodMod COT ADMIN TOOLS ACTIVE", "Debug");
				PlayerHasGodMode = true;
			}
		#endif
		#ifdef VPPADMINTOOLS
			if ( GetGame().IsServer() && hasGodmode ){
				habPrint("CheckGodMod VPP ADMIN TOOLS ACTIVE", "Debug");
				PlayerHasGodMode = true;
			}
		#endif
		#ifdef ZOMBERRY_AT
			if ( GetGame().IsServer() && ZBGodMode ){
				habPrint("CheckGodMod ZOMBERRY ADMIN TOOLS ACTIVE", "Debug");
				PlayerHasGodMode = true;
			}
		#endif
		return PlayerHasGodMode;
	}
	
	
	//code for preventing raising weapons
	bool habCanRaiseWeapon(){
		return m_HeroesAndBandits_CanRaiseWeapon;
	}
	
	void habSetCanRaiseWeapon( bool canRaiseWeapon, int index = 0){
		if (GetGame().IsServer()){
			m_HeroesAndBandits_CanRaiseWeaponIndex = index;
			m_HeroesAndBandits_CanRaiseWeaponSync = canRaiseWeapon;
			SetSynchDirty();
		}
	}
	
	void habSetKilledByZone(string zoneName, bool killedByZone = true){
		if (GetGame().IsServer()){
			m_HeroesAndBandits_KilledByZone = killedByZone;
			m_HeroesAndBandits_KilledByZoneName = zoneName;
			SetSynchDirty();
		}
	}
		
	
	bool habIsKilledByZone(){
		return m_HeroesAndBandits_KilledByZone;
	}
	
	string habGetKilledByZone(){
		return m_HeroesAndBandits_KilledByZoneName;
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		habPrint("Var sync m_HeroesAndBandits_DataLoaded:"  + m_HeroesAndBandits_DataLoaded, "Debug");
		
		
		if ( !m_HeroesAndBandits_CanRaiseWeaponSync && m_HeroesAndBandits_CanRaiseWeapon )
		{
			habPreventRaiseWeapon();
		} 
		else if ( m_HeroesAndBandits_CanRaiseWeaponSync && !m_HeroesAndBandits_CanRaiseWeapon )
		{
			habAllowRaiseWeapon();
		}
		
		if ( !m_HeroesAndBandits_AIRaiseWeaponSync && m_HeroesAndBandits_AIRaiseWeapon )
		{
			habAILowerWeapon();
		} 
		else if ( m_HeroesAndBandits_AIRaiseWeaponSync && !m_HeroesAndBandits_AIRaiseWeapon )
		{
			habAIRaiseWeapon();
		}
		
		if ( m_HeroesAndBandits_AIHitSync != m_HeroesAndBandits_AIHit){
			habHitByAIClient();
		}
		
		if ( m_HeroesAndBandits_AIFireWeaponSync != m_HeroesAndBandits_AIFireWeapon){
			habAIFireWeapon();
		}
		
		if ( m_HeroesAndBandits_ChangeAimSync != m_HeroesAndBandits_ChangeAim){
			habAIAimWeaponClient();
		}
		
	}
	
	void habPreventRaiseWeapon(){
		m_HeroesAndBandits_CanRaiseWeapon = false;
		GetInputController().OverrideRaise( true, false );
		GetInputController().OverrideMeleeEvade( true, false );
	}
	
	void habAllowRaiseWeapon(){
		m_HeroesAndBandits_CanRaiseWeapon = true;
		GetInputController().OverrideRaise( false, false );
		GetInputController().OverrideMeleeEvade( false, false );
	}
		
	void habEnteredZone(int zoneID, int index = 0)
	{
		int maxIndex =  m_HeroesAndBandits_InZones.Count() - 1;
		if ( index <= maxIndex ){
			habLeftZone(index);
		}
		habPrint("Player Entered Zone" + zoneID + " at index " + index, "Debug");
		m_HeroesAndBandits_InZones.Insert(zoneID);
		if ( GetHeroesAndBanditsSettings().DebugLogs ){
			m_HeroesAndBandits_InZones.Debug();
		}
	}
	
	void habLeftZone(int index){
		int maxIndex =  m_HeroesAndBandits_InZones.Count() - 1;
		if ( index == maxIndex ){
			habPrint("Removing Player from zone" + index, "Debug");
			m_HeroesAndBandits_InZones.Remove(index);
			if ( GetHeroesAndBanditsSettings().DebugLogs ){
				m_HeroesAndBandits_InZones.Debug();
			}
		} else if ( index < maxIndex ){
			habPrint("Removing Player from all subzones", "Debug");
			m_HeroesAndBandits_InZones.Remove(index);
			for ( int i = maxIndex; i >= index; i-- ){
				m_HeroesAndBandits_InZones.Remove(i);
			}
			if ( GetHeroesAndBanditsSettings().DebugLogs ){
				m_HeroesAndBandits_InZones.Debug();
			}
		} else {
			habPrint("Trying to leave zone not in" + index, "Exception");
			if ( GetHeroesAndBanditsSettings().DebugLogs ){
				m_HeroesAndBandits_InZones.Debug();
			}
		}
		
		if (m_HeroesAndBandits_CanRaiseWeaponIndex >= index){
			habSetCanRaiseWeapon( true, -1);
		}
		
		if (m_HeroesAndBandits_TraderIsBlockedIndex >= index){
			habSetTraderBlocked( false, -1 );
		}
	
	}
	
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
		if (!GetIdentity()){ 
			if (m_HeroesAndBandits_IsGuard){
				
			} else { 
				return;
			}
		} 
		PlayerBase targetPlayer = PlayerBase.Cast(this);
		PlayerBase sourcePlayer;
		string targetPlayerID = "";
		string sourcePlayerID = "";
		string weaponName = "";
		bool killedByObject = false;
		bool killedByZombie = false;
		int deathType = habDeathType.ByUnknown;
		
		if (targetPlayer && targetPlayer.GetIdentity()) {
			targetPlayerID = targetPlayer.GetIdentity().GetPlainId();
		}
		
		m_HeroesAndBandits_Killed = true; //Pervent kills gettting counted twice with Explosions
		if (GetGame().IsServer() && (GetIdentity() || m_HeroesAndBandits_IsGuard)){
			if (killer.IsMan())	{
				sourcePlayer = PlayerBase.Cast(killer);
				weaponName = "#HAB_KILLFEED_FISTS";
			} else if (killer.IsWeapon()) {
				sourcePlayer = PlayerBase.Cast(EntityAI.Cast(killer).GetHierarchyParent());
				weaponName = "#HAB_KILLFEED_PRE "+ killer.GetDisplayName();
			} else if (killer.IsMeleeWeapon()) {
				sourcePlayer = PlayerBase.Cast(EntityAI.Cast(killer).GetHierarchyParent());
				weaponName = "#HAB_KILLFEED_PRE "+ killer.GetDisplayName();
			} else if (killer.IsTransport() && !m_HeroesAndBandits_IsGuard){
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
				habPrint("Player " + targetPlayerID + " Killed by " + weaponName + " placed by " + sourcePlayerID,"Debug");
			} 
			#ifdef EXPANSIONMOD
				else if ( killer.IsInherited(Expansion_C4_Explosion)){
					Expansion_C4_Explosion expansionExplosive = Expansion_C4_Explosion.Cast(killer);
					if ( expansionExplosive ){
						killedByObject = true;
						sourcePlayerID = expansionExplosive.habGetActivatedBy();
						weaponName = "#HAB_KILLFEED_PRE " + "Home Made Explosive"; //TODO

						habPrint("Player " + targetPlayerID + " Killed by " + weaponName + " placed by " + sourcePlayerID, "Debug");
					}
				}
			#endif
			else if (killer.IsInherited(ZombieBase)){
				killedByZombie = true;
				deathType = habDeathType.ByZombie;
			}
			else if (GetBleedingManagerServer().GetBleedingSourcesCount() > 0){
				deathType = habDeathType.ByBleeding;
				if (m_HeroesAndBandits_LastBleedingSourceType == habDeathType.ByZombie){
					killedByZombie = true;
					deathType = habDeathType.ByZombieBleeding;
				}
				sourcePlayerID = m_HeroesAndBandits_LastBleedingSourceID;
			}
			else {
				if ( killer )
				{
					habPrint("Player " + targetPlayerID + " Killed by " + killer.GetType() ,"Debug");
				}
				return;
			}
			if (m_HeroesAndBandits_IsGuard){
				if (sourcePlayer.GetIdentity()){
					GetHeroesAndBandits().NewPlayerAction(sourcePlayer.GetIdentity().GetPlainId(), "GuardKill");
				}
				return;
			}
			
			if (( sourcePlayer && targetPlayer ) || ( killedByObject && targetPlayer )) {//Make sure Players are valid
				if ( !killedByObject && sourcePlayer ) {
					sourcePlayerID = sourcePlayer.GetIdentity().GetPlainId();
				} else {
					//Something went wrong perventing a crash
					habPrint("Something went wrong with Player Killed" ,"Debug");
					return;
				}
				
				string killerAffinity = GetHeroesAndBandits().GetPlayerAffinity(sourcePlayerID);
				if (m_HeroesAndBandits_KilledByZone){
					deathType = habDeathType.ByGuardAI;
				} else if (sourcePlayerID == targetPlayerID){
					deathType = habDeathType.BySucide;
				} else if ( killerAffinity == "hero") {
					deathType = habDeathType.ByHero;
				} else if (killerAffinity == "bandit") {
					deathType = habDeathType.ByBandit;
				} else if (killerAffinity == "bambi") {
					deathType = habDeathType.ByBambi;
				} else {
					deathType = habDeathType.ByOtherAffinity;
				}
				
				if (m_HeroesAndBandits_KilledByZone){
					   GetHeroesAndBandits().TriggerKillFeed(m_HeroesAndBandits_KilledByZoneName, targetPlayerID, m_HeroesAndBandits_KilledByGun, deathType);
				} else if (sourcePlayerID == targetPlayerID){ //Sucide
					if ( targetPlayer && !targetPlayer.IsInVehicle() ){//If not in Vehicle Crash
						GetHeroesAndBandits().NewPlayerAction(targetPlayerID, GetHeroesAndBandits().GetPlayerHeroOrBandit(targetPlayerID)+"Sucide");
						GetHeroesAndBandits().TriggerSucideFeed(targetPlayerID);
					} 
				}else {
					GetHeroesAndBandits().NewPlayerAction(sourcePlayerID, GetHeroesAndBandits().GetPlayerHeroOrBandit(sourcePlayerID)+"Vs"+GetHeroesAndBandits().GetPlayerHeroOrBandit(targetPlayerID));
					GetHeroesAndBandits().TriggerKillFeed(sourcePlayerID, targetPlayerID, weaponName, deathType);
				}
			}
		}
		if (GetIdentity() && habIsInZone()){
			habLeftZone(0); 
			habSetCanRaiseWeapon(true, -1);
		}
	}
	
	void habGuardKillZombie(EntityAI zombie){
		ZombieBase Zoombie = ZombieBase.Cast(zombie); //Cause I can't use Zombie
		if (Zoombie){
			Zoombie.SetHealth("","Health",0);
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
		
		if (source.IsInherited(ZombieBase) && habIsGuard()){
			hitByZombie = true;
			habGuardKillZombie(source);
		}
		
		if (targetPlayer && targetPlayer.GetIdentity()) {
			targetPlayerID = targetPlayer.GetIdentity().GetPlainId();
		}
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
				habPrint("Player " + GetIdentity().GetPlainId() + " Killed by " + weaponName + " placed by " + objectActivatedByID,"Debug");
				sourcePlayerID = objectActivatedByID;
				targetPlayerID = GetIdentity().GetPlainId();
				int deathType = habDeathType.ByUnknown;
				string killerAffinity = GetHeroesAndBandits().GetPlayerAffinity(sourcePlayerID);
				if ( sourcePlayerID != "null" )
				{
					if (sourcePlayerID == targetPlayerID){
						deathType = habDeathType.BySucide;
					} else if ( killerAffinity == "hero") {
						deathType = habDeathType.ByHero;
					} else if (killerAffinity == "bandit") {
						deathType = habDeathType.ByBandit;
					} else if (killerAffinity == "bambi") {
						deathType = habDeathType.ByBambi;
					} else {
						deathType = habDeathType.ByOtherAffinity;
					}
					if (sourcePlayerID == targetPlayerID){ //Sucide
						GetHeroesAndBandits().NewPlayerAction(sourcePlayerID, GetHeroesAndBandits().GetPlayerHeroOrBandit(sourcePlayerID)+"Sucide");
						GetHeroesAndBandits().TriggerSucideFeed(sourcePlayerID);
					}else {
						GetHeroesAndBandits().NewPlayerAction(sourcePlayerID, GetHeroesAndBandits().GetPlayerHeroOrBandit(sourcePlayerID)+"Vs"+GetHeroesAndBandits().GetPlayerHeroOrBandit(targetPlayerID));
						GetHeroesAndBandits().TriggerKillFeed(sourcePlayerID, targetPlayerID, weaponName, deathType);
					}
				}
			} else {
				habPrint( "" + GetIdentity().GetPlainId() + " killed by Explosion with " + source.GetType(), "Debug");
			}
		} else  if ( damageType == DT_EXPLOSION && !source && !this.IsAlive() && GetGame().IsServer() && GetIdentity() ) {
			habPrint( "" + GetIdentity().GetPlainId() + " killed by Explosion with no source", "Debug");
		}
		
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		int afterHitBleedingSources = 0;
		if (GetGame().IsServer() && GetIdentity() && GetBleedingManagerServer()){
			afterHitBleedingSources = GetBleedingManagerServer().GetBleedingSourcesCount();
		}
				
		if (GetGame().IsServer() && hitByZombie  && habIsGuard() && IsAlive() ){//Heal back after Zombie Hits if guard unless it was a killing blow
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
				habPrint("Player " + targetPlayer.GetIdentity().GetPlainId() + " Hit by TrapBase placed by " + sourcePlayerID,"Debug");
			} 
			#ifdef EXPANSIONMOD
				else if ( source.IsInherited(Expansion_C4_Explosion)){
					Expansion_C4_Explosion expansionExplosive = Expansion_C4_Explosion.Cast(source);
					if ( expansionExplosive ){
						hitByObject = true;
						sourcePlayerID = expansionExplosive.habGetActivatedBy();
						
						habPrint("Player " + targetPlayer.GetIdentity().GetPlainId() + " Hit by Expansion_C4_Explosion placed by " + sourcePlayerID, "Debug");
					}
				}
			#endif
			else if (source.IsInherited(ZombieBase)){
				hitByZombie = true;
				m_HeroesAndBandits_LastBleedingSourceType = habDeathType.ByZombie;
			}
			else {
				if ( source )
				{
					habPrint("Player " + targetPlayer.GetIdentity().GetPlainId() + " Hit by " + source.GetType() ,"Debug");
				}
				return;
			}
						
			if (sourcePlayer){
				
				string hitByAffinity = "";
				if (sourcePlayer.GetIdentity()){
					sourcePlayerID = sourcePlayer.GetIdentity().GetPlainId();
					hitByAffinity = GetHeroesAndBandits().GetPlayerAffinity(sourcePlayerID);
				}
				if (sourcePlayerID == targetPlayerID){
					m_HeroesAndBandits_LastBleedingSourceType = habDeathType.BySucide;
					m_HeroesAndBandits_LastBleedingSourceID = sourcePlayerID;
				} else if ( hitByAffinity == "hero") {
					m_HeroesAndBandits_LastBleedingSourceType = habDeathType.ByHero;
					m_HeroesAndBandits_LastBleedingSourceID = sourcePlayerID;
				} else if (hitByAffinity == "bandit") {
					m_HeroesAndBandits_LastBleedingSourceType = habDeathType.ByBandit;
					m_HeroesAndBandits_LastBleedingSourceID = sourcePlayerID;
				} else if (hitByAffinity == "bambi") {
					m_HeroesAndBandits_LastBleedingSourceType = habDeathType.ByBambi;
					m_HeroesAndBandits_LastBleedingSourceID = sourcePlayerID;
				} else {
					m_HeroesAndBandits_LastBleedingSourceType = habDeathType.ByOtherAffinity;
					m_HeroesAndBandits_LastBleedingSourceID = sourcePlayerID;
				}
			}
		}
	}

	
	void habHitByAI(float dmg, HeroesAndBanditsGuard source, string hitZone, string ammo)
	{	
		if (GetIdentity()){
			habPrint(GetIdentity().GetName() + " hit in " + hitZone + " from " + source.ZoneName + " for " + dmg + " damage", "Debug");
		}	
		float bleed = Math.RandomFloat(0,1);
		if ( hitZone != "" && GetBleedingManagerServer() && bleed < 0.23)
		{
			GetBleedingManagerServer().AttemptAddBleedingSourceBySelection(hitZone);
			m_HeroesAndBandits_LastBleedingSourceType = habDeathType.ByGuardAI;
			m_HeroesAndBandits_LastBleedingSourceID = source.ZoneName;
		}
		
		if ( m_ActionManager )
			m_ActionManager.Interrupt();
		
		m_HeroesAndBandits_AIHitSync++;
		m_HeroesAndBandits_AIHit = m_HeroesAndBandits_AIHitSync;
		if (dmg > 20){
			SendSoundEvent(EPlayerSoundEventID.TAKING_DMG_HEAVY);
		} else {
			SendSoundEvent(EPlayerSoundEventID.TAKING_DMG_LIGHT);
		}
		DayZPlayerSyncJunctures.SendDamageHit(this, 0, habGetAnimHitDir(source), false);
		m_HeroesAndBandits_KilledByZone = true;
		m_HeroesAndBandits_KilledByZoneName = source.ZoneName;
		m_HeroesAndBandits_KilledByGun = "#HAB_KILLFEED_PRE " + source.GetWeaponName();
		m_HeroesAndBandits_LatestZoneDmg = dmg;
		DecreaseHealth("","Health",dmg);
		if (IsAlive()){// since I need to set it before it kills the player reset it back if the player didn't die
			m_HeroesAndBandits_KilledByZone = false;
			m_HeroesAndBandits_KilledByGun = "";
		}
		SetSynchDirty();
	}
	
	float habGetAnimHitDir(HeroesAndBanditsGuard source)
	{
		float habAnimHitDir;
		//! direction
		vector targetDirection = GetDirection();
		vector toSourceDirection = (source.GetPosition() - GetPosition());

		targetDirection[1] = 0;
		toSourceDirection[1] = 0;

		targetDirection.Normalize();
		toSourceDirection.Normalize();

		float cosFi = vector.Dot(targetDirection, toSourceDirection);
		vector cross = targetDirection * toSourceDirection;

		habAnimHitDir = Math.Acos(cosFi) * Math.RAD2DEG;
		if (cross[1] < 0)
			habAnimHitDir = -habAnimHitDir;
		
		return habAnimHitDir;
	}

	
	void habHitByAIClient(){
		habPrint("Calling habHitByAIClient dmg is " + m_HeroesAndBandits_LatestZoneDmg, "Debug");
		m_HeroesAndBandits_AIHit = m_HeroesAndBandits_AIHitSync;
		if (m_HeroesAndBandits_LatestZoneDmg > 20){
			RequestSoundEvent(EPlayerSoundEventID.TAKING_DMG_HEAVY);
		} else {
			RequestSoundEvent(EPlayerSoundEventID.TAKING_DMG_LIGHT);
		}

	}
	
	// making a safe way to grab the player withing the player class
	HeroesAndBanditsPlayer GetHaBPlayer()
	{
		if (GetGame().IsServer()){
			if (GetIdentity()){
				return GetHeroesAndBandits().GetPlayer(GetIdentity().GetPlainId());
			} 
		} else {
			if (g_HeroesAndBanditsPlayer){
				return g_HeroesAndBanditsPlayer;
			} else {
				habPrint("Player not defined on client","debug");
			}
		}
		return null;
	}
	
	override bool CanReceiveItemIntoHands(EntityAI item_to_hands)
	{
		if (!GetIdentity() || !GetHeroesAndBanditsLevels() || !GetHeroesAndBanditsSettings() || !m_HeroesAndBandits_DataLoaded){
			habPrint("CanReleaseAttachment Settings Not Definied - " + item_to_hands.GetType(), "Debug");
			return super.CanReceiveItemIntoHands(item_to_hands);
		}
		habAffinity tempAffinity = GetHeroesAndBanditsLevels().DefaultAffinity;
		if (GetHeroesAndBanditsSettings().Mode != 1 && m_HeroesAndBandits_AffinityIndex != -1){
			tempAffinity = GetHeroesAndBanditsLevels().Affinities.Get(m_HeroesAndBandits_AffinityIndex);
		} else if (GetHeroesAndBanditsSettings().Mode == 1){ //This is a bit more performance heavy so only useing if need to
			if (GetHaBPlayer().checkItem(item_to_hands.GetType(), "inhands")){
				habPrint("CanReceiveItemIntoHands Item Blocked - "  + item_to_hands.GetType(), "Debug");
				return super.CanReceiveItemIntoHands(item_to_hands);
			} else {
				return false;
			}
		}
		if (tempAffinity.checkItem(m_HeroesAndBandits_AffinityPoints, item_to_hands.GetType(), "inhands"))
		{
			return super.CanReceiveItemIntoHands(item_to_hands);
		}
		return false;
	}
	
	override bool CanReleaseAttachment(EntityAI attachment)
	{
		if (!GetIdentity()){
			habPrint("CanReleaseAttachment GetIdentity Not Definied"  + attachment.GetType(), "Debug" );
			return super.CanReleaseAttachment(attachment);
		}
		if (!GetHeroesAndBanditsLevels()){
			habPrint("CanReleaseAttachment GetHeroesAndBanditsLevels Not Definied"  + attachment.GetType(), "Debug" );
			return super.CanReleaseAttachment(attachment);
		}
		if (!GetHeroesAndBanditsSettings()){
			habPrint("CanReleaseAttachment GetHeroesAndBanditsSettings Not Definied"  + attachment.GetType(), "Debug" );
			return super.CanReleaseAttachment(attachment);
		}
		if (!m_HeroesAndBandits_DataLoaded){
			habPrint("CanReleaseAttachment m_HeroesAndBandits_DataLoaded Not Definied"  + attachment.GetType(), "Debug" );
			return super.CanReleaseAttachment(attachment);
		}
		if (m_HeroesAndBandits_OverrideItemBlocks){
			habPrint("CanReleaseAttachment Override Item Blocks Enabled"  + attachment.GetType(), "Debug" );
			return super.CanReleaseAttachment(attachment);
		}
		if ( GetHeroesAndBanditsSettings().Mode == 1 ){
			habPrint("CanReleaseAttachment Mode == 1", "Debug");
			return super.CanReleaseAttachment(attachment);
		}
		habAffinity tempAffinity = GetHeroesAndBanditsLevels().DefaultAffinity;
		if (m_HeroesAndBandits_AffinityIndex != -1){
			tempAffinity = GetHeroesAndBanditsLevels().Affinities.Get(m_HeroesAndBandits_AffinityIndex);
		}
		ClothingBase mask = ClothingBase.Cast(GetInventory().FindAttachment(InventorySlots.MASK));
		EntityAI itemInHands = GetHumanInventory().GetEntityInHands();
		int index = -1;
		if (mask){
			if (attachment == mask){
				if (!GetHeroesAndBanditsSettings().BanditCanRemoveMask && tempAffinity.Name == "bandit"){
					habPrint("CanReleaseAttachment Item Blocked - "  + attachment.GetType(), "Debug");
					if (itemInHands){
						index = GetHeroesAndBanditsSettings().BanditMasks.Find(itemInHands.GetType());
						if (index != -1){
							habPrint("CanReleaseAttachment Item In Hands - " + itemInHands.GetType() + " attachment: "  + attachment.GetType(), "Debug");
							return super.CanReleaseAttachment(attachment); 
						}
					}
					return false;
				}
				if (!GetHeroesAndBanditsSettings().HeroCanRemoveMask && tempAffinity.Name == "hero"){
					if (itemInHands){
						index = GetHeroesAndBanditsSettings().HeroMasks.Find(itemInHands.GetType());
						if (index != -1){
							habPrint("CanReleaseAttachment Item In Hands - " + itemInHands.GetType() + " attachment: "  + attachment.GetType(), "Debug");
							return super.CanReleaseAttachment(attachment); 
						}
					}
					
					habPrint("CanReleaseAttachment Item Blocked - "  + attachment.GetType(), "Debug");
					return false;
				}
			}
		}
		ClothingBase armband = ClothingBase.Cast(GetInventory().FindAttachment(InventorySlots.ARMBAND));
		if (armband){
			if (attachment == armband){
				if (!GetHeroesAndBanditsSettings().BanditCanRemoveArmBand && tempAffinity.Name == "bandit"){
					habPrint("CanReleaseAttachment Item Blocked - "  + attachment.GetType(), "Debug");
					if (itemInHands){
						index = GetHeroesAndBanditsSettings().BanditArmBands.Find(itemInHands.GetType());
						if (index != -1){
							habPrint("CanReleaseAttachment Item In Hands - " + itemInHands.GetType() + " attachment: "  + attachment.GetType(), "Debug");
							return super.CanReleaseAttachment(attachment); 
						}
					}
					return false;
				}
				if (!GetHeroesAndBanditsSettings().HeroCanRemoveArmBand && tempAffinity.Name == "hero"){
					if (itemInHands){
						index = GetHeroesAndBanditsSettings().HeroArmBands.Find(itemInHands.GetType());
						if (index != -1){
							habPrint("CanReleaseAttachment Item In Hands - " + itemInHands.GetType() + " attachment: "  + attachment.GetType(), "Debug");
							return super.CanReleaseAttachment(attachment); 
						}
					}
					
					habPrint("CanReleaseAttachment Item Blocked - "  + attachment.GetType(), "Debug");
					return false;
				}
			}
		}

		return super.CanReleaseAttachment(attachment);
	}
	
	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);
		habPrint("Item " + item.GetType() + " Removed from " + slot_name, "Debug");
		if ((slot_name == "mask" || slot_name ==  "Armband") && GetHeroesAndBanditsSettings() && GetIdentity()){
			if (GetGame().IsServer()){
				m_HeroesAndBandits_OverrideItemBlocks = true;
				SetSynchDirty();
				GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater(this.habRestoreClothing, 100, false, item, slot_name); //Call later so the item is actually removed
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.habResetOverrideItemBlocks, 800, false); //Should Restore on its own this is a double check
			}
		}
	}
	
	void habRestoreClothing(EntityAI item, string slot_name){
		string slotName = slot_name; 
		slotName.ToLower();
		EntityAI detachedItem =  EntityAI.Cast(item);
		habAffinity tempAffinity = GetHeroesAndBanditsLevels().DefaultAffinity;
		if (GetHeroesAndBanditsSettings().Mode != 1 && m_HeroesAndBandits_AffinityIndex != -1){
			tempAffinity = GetHeroesAndBanditsLevels().Affinities.Get(m_HeroesAndBandits_AffinityIndex);
		} else if (GetHeroesAndBanditsSettings().Mode == 1){
			return; //Sorry not ready for affinities mode
		}
		bool CheckMaskBandit = (!GetHeroesAndBanditsSettings().BanditCanRemoveMask && tempAffinity.Name == "bandit" );
		bool CheckMaskHero = (!GetHeroesAndBanditsSettings().HeroCanRemoveMask && tempAffinity.Name == "hero" );
		bool CheckArmbandBandit = (!GetHeroesAndBanditsSettings().BanditCanRemoveArmBand && tempAffinity.Name == "bandit" );
		bool CheckArmbandHero = (!GetHeroesAndBanditsSettings().HeroCanRemoveArmBand && tempAffinity.Name == "hero" );
		habPrint("habRestoreClothing CheckMaskBandit: " + CheckMaskBandit + " CheckMaskHero: " + CheckMaskHero + " CheckArmbandBandit: " +CheckArmbandBandit + " CheckArmbandBandit: " +CheckArmbandBandit, "Debug");
		int index = -1;
		if (slotName == "mask" && ( CheckMaskBandit || CheckMaskHero )){
			EntityAI mask = EntityAI.Cast(FindAttachmentBySlotName(slot_name));
			if ( mask ){
				if ( CheckMaskHero ){
					index = GetHeroesAndBanditsSettings().HeroMasks.Find(mask.GetType());
					
				} else if (CheckMaskBandit){
					index = GetHeroesAndBanditsSettings().BanditMasks.Find(mask.GetType());
				}
				if (index != -1){
					habPrint("habRestoreClothing Item Valid index:" + index, "Debug");
					return;
				}
			}
		} else if (slotName == "armband" && ( CheckArmbandBandit || CheckArmbandHero )){
			EntityAI armband = EntityAI.Cast(FindAttachmentBySlotName(slot_name));
			if ( armband ){
				if ( CheckArmbandHero ){
					index = GetHeroesAndBanditsSettings().HeroArmBands.Find(armband.GetType());
				} else if (CheckArmbandBandit){
					index = GetHeroesAndBanditsSettings().BanditArmBands.Find(armband.GetType());
				}
				if (index != -1){
					habPrint("habRestoreClothing Item Valid index:" + index, "Debug");
					return;
				}
			}
		} else {
			return;
		}
		int slot_id = -1; 
		if ( slot_name == "mask" ) {
		habPrint("habRestoreClothing slot_name:" + slot_name, "Debug");
			slot_id = InventorySlots.MASK;
		} else if ( slot_name == "Armband" ) {
			habPrint("habRestoreClothing slot_name:" + slot_name, "Debug");
			slot_id = InventorySlots.ARMBAND;
		}
		
		if ( slot_id == -1) {
			habPrint("habRestoreClothing slot_name: " + slot_name + " slot_id: " + slot_id, "Debug");
			return;
		}
		
		habPrint("habRestoreClothing detachedItem:" + detachedItem.GetType(), "Debug");
		GetHumanInventory().TakeEntityAsAttachment(InventoryMode.SERVER, detachedItem);
		m_HeroesAndBandits_OverrideItemBlocks = false;
		SetSynchDirty();
	}
	
	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		if (!GetIdentity()){
			habPrint("CanReleaseAttachment GetIdentity Not Definied"  + attachment.GetType(), "Debug" );
			return super.CanReceiveAttachment(attachment, slotId);
		}
		if (!GetHeroesAndBanditsLevels()){
			habPrint("CanReceiveAttachment GetHeroesAndBanditsLevels Not Definied"  + attachment.GetType(), "Debug" );
			return super.CanReceiveAttachment(attachment, slotId);
		}
		if (!GetHeroesAndBanditsSettings()){
			habPrint("CanReceiveAttachment GetHeroesAndBanditsSettings Not Definied"  + attachment.GetType(), "Debug" );
			return super.CanReceiveAttachment(attachment, slotId);
		}
		if (!m_HeroesAndBandits_DataLoaded){
			habPrint("CanReceiveAttachment m_HeroesAndBandits_DataLoaded Not Definied"  + attachment.GetType(), "Debug" );
			return super.CanReceiveAttachment(attachment, slotId);
		}
		if (m_HeroesAndBandits_OverrideItemBlocks){
			habPrint("CanReceiveAttachment Override Item Blocks Enabled"  + attachment.GetType(), "Debug" );
			return super.CanReceiveAttachment(attachment, slotId);
		}
		
		habAffinity tempAffinity = GetHeroesAndBanditsLevels().DefaultAffinity;
		if (GetHeroesAndBanditsSettings().Mode != 1 && m_HeroesAndBandits_AffinityIndex != -1){
			tempAffinity = GetHeroesAndBanditsLevels().Affinities.Get(m_HeroesAndBandits_AffinityIndex);
		} else if (GetHeroesAndBanditsSettings().Mode == 1){ //This is a bit more performance heavy so only useing if need to
			if (GetHaBPlayer().checkItem(attachment.GetType(), "attach")){
				return super.CanReceiveAttachment(attachment, slotId);
			} else {
				habPrint("CanReceiveAttachment Item Blocked - "  + attachment.GetType(), "Debug");
				return false;
			}
		}
		if (tempAffinity.checkItem(m_HeroesAndBandits_AffinityPoints, attachment.GetType(), "attach"))
		{
			int index = -1;
			string itemType = attachment.GetType();
			bool whitelistMode = false;
			if (slotId == InventorySlots.MASK){
				
				if (!GetHeroesAndBanditsSettings().BanditCanRemoveMask && tempAffinity.Name == "bandit"){
					index = GetHeroesAndBanditsSettings().BanditMasks.Find(itemType);
					whitelistMode = true;
					habPrint("CanReceiveAttachment Item Mask Check - index: " + index + " Attachment "  + attachment.GetType(), "Debug");
				}
				if (!GetHeroesAndBanditsSettings().HeroCanRemoveMask && tempAffinity.Name == "hero"){
					index = GetHeroesAndBanditsSettings().HeroMasks.Find(itemType);
					whitelistMode = true;
					habPrint("CanReceiveAttachment Item Mask Check - index: " + index + " Attachment "  + attachment.GetType(), "Debug");
				}
				
				if (index != -1){
					return super.CanReceiveAttachment(attachment, slotId);
				} else if ( whitelistMode ){
					return false;
				}
			}
			if (slotId == InventorySlots.ARMBAND){
				if (!GetHeroesAndBanditsSettings().BanditCanRemoveArmBand && tempAffinity.Name == "bandit"){
					index = GetHeroesAndBanditsSettings().BanditArmBands.Find(itemType);
					whitelistMode = true;
					habPrint("CanReceiveAttachment Item Mask Check - index: " + index + " Attachment "  + attachment.GetType(), "Debug");
				}
				if (!GetHeroesAndBanditsSettings().HeroCanRemoveArmBand && tempAffinity.Name == "hero"){
					index = GetHeroesAndBanditsSettings().HeroArmBands.Find(itemType);
					whitelistMode = true;
					habPrint("CanReceiveAttachment Item Mask Check - index: " + index + " Attachment "  + attachment.GetType(), "Debug");
				}
				if (index != -1){
					return super.CanReceiveAttachment(attachment, slotId);
				} else if ( whitelistMode ){
					return false;
				}
			}
			return super.CanReceiveAttachment(attachment, slotId);
		}
		return false;
	}

	
	void habAILowerWeaponServer(){
		habPrint("habAILowerWeaponServer called", "Debug");
		m_HeroesAndBandits_AIRaiseWeaponSync = false;
		m_HeroesAndBandits_AIRaiseWeapon = false;
		GetCommand_Move().ForceStance(DayZPlayerConstants.STANCEIDX_ERECT);
		//GetInputController().OverrideRaise( true, false );
		SetSynchDirty();
	}
	
	void habAILowerWeapon(){
		habPrint("habAILowerWeapon called", "Debug");
		m_HeroesAndBandits_AIRaiseWeapon = false;
		GetCommand_Move().ForceStance(DayZPlayerConstants.STANCEIDX_ERECT);
		//GetInputController().OverrideRaise( true, false );
	}
	
	void habAIFireWeaponServer(){
		m_HeroesAndBandits_AIFireWeaponSync++;
		m_HeroesAndBandits_AIFireWeapon = m_HeroesAndBandits_AIFireWeaponSync;
		SetSynchDirty();
	}
	
	
	void habAIFireWeapon(){
		m_HeroesAndBandits_AIFireWeapon = m_HeroesAndBandits_AIFireWeaponSync;
		habPrint("habAIFireWeapon called", "Debug");
		Weapon_Base gunInHands = Weapon_Base.Cast(this.GetHumanInventory().GetEntityInHands());
		if (gunInHands){
			ItemBase suppressor = gunInHands.GetAttachedSuppressor();
			int muzzleType = gunInHands.GetCurrentMuzzle();
			string ammoType = GetGame().ConfigGetTextOut( "CfgMagazines " + gunInHands.GetChamberAmmoTypeName(muzzleType) + " ammo" );
			
			habPrint("habAIFireWeapon gunInHands: " + gunInHands.GetType() + " muzzleType: " + muzzleType + " ammoType: " + ammoType + " GetChamberAmmoTypeName: " +  gunInHands.GetChamberAmmoTypeName(muzzleType), "Debug");
			ItemBase.PlayFireParticles(gunInHands, muzzleType, ammoType, gunInHands, suppressor, "CfgWeapons" );
			//gunInHands.PlayFireParticles(gunInHands, muzzleType, ammoType, NULL, NULL, "CfgVehicles" );
			//GetInputController().OverrideRaise( true, false );
		} else {
			habPrint("habAIFireWeapon called Guns in hand not defined", "Debug");
		}
	}
	
	void habAIRaiseWeaponServer(){
		habPrint("habAIRaiseWeaponServer called", "Debug");
		m_HeroesAndBandits_AIRaiseWeaponSync = true;
		m_HeroesAndBandits_AIRaiseWeapon = true;
		GetCommand_Move().ForceStance(DayZPlayerConstants.STANCEIDX_RAISEDERECT);
		//GetInputController().OverrideRaise( true, true );
		SetSynchDirty();
	}
	

	void habAIRaiseWeapon(){
		habPrint("habAIRaiseWeapon called", "Debug");
		m_HeroesAndBandits_AIRaiseWeapon = true;
		GetCommand_Move().ForceStance(DayZPlayerConstants.STANCEIDX_RAISEDERECT);
		//GetInputController().OverrideRaise( true, true );
	}
	
	void habAIAimWeaponServer(float x, float y = 0){
		m_HeroesAndBandits_ChangeAimSync = !m_HeroesAndBandits_ChangeAimSync;
		m_HeroesAndBandits_ChangeAim = m_HeroesAndBandits_ChangeAimSync;
		m_HeroesAndBandits_ChangeAimX = x;
		m_HeroesAndBandits_ChangeAimY = y;
		bool doOverride = true;
		if (m_HeroesAndBandits_ChangeAimX == 0){
			doOverride = false;
		}
		//this.GetInputController().OverrideMovementAngle(doOverride, m_HeroesAndBandits_ChangeAimX);
		//this.GetInputController().OverrideMovementSpeed(doOverride, 1);
		//this.GetInputController().OverrideAimChangeX(true, x);
		//this.GetInputController().OverrideAimChangeY(true, y);
		SetSynchDirty();
	}
	
	void habAIAimWeaponClient(){
		m_HeroesAndBandits_ChangeAim = m_HeroesAndBandits_ChangeAimSync;
		bool doOverride = true;
		if (m_HeroesAndBandits_ChangeAimX == 0){
			doOverride = false;
		} 
		//this.GetInputController().OverrideMovementAngle(doOverride, m_HeroesAndBandits_ChangeAimX);
		//this.GetInputController().OverrideMovementSpeed(doOverride, 1);
	}
};
