modded class Fence 
{
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if ( source ) {
			if (source.IsInherited(Grenade_Base)){
				// If source is Grenade 
				
				Grenade_Base grenade = Grenade_Base.Cast(source);
				this.habLastHitBy = grenade.habGetActivatedBy();
			}
		}
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}
};