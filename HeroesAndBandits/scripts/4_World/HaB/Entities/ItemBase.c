#ifdef EXPANSIONMOD
modded class ItemBase extends InventoryItem
{
	override void Explode(int damageType, string ammoType = "")
	{
		
		float blastRange = 5;
		
		array<Object> nearest_objects = new array<Object>;
		array<CargoBase> proxy_cargos = new array<CargoBase>;
		GetGame().GetObjectsAtPosition3D( this.GetPosition(), blastRange, nearest_objects, proxy_cargos );
		for ( int i = 0; i < nearest_objects.Count(); i++ )
		{
			Object nearest_object = nearest_objects.Get(i);

			if ( nearest_object.IsInherited( ExpansionBaseBuilding ) )
			{
				BaseBuildingBase baseBuildingObject = BaseBuildingBase.Cast(nearest_object);
				baseBuildingObject.habSetLastHitByObj(this);
			}
		}
		
		super.Explode(damageType, ammoType);
	}
};
#endif