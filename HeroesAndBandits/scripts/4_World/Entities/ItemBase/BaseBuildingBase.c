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
			#ifdef BASICBOMBS
			/*else if (hitBy.IsInherited(BB_PipeBomb) ){
				BB_PipeBomb bomb = BB_PipeBomb.Cast(hitBy);
				if (bomb && bomb.GetIgnitedBySteamId() != "null"){
					habLastHitBy = bomb.GetIgnitedBySteamId();
					return;
				}
			}*/
			#endif
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
				sourcePlayer.NewHABAction("PartDestroyed",this);
			} else if ( habLastHitBy ){
				if ( habLastHitBy != "null")
				{
					//TODO
				}
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
				habLastHitBy = trap.habGetActivatedBy();
			} else if (source.IsInherited(Grenade_Base)){
				//Doesn't work currently had to add to individual classes
				
				Grenade_Base grenade = Grenade_Base.Cast(source);
			} 
			#ifdef EXPANSIONMOD
			else if ( source.IsInherited(Expansion_C4_Explosion) ){
				Expansion_C4_Explosion expansionExplosive = Expansion_C4_Explosion.Cast(source);
				if ( expansionExplosive ){
					habLastHitBy = expansionExplosive.habGetActivatedBy();
				}
			}
			#endif
			if ( sourcePlayer )
			{
				habLastHitBy = sourcePlayer.GetIdentity().GetId();
			}
		}
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}
	

};
