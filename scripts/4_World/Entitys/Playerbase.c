modded class PlayerBase
{
	float m_HeroesAndBandits_WarningSent = -1; //For Zones
	ref array< int > m_HeroesAndBandits_InZones = new ref array< int >; //For Zones
	bool  m_HeroesAndBandits_Killed = false;
	
	override void Init()
	{
		super.Init();
		
		RegisterNetSyncVariableFloat("m_HeroesAndBandits_WarningSent");
		RegisterNetSyncVariableBool("m_HeroesAndBandits_Killed");
		
	}
	
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
		m_HeroesAndBandits_Killed = true; //Pervent kills gettting counted twice with Explosions
		if (GetGame().IsServer()){
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
				if (sourcePlayerID == targetPlayerID){ //Sucide
					if ( killedByObject ){
						GetHeroesAndBandits().NewPlayerAction(sourcePlayerID, GetHeroesAndBandits().GetPlayerHeroOrBandit(sourcePlayerID)+"Sucide");
						GetHeroesAndBandits().TriggerSucideFeed(sourcePlayerID);
					} else if ( sourcePlayer && !sourcePlayer.IsInVehicle() ){//If not in Vehicle Crash
						GetHeroesAndBandits().NewPlayerAction(sourcePlayerID, GetHeroesAndBandits().GetPlayerHeroOrBandit(sourcePlayerID)+"Sucide");
						GetHeroesAndBandits().TriggerSucideFeed(sourcePlayerID);
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

		if ( damageType == DT_EXPLOSION && source && !this.IsAlive() && !m_HeroesAndBandits_Killed) {
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
		} else  if ( damageType == DT_EXPLOSION && !source && !this.IsAlive() ) {
			habPrint( "" + GetIdentity().GetPlainId() + " killed by Explosion with no source", "Debug");
		}
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}
}