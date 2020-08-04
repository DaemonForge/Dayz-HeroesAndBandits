#ifdef EXPANSIONMOD
	modded class ExpansionSafeBase extends Container_Base
	{
		string habLastHitBy;
		bool habKilled = false;

		override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
		{
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
					habPrint( GetType() + " hit by " + trap.GetType() + " set by " + trap.habGetActivatedBy(), "Debug");
					habLastHitBy = trap.habGetActivatedBy();
				} else if (source.IsInherited(Grenade_Base)){
					Grenade_Base grenade = Grenade_Base.Cast(source);
					habPrint( GetType() + " hit by " + grenade.GetType() + " set by " + grenade.habGetActivatedBy(), "Debug");
					habLastHitBy = grenade.habGetActivatedBy();
				} else if ( source.IsInherited(Expansion_C4_Explosion) ){
					Expansion_C4_Explosion expansionExplosive = Expansion_C4_Explosion.Cast(source);
					if ( expansionExplosive ){
						habPrint( GetType() + " hit by " + expansionExplosive.GetType() + " set by " + expansionExplosive.habGetActivatedBy() +  " in BaseBuildingBase Class", "Debug");
						habLastHitBy = expansionExplosive.habGetActivatedBy();
					}
				} else {
					habPrint( GetType() + " hit by " + source.GetType(), "Debug");
				}
				if ( sourcePlayer )
				{
					habPrint( GetType() + " hit by " + sourcePlayer.GetIdentity().GetPlainId() + " with " + source.GetType(), "Debug");
					habLastHitBy = sourcePlayer.GetIdentity().GetPlainId();
				}
			} else if ( damageType == DT_EXPLOSION ) {
				habPrint( GetType() + " hit by Explosion with unknown source in BaseBuildingBase class" , "Debug");
			}
			super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);	
		}	
	

		override void EEKilled( Object killer )
		{
			if ( killer ){
				habPrint(GetType() + " Destroyed by " + killer.GetType() + " in ExpansionSafeBase class" , "Debug");
				if (killer.IsInherited(Grenade_Base)){
					Grenade_Base grenade = Grenade_Base.Cast(killer);
					habPrint( GetType() + " Destroyed by " + grenade.GetType() + " set by " + grenade.habGetActivatedBy(), "Debug");
					habLastHitBy = grenade.habGetActivatedBy();
				}
			}
			if ( GetGame().IsServer() ){
				if ( habLastHitBy && !habKilled){
					if ( habLastHitBy != "null" )
					{
						habPrint("Player with ID " + habLastHitBy + " Destoryed " + GetType(), "Debug");
						GetHeroesAndBandits().NewPlayerAction(habLastHitBy, GetType()+"Raid");
						habKilled = true; //pervent counting double
					}
				} else {
					habPrint("Destroyed Item " + GetType() , "Debug");
				}
			}
			super.EEKilled( killer );
		}
	}
#endif