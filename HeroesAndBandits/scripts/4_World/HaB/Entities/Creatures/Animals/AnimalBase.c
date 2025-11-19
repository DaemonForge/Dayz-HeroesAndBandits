modded class AnimalBase
{
	
	protected int m_HeroesAndBandits_LastBleedingSourceType = -1;
	protected string m_HeroesAndBandits_LastBleedingSourceID;
	
	override void EEKilled(Object killer)
    {

		Print(ClassName() + " killed by " + killer.ClassName());
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
			} else if (m_HeroesAndBandits_LastBleedingSourceType == habDeathType.Bambi && m_HeroesAndBandits_LastBleedingSourceID != ""){
				sourcePlayer = PlayerBase.Cast(UUtil.FindPlayer(m_HeroesAndBandits_LastBleedingSourceID));
			}
			
			if (sourcePlayer && sourcePlayer.GetIdentity()){
				if (IsInherited(Animal_UrsusArctos)){
					sourcePlayer.NewHABAction("huntbear",this);
				} else if (IsInherited(Animal_CanisLupus)){
					sourcePlayer.NewHABAction("huntwolf",this);
				} else if (IsInherited(Animal_GallusGallusDomesticus)){
					sourcePlayer.NewHABAction("huntchicken",this);
				} else {
					sourcePlayer.NewHABAction("huntanimal",this);
				}
			}
		}
        super.EEKilled(killer);
    }
	
	
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		Print(ClassName() + " hit by " + source.ClassName());
		PlayerBase sourcePlayer;
		string sourcePlayerID;
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
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}

};