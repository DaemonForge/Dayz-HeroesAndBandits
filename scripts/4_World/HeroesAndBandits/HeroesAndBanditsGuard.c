class HeroesAndBanditsGuard
{
	float X;
	float Y;
	float Z;
	float Orientation;
	private PlayerBase Guard;
	string Skin;
	string WeaponInHands;
	string WeaponInHandsMag;
	ref TStringArray WeaponInHandsAttachments;
	ref TStringArray GuardGear;

    void HeroesAndBanditsGuard(float x, float y, float z, float orientation, string skin, string weaponInHands, string weaponInHandsMag, TStringArray weaponInHandsAttachments, TStringArray guardGear) 
	{
		X = x;
		Y = y;
		Z = z;
		Orientation = orientation;
		Skin = skin;
		WeaponInHands = weaponInHands;
		WeaponInHandsMag = weaponInHandsMag;
		WeaponInHandsAttachments =  weaponInHandsAttachments;
		GuardGear = guardGear;
		
    }
	
	void Spawn()
	{
			habPrint("Spawning Guard: " + Skin + " at " + " X:" + X + " Y:" + Y +" Z:" + Z, "Verbose");	
			Object obj = GetGame().CreateObject( Skin , getVector(), false, false, true );
			obj.SetPosition(getVector());
			EntityAI weaponInHands;
			EntityAI weaponInHandsMag;
			if (Class.CastTo(Guard, obj))
			{	
				Guard.SetAllowDamage(false);
				for ( int i =0; i < GuardGear.Count(); i++ )
				{
					Guard.GetInventory().CreateAttachment(GuardGear.Get(i));
				}
				if (Guard.GetHumanInventory().GetEntityInHands()){
					habPrint("Item spawned in Guard: " + Skin + " at " + " X:" + X + " Y:" + Y +" Z:" + Z + " Item: " + Guard.GetHumanInventory().GetEntityInHands().GetDisplayName() +  " Removing it", "Exception");	
					Guard.GetHumanInventory().GetEntityInHands().Delete(); //Remove any Items in Hand
				}
				weaponInHands = Guard.GetHumanInventory().CreateInHands(WeaponInHands);
				for ( int j =0; j < WeaponInHandsAttachments.Count(); j++ )
				{
					weaponInHands.GetInventory().CreateAttachment(WeaponInHandsAttachments.Get(j));
				}
				
			}
			vector guardOrientation = vector.Zero;
			guardOrientation[0] = Orientation;
			guardOrientation[1] = 0;
			guardOrientation[2] = 0;
			
			obj.SetOrientation(guardOrientation);
	}
	
	void ReloadWeapon()
	{
		habPrint("Reloading Gun Guard: " + Skin + " at " + " X:" + X + " Y:" + Y +" Z:" + Z, "Verbose");	
		Weapon_Base weaponInHands = Weapon_Base.Cast(Guard.GetHumanInventory().GetEntityInHands());
		if (weaponInHands)
		{
			EntityAI mag = EntityAI.Cast(weaponInHands.GetInventory().CreateAttachment(WeaponInHandsMag));
			habPrint("AttachMag", "Debug"); 
			weaponInHands.AttachMagazine(Weapon_Base.GetCurrentMuzzle(), Magazine.Cast(mag));
			habPrint("pushToChamberFromAttachedMagazine", "Debug");
			pushToChamberFromAttachedMagazine( weaponInHands, weaponInHands.GetCurrentMuzzle() );

		}
	}
	
	void ReloadWeaponTest()
	{
		habPrint("Reloading Test Gun Guard: " + Skin + " at " + " X:" + X + " Y:" + Y +" Z:" + Z, "Verbose");	
		Weapon_Base weaponInHands = Weapon_Base.Cast(Guard.GetHumanInventory().GetEntityInHands() );
		if (weaponInHands.IsWeapon())
		{
			EntityAI mag = EntityAI.Cast(Guard.GetInventory().CreateAttachment(WeaponInHandsMag) );
			habPrint("AttachMag", "Debug");		
			WeaponManager(PlayerBase.Cast(Guard)).AttachMagazine(Magazine.Cast(mag));
			habPrint("pushToChamberFromAttachedMagazine", "Debug");
			pushToChamberFromAttachedMagazine( weaponInHands, weaponInHands.GetCurrentMuzzle() );
		}
	}
	
	void FireWeapon()
	{
		habPrint("Firing Gun Guard: " + Skin + " at " + " X:" + X + " Y:" + Y +" Z:" + Z, "Verbose");	
		EntityAI weaponInHands = EntityAI.Cast(Guard.GetHumanInventory().GetEntityInHands());
		if (weaponInHands.IsWeapon())
		{
			WeaponManager(PlayerBase.Cast(Guard)).Fire(Weapon_Base.Cast(weaponInHands));
		}
	}

	vector getVector(){
		return Vector( X, Y, Z );
	}
}

