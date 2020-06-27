modded class BaseBuildingBase
{
	
	string habLastHitBy;
	
	override void OnPartDestroyedServer( Man player, string part_name, int action_id, bool destroyed_by_connected_part = false )
	{
		if (GetGame().IsServer()){
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
				habPrint( GetType() + " hit by " + source.GetType() + " set by " + source.habGetActivatedBy(), "Debug");
				habLastHitBy = source.habGetActivatedBy();
			} else {
				habPrint( GetType() + " hit by " + source.GetType(), "Debug");
			}
			if ( sourcePlayer )
			{
				habPrint( GetType() + " hit by " + sourcePlayer.GetIdentity().GetPlainId(), "Debug");
				habLastHitBy = sourcePlayer.GetIdentity().GetPlainId();
			}
		} else if ( damageType == DT_EXPLOSION ) {
			habPrint( GetType() + " Hit by Explosion with no source", "Debug");
		} else {
			habPrint( GetType() + " Hit by Type " + damageType + " with no source", "Debug");
		}
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}
	

}

