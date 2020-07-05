#ifdef EXPANSIONMOD
	modded class ExpansionBaseBuilding
	{
		override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
		{
			habPrint( GetType() + " EEHITBY in in ExpansionBaseBuilding class" , "Debug");
			if ( source ) {
				PlayerBase sourcePlayer;
				if (source.IsMan())	{
					if (source.IsInherited(SurvivorBase))
					{
						sourcePlayer = PlayerBase.Cast(source);
					}
				} else if (source.IsWeapon()) {
					sourcePlayer = PlayerBase.Cast(EntityAI.Cast(source).GetHierarchyParent());
				} else if (source.IsMeleeWeapon()) {
					sourcePlayer = PlayerBase.Cast(EntityAI.Cast(source).GetHierarchyParent());
				} else if (source.IsInherited(TrapBase)){
					TrapBase trap = TrapBase.Cast(source);
					habPrint( GetType() + " hit by " + trap.GetType() + " set by " + trap.habGetActivatedBy() + " in ExpansionBaseBuilding class", "Debug");
					habLastHitBy = trap.habGetActivatedBy();
				} else if (source.IsInherited(Grenade_Base)){
					Grenade_Base grenade = Grenade_Base.Cast(source);
					habPrint( GetType() + " hit by " + grenade.GetType() + " set by " + grenade.habGetActivatedBy() + " in ExpansionBaseBuilding class", "Debug");
				} 
				else if ( source.IsInherited(Expansion_C4_Explosion)){
					Expansion_C4_Explosion expansionExplosive = Expansion_C4_Explosion.Cast(source);
					if ( expansionExplosive ){
						habPrint( GetType() + " hit by " + expansionExplosive.GetType() + " set by " + expansionExplosive.habGetActivatedBy() + " in ExpansionBaseBuilding class", "Debug");
						habLastHitBy = expansionExplosive.habGetActivatedBy();
					}
				}
				else {
					habPrint( GetType() + " hit by " + source.GetType() + " in ExpansionBaseBuilding class", "Debug");
				}
				if ( sourcePlayer )
				{
					habPrint( GetType() + " hit by " + sourcePlayer.GetIdentity().GetPlainId() + " with " + source.GetType() + " in ExpansionBaseBuilding class", "Debug");
					habLastHitBy = sourcePlayer.GetIdentity().GetPlainId();
				}
			} else if (damageType == DT_EXPLOSION) {
				habPrint( GetType() + " hit by Explosion with unknown source in ExpansionBaseBuilding class" , "Debug");
			}
			super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		}
	
	
		override void EEKilled( Object killer )
		{
			PlayerBase sourcePlayer;
			if ( killer ){
				habPrint( GetType() + " hit by " + killer.GetType(), "Debug");
				if ( killer.IsInherited(Expansion_C4_Explosion)){
					Expansion_C4_Explosion expansionExplosive = Expansion_C4_Explosion.Cast(killer);
					if ( expansionExplosive ){
						habPrint( GetType() + " hit by " + expansionExplosive.GetType() + " set by " + expansionExplosive.habGetActivatedBy() +  " in ExpansionBaseBuilding", "Debug");
						habLastHitBy = expansionExplosive.habGetActivatedBy();
					}
				} else if (killer.IsInherited(SurvivorBase))	{
					sourcePlayer = PlayerBase.Cast(killer);
				} else if (killer.IsWeapon()) {
					sourcePlayer = PlayerBase.Cast(EntityAI.Cast(killer).GetHierarchyParent());
				} else if (killer.IsMeleeWeapon()) {
					sourcePlayer = PlayerBase.Cast(EntityAI.Cast(killer).GetHierarchyParent());
				} else if (killer.IsInherited(TrapBase)){
					TrapBase trap = TrapBase.Cast(killer);
					habPrint( GetType() + " hit by " + trap.GetType() + " set by " + trap.habGetActivatedBy() + " in ExpansionBaseBuilding", "Debug");
					habLastHitBy = trap.habGetActivatedBy();
				} else if (killer.IsInherited(Grenade_Base)){					
					Grenade_Base grenade = Grenade_Base.Cast(killer);
					habPrint( GetType() + " hit by " + grenade.GetType() + " set by " + grenade.habGetActivatedBy() + " in ExpansionBaseBuilding", "Debug");
					habLastHitBy = grenade.habGetActivatedBy();
				}
			}
			if ( sourcePlayer){
					habLastHitBy = sourcePlayer.GetIdentity().GetPlainId();
			}
			if ( GetGame().IsServer() ){
				if ( habLastHitBy ){
					if ( habLastHitBy != "null")
					{
						habPrint("Player with ID " + habLastHitBy + " Destoryed " + GetType(), "Debug");
						GetHeroesAndBandits().NewPlayerAction(habLastHitBy, GetType()+"Raid");
					}
				}else {
					habPrint("Destroyed Item " + GetType() , "Debug");
				}
			}
			super.EEKilled( killer );
		}
	}
#endif