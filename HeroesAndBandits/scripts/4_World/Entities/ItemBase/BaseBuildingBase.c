modded class BaseBuildingBase
{
	
	string habLastHitBy;
	
	void habSetLastHitByObj(Object hitBy){
		if ( hitBy ){
			if (hitBy.IsInherited(TrapBase)){
				TrapBase trap = TrapBase.Cast(hitBy);
				if (trap && trap.habGetActivatedBy() != "null"){
					habLastHitBy = trap.habGetActivatedBy();
				}
			} else if ( hitBy.IsInherited(Grenade_Base) ){
				Grenade_Base grenade = Grenade_Base.Cast(hitBy);
				if (grenade && grenade.habGetActivatedBy() != "null"){
					habLastHitBy = grenade.habGetActivatedBy();
				}
			}
			#ifdef EXPANSIONMOD
			 else if ( hitBy.IsInherited(Expansion_C4_Explosion) ){
				Expansion_C4_Explosion expansionExplosive = Expansion_C4_Explosion.Cast(hitBy);
				if (expansionExplosive && expansionExplosive.habGetActivatedBy() != "null"){
					habLastHitBy = expansionExplosive.habGetActivatedBy();
				}
			}
			#endif
		}
	}
	
	override void OnPartDestroyedServer( Man player, string part_name, int action_id, bool destroyed_by_connected_part = false )
	{
		if ( GetGame().IsServer() ){
			PlayerBase sourcePlayer = PlayerBase.Cast(player);
			if ( sourcePlayer ){
				habPrint("Player with ID" + sourcePlayer.GetIdentity().GetPlainId() + " Destroyed Item " + GetType() + " part " + part_name, "Debug");
				GetHeroesAndBandits().NewPlayerAction(sourcePlayer.GetIdentity().GetPlainId(), GetType()+"PartRaid");
			} else if ( habLastHitBy ){
				if ( habLastHitBy != "null")
				{
					habPrint("Player with ID " + habLastHitBy + " Destoryed " + GetType() + " part " + part_name, "Debug");
					GetHeroesAndBandits().NewPlayerAction(habLastHitBy, GetType()+"PartRaid");
				}
			}else {
				habPrint("Destroyed Item " + GetType() + " part " + part_name, "Debug");
			}
		}
		super.OnPartDestroyedServer( player, part_name, action_id );
	}
	
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
				//Doesn't work currently had to add to individual classes
				
				Grenade_Base grenade = Grenade_Base.Cast(source);
				habPrint( GetType() + " hit by " + grenade.GetType() + " set by " + grenade.habGetActivatedBy(), "Debug");
			} 
			#ifdef EXPANSIONMOD
			else if ( source.IsInherited(Expansion_C4_Explosion) ){
				Expansion_C4_Explosion expansionExplosive = Expansion_C4_Explosion.Cast(source);
				if ( expansionExplosive ){
					habPrint( GetType() + " hit by " + expansionExplosive.GetType() + " set by " + expansionExplosive.habGetActivatedBy() +  " in BaseBuildingBase Class", "Debug");
					habLastHitBy = expansionExplosive.habGetActivatedBy();
				}
			}
			#endif
			else {
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
	

};
