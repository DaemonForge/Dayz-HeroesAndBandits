modded class PlayerBase
{
	ref array< int > m_HeroesAndBandits_InZones = new ref array< int >; //For new Zones
	private bool  m_HeroesAndBandits_Killed = false;
	
	private int  m_HeroesAndBandits_CanRaiseWeaponIndex = -1;
	private bool  m_HeroesAndBandits_CanRaiseWeaponSync = true;
	private bool  m_HeroesAndBandits_CanRaiseWeapon = true;
	
	private int  m_HeroesAndBandits_AIHitSync = 0;
	private int  m_HeroesAndBandits_AIHit = 0;
	
	private bool  m_HeroesAndBandits_AIRaiseWeaponSync = false;
	private bool  m_HeroesAndBandits_AIRaiseWeapon = false;
	
	private int m_HeroesAndBandits_AffinityIndex = -1;
	private float m_HeroesAndBandits_AffinityPoints = 0;
	private bool m_HeroesAndBandits_DataLoaded = false;
	private int m_HeroesAndBandits_LevelIndex = -1;
	
	private bool m_HeroesAndBandits_KilledByZone = false;
	private string m_HeroesAndBandits_KilledByZoneName = "";
	private string m_HeroesAndBandits_KilledByGun = "";
	
	override void Init()
	{
		super.Init();
		RegisterNetSyncVariableBool("m_HeroesAndBandits_Killed");
		RegisterNetSyncVariableBool("m_HeroesAndBandits_CanRaiseWeaponSync");
		RegisterNetSyncVariableInt("m_HeroesAndBandits_AffinityIndex");
		RegisterNetSyncVariableFloat("m_HeroesAndBandits_AffinityPoints");
		RegisterNetSyncVariableInt("m_HeroesAndBandits_LevelIndex");
		RegisterNetSyncVariableBool("m_HeroesAndBandits_DataLoaded");
		RegisterNetSyncVariableBool("m_HeroesAndBandits_AIRaiseWeaponSync");
		RegisterNetSyncVariableInt("m_HeroesAndBandits_AIHitSync");
		RegisterNetSyncVariableInt("m_HeroesAndBandits_KilledByZone");
	}
	
	override void OnSelectPlayer()
	{
		super.OnSelectPlayer();
		if (GetGame().IsServer() && GetIdentity() ){ 
			HeroesAndBanditsPlayer tempHABPlayer = GetHeroesAndBandits().GetPlayer(GetIdentity().GetPlainId());
			m_HeroesAndBandits_AffinityIndex = tempHABPlayer.getAffinityIndex();
			m_HeroesAndBandits_AffinityPoints = tempHABPlayer.getAffinityPoints(tempHABPlayer.getAffinityName());
			m_HeroesAndBandits_LevelIndex= tempHABPlayer.getLevelIndex();
			m_HeroesAndBandits_DataLoaded = true;
			habPrint("Player: " + GetIdentity().GetPlainId() + " Loaded with Affinty Index of " + m_HeroesAndBandits_AffinityIndex + " Points: " + m_HeroesAndBandits_AffinityPoints, "Debug");
			SetSynchDirty();	
		}
	}
	
	int habTopZoneIndex(){
		if (m_HeroesAndBandits_InZones.Count() > 0){
			return m_HeroesAndBandits_InZones.Get(0);
		}
		return -1;
	}
	
	
	array< int > habGetInZones(){
		return m_HeroesAndBandits_InZones;
	}
	
	void habLevelChange( int affinityIndex, float affinityPoints, int levelIndex){
		m_HeroesAndBandits_AffinityIndex = affinityIndex;
		m_HeroesAndBandits_AffinityPoints = affinityPoints;
	
		m_HeroesAndBandits_LevelIndex = levelIndex;

		SetSynchDirty();
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
	
	bool habIsKilledByZone(){
		return m_HeroesAndBandits_KilledByZone;
	}
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		
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
			habHitByAIClient( 6.0 );
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
	}
	
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
		if (!GetIdentity()){ return; } //If this isn't Player return
		m_HeroesAndBandits_Killed = true; //Pervent kills gettting counted twice with Explosions
		if (GetGame().IsServer() && GetIdentity()){
			bool killedByObject = false;
			string objectPlacerID = "";
			PlayerBase sourcePlayer;
			PlayerBase targetPlayer = this;
			string weaponName = "";
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
				objectPlacerID = trap.habGetActivatedBy();
				weaponName =  "#HAB_KILLFEED_PRE " + trap.GetDisplayName();
				habPrint("Player " + targetPlayer.GetIdentity().GetPlainId() + " Killed by " + weaponName + " placed by " + objectPlacerID,"Debug");
			} 
			#ifdef EXPANSIONMOD
				else if ( killer.IsInherited(Expansion_C4_Explosion)){
					Expansion_C4_Explosion expansionExplosive = Expansion_C4_Explosion.Cast(killer);
					if ( expansionExplosive ){
						killedByObject = true;
						objectPlacerID = expansionExplosive.habGetActivatedBy();
						weaponName = "#HAB_KILLFEED_PRE " + "Home Made Explosive"; //TODO

						habPrint("Player " + targetPlayer.GetIdentity().GetPlainId() + " Killed by " + weaponName + " placed by " + objectPlacerID, "Debug");
					}
				}
			#endif
			else {
				if ( killer )
				{
					habPrint("Player " + targetPlayer.GetIdentity().GetPlainId() + " Killed by " + killer.GetType() ,"Debug");
				}
				return;
			}
			
			if (( sourcePlayer && targetPlayer ) || ( killedByObject && targetPlayer )) {//Make sure Players are valid
				string sourcePlayerID;
				if (killedByObject){
					sourcePlayerID = objectPlacerID;
				}else if ( sourcePlayer ) {
					sourcePlayerID = sourcePlayer.GetIdentity().GetPlainId();
				} else {
					//Something went wrong perventing a crash
					habPrint("Something went wrong with Player Killed" ,"Debug");
					return;
				}

				string targetPlayerID = targetPlayer.GetIdentity().GetPlainId();
				if (m_HeroesAndBandits_KilledByZone){
					   GetHeroesAndBandits().TriggerKillFeed(m_HeroesAndBandits_KilledByZoneName, targetPlayerID, m_HeroesAndBandits_KilledByGun);
				} else if (sourcePlayerID == targetPlayerID){ //Sucide
					if ( targetPlayer && !targetPlayer.IsInVehicle() ){//If not in Vehicle Crash
						GetHeroesAndBandits().NewPlayerAction(targetPlayerID, GetHeroesAndBandits().GetPlayerHeroOrBandit(targetPlayerID)+"Sucide");
						GetHeroesAndBandits().TriggerSucideFeed(targetPlayerID);
					} 
				}else {
					GetHeroesAndBandits().NewPlayerAction(sourcePlayerID, GetHeroesAndBandits().GetPlayerHeroOrBandit(sourcePlayerID)+"Vs"+GetHeroesAndBandits().GetPlayerHeroOrBandit(targetPlayerID));
					GetHeroesAndBandits().TriggerKillFeed(sourcePlayerID, targetPlayerID, weaponName);
				}
			}
		}
	}
	
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{

		if ( damageType == DT_EXPLOSION && source && !this.IsAlive() && !m_HeroesAndBandits_Killed && GetGame().IsServer() && GetIdentity()) {
			m_HeroesAndBandits_Killed = true; //Pervent kills gettting counted twice with Explosions
			string sourcePlayerID;
			string targetPlayerID;
			string weaponName;
			if (source.IsInherited(Grenade_Base)){
				Grenade_Base grenade = Grenade_Base.Cast(source);
				string objectActivatedByID = grenade.habGetActivatedBy();
				weaponName =  "#HAB_KILLFEED_PRE " + grenade.GetType();
				habPrint("Player " + GetIdentity().GetPlainId() + " Killed by " + weaponName + " placed by " + objectActivatedByID,"Debug");
				sourcePlayerID = objectActivatedByID;
				targetPlayerID = GetIdentity().GetPlainId();
				if ( sourcePlayerID != "null" )
				{
					if (sourcePlayerID == targetPlayerID){ //Sucide
						GetHeroesAndBandits().NewPlayerAction(sourcePlayerID, GetHeroesAndBandits().GetPlayerHeroOrBandit(sourcePlayerID)+"Sucide");
						GetHeroesAndBandits().TriggerSucideFeed(sourcePlayerID);
					}else {
						GetHeroesAndBandits().NewPlayerAction(sourcePlayerID, GetHeroesAndBandits().GetPlayerHeroOrBandit(sourcePlayerID)+"Vs"+GetHeroesAndBandits().GetPlayerHeroOrBandit(targetPlayerID));
						GetHeroesAndBandits().TriggerKillFeed(sourcePlayerID, targetPlayerID, weaponName);
					}
				}
			} else {
				habPrint( "" + GetIdentity().GetPlainId() + " killed by Explosion with " + source.GetType(), "Debug");
			}
		} else  if ( damageType == DT_EXPLOSION && !source && !this.IsAlive() && GetGame().IsServer() && GetIdentity() ) {
			habPrint( "" + GetIdentity().GetPlainId() + " killed by Explosion with no source", "Debug");
		}
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}

	void habHitByAI(string zoneName, float dmg, HeroesAndBanditsGuard source, string dmgZone, string ammo, string zoneImage = "")
	{	
		if (GetIdentity()){
			habPrint(GetIdentity().GetName() + " hit by " + ammo + " from " + zoneName + " for " + dmg + " damage", "Debug");
		}	
		int bleed = Math.RandomInt(0,100);
		if ( dmg > 5 && GetBleedingManagerServer() && bleed < 20)
		{
			TStringArray bleedingSources = {"LeftForeArmRoll","RightFoot","LeftFoot","RightForeArmRoll","Neck","Spine2","Head"};
			GetBleedingManagerServer().AttemptAddBleedingSourceBySelection(bleedingSources.GetRandomElement());
		}
		
		if( m_ActionManager )
			m_ActionManager.Interrupt();
		
		m_HeroesAndBandits_AIHitSync++;
		m_HeroesAndBandits_AIHit = m_HeroesAndBandits_AIHitSync;
		if (dmg > 30){
			SendSoundEvent(EPlayerSoundEventID.TAKING_DMG_HEAVY);
		} else {
			SendSoundEvent(EPlayerSoundEventID.TAKING_DMG_LIGHT);
		}
		AddHealth("","Health",-dmg);
		if (!IsAlive()){
			m_HeroesAndBandits_KilledByZone = true;
			m_HeroesAndBandits_KilledByZoneName = zoneName;
			m_HeroesAndBandits_KilledByGun = "#HAB_KILLFEED_PRE " + source.GetWeaponName();
		}
		SetSynchDirty();
	}
	
	void habHitByAIClient( float dmg ){
		habPrint("Calling habHitByAIClient dmg is " + dmg, "Debug");
		m_HeroesAndBandits_AIHit = m_HeroesAndBandits_AIHitSync;
		if (dmg > 33){
			RequestSoundEvent(EPlayerSoundEventID.TAKING_DMG_HEAVY, true );
		} else {
			RequestSoundEvent(EPlayerSoundEventID.TAKING_DMG_LIGHT, true );
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

	// Doesn't seem to work removing for now will circle back at a later time
	/*
	override bool CanReceiveItemIntoCargo(EntityAI cargo)
	{
		if (!GetIdentity() || !GetHeroesAndBanditsLevels() || !GetHeroesAndBanditsSettings() || !m_HeroesAndBandits_DataLoaded){
			habPrint("CanReceiveItemIntoCargo Settings Not Definied - "  + cargo.GetType(), "Debug");
			return super.CanReceiveItemIntoCargo(cargo);
		}
		habAffinity tempAffinity = GetHeroesAndBanditsLevels().DefaultAffinity;
		if (GetHeroesAndBanditsSettings().Mode != 1 && m_HeroesAndBandits_AffinityIndex != -1){
			tempAffinity = GetHeroesAndBanditsLevels().Affinities.Get(m_HeroesAndBandits_AffinityIndex);
		} else if (GetHeroesAndBanditsSettings().Mode == 1){ //This is a bit more performance heavy so only useing if need to
			if (GetHaBPlayer().checkItem(cargo.GetType(), "inventory")){
				return super.CanReceiveItemIntoCargo(cargo);
			} else {
				habPrint("CanReceiveItemIntoCargo Item Blocked - "  + cargo.GetType(), "Debug");
				return false;
			}
		}
		if (tempAffinity.checkItem(m_HeroesAndBandits_AffinityPoints, cargo.GetType(), "inventory"))
		{
			return super.CanReceiveItemIntoCargo(cargo);
		}
		return false;
	}
	
	override bool CanSwapItemInCargo(EntityAI child_entity, EntityAI new_entity)
	{
		if (!GetIdentity() || !GetHeroesAndBanditsLevels() || !GetHeroesAndBanditsSettings() || !m_HeroesAndBandits_DataLoaded){
			habPrint("CanSwapItemInCargo Settings Not Definied - "  + new_entity.GetType(), "Debug");
			return super.CanSwapItemInCargo(child_entity, new_entity);
		}
		habAffinity tempAffinity = GetHeroesAndBanditsLevels().DefaultAffinity;
		if (GetHeroesAndBanditsSettings().Mode != 1 && m_HeroesAndBandits_AffinityIndex != -1){
			tempAffinity = GetHeroesAndBanditsLevels().Affinities.Get(m_HeroesAndBandits_AffinityIndex);
		} else if (GetHeroesAndBanditsSettings().Mode == 1){ //This is a bit more performance heavy so only useing if need to
			if (GetHaBPlayer().checkItem(new_entity.GetType(), "inventory")){
				return super.CanSwapItemInCargo(child_entity, new_entity);
			} else {
				habPrint("CanSwapItemInCargo Item Blocked - "  + new_entity.GetType(), "Debug");
				return false;
			}
		}
		if (tempAffinity.checkItem(m_HeroesAndBandits_AffinityPoints, new_entity.GetType(), "inventory"))
		{
			return super.CanSwapItemInCargo(child_entity, new_entity);
		}
		return false;
	}
	*/
	
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
		if (!GetIdentity() || !GetHeroesAndBanditsLevels() || !GetHeroesAndBanditsSettings() || !m_HeroesAndBandits_DataLoaded){
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
		if (mask){
			if (attachment == mask){
				if (!GetHeroesAndBanditsSettings().BanditCanRemoveMask && tempAffinity.Name == "bandit"){
					habPrint("CanReleaseAttachment Item Blocked - "  + attachment.GetType(), "Debug");
					return false;
				}
				if (!GetHeroesAndBanditsSettings().HeroCanRemoveMask && tempAffinity.Name == "hero"){
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
					return false;
				}
				if (!GetHeroesAndBanditsSettings().HeroCanRemoveArmBand && tempAffinity.Name == "hero"){
					habPrint("CanReleaseAttachment Item Blocked - "  + attachment.GetType(), "Debug");
					return false;
				}
			}
		}

		return super.CanReleaseAttachment(attachment);
	}
	
	
	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		if (!GetIdentity() || !GetHeroesAndBanditsLevels() || !GetHeroesAndBanditsSettings() || !m_HeroesAndBandits_DataLoaded){
			habPrint("CanReceiveAttachment Settings Not Definied"  + attachment.GetType(), "Debug" );
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
			if (slotId == InventorySlots.MASK){
				int index = -1;
				string itemType = attachment.GetType();
				if (!GetHeroesAndBanditsSettings().BanditCanRemoveMask && tempAffinity.Name == "bandit"){
					index = GetHeroesAndBanditsSettings().BanditMasks.Find(itemType);
					habPrint("CanReceiveAttachment Item Mask Check - index: " + index + " Attachment "  + attachment.GetType(), "Debug");
				}
				if (!GetHeroesAndBanditsSettings().HeroCanRemoveMask && tempAffinity.Name == "hero"){
					index = GetHeroesAndBanditsSettings().HeroMasks.Find(itemType);
					habPrint("CanReceiveAttachment Item Mask Check - index: " + index + " Attachment "  + attachment.GetType(), "Debug");
				}
				if (index != -1){
					return true;
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
		GetInputController().OverrideRaise( true, false );
		SetSynchDirty();
	}
	
	void habAILowerWeapon(){
		habPrint("habAILowerWeapon called", "Debug");
		m_HeroesAndBandits_AIRaiseWeapon = false;
		GetInputController().OverrideRaise( true, false );
	}
	
	void habAIRaiseWeaponServer(){
		habPrint("habAIRaiseWeaponServer called", "Debug");
		m_HeroesAndBandits_AIRaiseWeaponSync = true;
		m_HeroesAndBandits_AIRaiseWeapon = true;
		GetInputController().OverrideRaise( true, true );
		SetSynchDirty();
	}
	
	void habAIRaiseWeapon(){
		habPrint("habAIRaiseWeapon called", "Debug");
		m_HeroesAndBandits_AIRaiseWeapon = true;
		GetInputController().OverrideRaise( true, true );
	}
	
	void habAIAimWeapon(float x, float y){
		GetInputController().OverrideAimChangeX(true, x);
		GetInputController().OverrideAimChangeY(true, y);
	}
}
