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
	private float m_HeroesAndBandits_LatestZoneDmg = 0;
	

	
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
		RegisterNetSyncVariableFloat("m_HeroesAndBandits_LatestZoneDmg");
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
		int oldAffinity = m_HeroesAndBandits_AffinityIndex;
		m_HeroesAndBandits_AffinityIndex = affinityIndex;
		m_HeroesAndBandits_AffinityPoints = affinityPoints;
	
		m_HeroesAndBandits_LevelIndex = levelIndex;
		if (oldAffinity != m_HeroesAndBandits_AffinityIndex){
			habAffinityChange( oldAffinity );
		}
		SetSynchDirty();
	}
	
	void habAffinityChange( int oldAffinity ){
		bool createItem = false;
		habAffinity tempAffinity = GetHeroesAndBanditsLevels().DefaultAffinity;
		if (GetHeroesAndBanditsSettings().Mode != 1 && m_HeroesAndBandits_AffinityIndex != -1){
			tempAffinity = GetHeroesAndBanditsLevels().Affinities.Get(m_HeroesAndBandits_AffinityIndex);
			createItem = true;
		}
		bool CheckMaskBandit = (!GetHeroesAndBanditsSettings().BanditCanRemoveMask && tempAffinity.Name == "bandit" );
		bool CheckMaskHero = (!GetHeroesAndBanditsSettings().HeroCanRemoveMask && tempAffinity.Name == "hero" );
		bool CheckArmbandBandit = (!GetHeroesAndBanditsSettings().BanditCanRemoveArmBand && tempAffinity.Name == "bandit" );
		bool CheckArmbandHero = (!GetHeroesAndBanditsSettings().HeroCanRemoveArmBand && tempAffinity.Name == "hero" );
		int index = -1;
		if ( createItem ){
			EntityAI mask = EntityAI.Cast(GetInventory().FindAttachment(InventorySlots.MASK));
			EntityAI armband = EntityAI.Cast(GetInventory().FindAttachment(InventorySlots.ARMBAND));
			if (CheckMaskBandit && GetHeroesAndBanditsSettings().BanditMasks.Count() > 0){ 
				if ( mask ){ 
					
					
				} else {
					
				}
			}
			if (CheckMaskHero && GetHeroesAndBanditsSettings().HeroMasks.Count() > 0){
				if ( mask ){
					
					
				} else {
					
				}
			}
			if (CheckArmbandBandit && GetHeroesAndBanditsSettings().BanditArmBads.Count() > 0){
				if ( armband ){
					
				} else {
					
				}
			}
			if (CheckArmbandHero && GetHeroesAndBanditsSettings().HeroArmBands.Count() > 0){
				if ( armband ){
					
				} else {
					
				}
			}
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
		int animType;
		float animHitDir;
		bool animHitFullbody;
		EvaluateDamageHitAnimation(damageResult, damageType, source, dmgZone, ammo, modelPos, animType, animHitDir, animHitFullbody);
		habPrint("DamageHit Animation Info animType: " + animType + " animHitDir: " + animHitDir + " animHitFullbody: " + animHitFullbody, "Debug");
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
			TStringArray bleedingSources = {"LeftForeArmRoll","RightArm","LeftArm","RightLeg","LeftLeg","RightForeArmRoll","Torso","Neck","Head"};
			GetBleedingManagerServer().AttemptAddBleedingSourceBySelection(bleedingSources.GetRandomElement());
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
		m_HeroesAndBandits_KilledByZoneName = zoneName;
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
						index = GetHeroesAndBanditsSettings().BanditArmBads.Find(itemInHands.GetType());
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
				GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater(this.habRestoreClothing, 50, false, item, slot_name); //Call later so the item is actually removed
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
					index = GetHeroesAndBanditsSettings().BanditArmBads.Find(armband.GetType());
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
					index = GetHeroesAndBanditsSettings().BanditArmBads.Find(itemType);
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
		//GetInputController().OverrideRaise( true, false );
		SetSynchDirty();
	}
	
	void habAILowerWeapon(){
		habPrint("habAILowerWeapon called", "Debug");
		m_HeroesAndBandits_AIRaiseWeapon = false;
		//GetInputController().OverrideRaise( true, false );
	}
	
	void habAIRaiseWeaponServer(){
		habPrint("habAIRaiseWeaponServer called", "Debug");
		m_HeroesAndBandits_AIRaiseWeaponSync = true;
		m_HeroesAndBandits_AIRaiseWeapon = true;
		//GetInputController().OverrideRaise( true, true );
		//SendCompleteWeaponRaiseJuncture();
		SetSynchDirty();
	}
	
	void habAIRaiseWeapon(){
		habPrint("habAIRaiseWeapon called", "Debug");
		m_HeroesAndBandits_AIRaiseWeapon = true;
		//RunADSTimer();
		//GetInputController().OverrideRaise( true, true );
	}
	
	void habAIAimWeapon(float x, float y){
		GetInputController().OverrideAimChangeX(true, x);
		GetInputController().OverrideAimChangeY(true, y);
	}
}
