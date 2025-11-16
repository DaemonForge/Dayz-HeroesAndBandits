modded class AnimalBase
{
	override void EEKilled(Object killer)
    {

        super.EEKilled(killer);
		if (g_Game.IsServer()){
			PlayerBase sourcePlayer;
			if (killer.IsMan()) {
				if (killer.IsInherited(SurvivorBase)) {
					sourcePlayer = PlayerBase.Cast(killer);
				} 
			} else if (killer.IsWeapon()) {
				sourcePlayer = PlayerBase.Cast(EntityAI.Cast(killer).GetHierarchyParent());
			} else if (killer.IsMeleeWeapon()) {
				sourcePlayer = PlayerBase.Cast(EntityAI.Cast(killer).GetHierarchyParent());
			} else if (killer.IsTransport()){
				CarScript vehicle;
				if (Class.CastTo(vehicle, killer)) {
					if ( vehicle.CrewSize() > 0 ) {
						sourcePlayer = PlayerBase.Cast(vehicle.CrewMember( 0 ));
					}
				}
			} else {
				return;
			}
			
			if (sourcePlayer && sourcePlayer.GetIdentity()){
				if (IsInherited(Animal_UrsusArctos)){
					sourcePlayer.NewHABAction("huntbear",this);
					return;
				}
				if (IsInherited(Animal_CanisLupus)){
					sourcePlayer.NewHABAction("huntwolf",this);
					return;
				}
				if (IsInherited(Animal_GallusGallusDomesticus)){
					sourcePlayer.NewHABAction("huntchicken",this);
					return;
				}
				sourcePlayer.NewHABAction("huntanimal",this);
			}
		}
    }
	{
		PlayerBase sourcePlayer;
		string sourcePlayerID;
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		if (g_Game.IsServer()){
			if (source.IsMan())	{
				sourcePlayer = PlayerBase.Cast(source);
			} else if (source.IsWeapon()) {
				sourcePlayer = PlayerBase.Cast(EntityAI.Cast(source).GetHierarchyParent());
			} else if (source.IsMeleeWeapon()) {
				sourcePlayer = PlayerBase.Cast(EntityAI.Cast(source).GetHierarchyParent());
			} 						
			if (sourcePlayer){
				if (sourcePlayer.GetIdentity()){
					sourcePlayerID = sourcePlayer.GetIdentity().GetId();
					m_HeroesAndBandits_LastBleedingSourceType = habDeathType.Bambi; //Just using bambi
					m_HeroesAndBandits_LastBleedingSourceID = sourcePlayerID;
				}
			}
		}
	}

};