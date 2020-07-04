
class HeroesAndBanditsZone
{
	int Index; //0 top level zones, 1 sub zone, 2 sub zone of a sub zone 
	int ZoneID;
    string Name;
	float X;
	float Z;
	int WarningRadius;
	bool ShowWarningMsg;
	string WarningMessage;
	bool ShowWelcomeMsg;
	string WelcomeMessage;
	string WelcomeIcon;
	int WelcomeMessageColor;
	int KillRadius;
    float MinHumanity;
    float MaxHumanity;
	bool OverrideSafeZone;
	bool GodModPlayers;
	ref array< ref HeroesAndBanditsGuard > Guards = new ref array< ref HeroesAndBanditsGuard >;
	ref array< ref HeroesAndBanditsZone > SubZones = new ref array< ref HeroesAndBanditsZone >;

	void Init(habZone zoneToLoad, int zoneID, int index = 0){
		Index = index;
		ZoneID = zoneID;
		Name = zoneToLoad.Name;
		X = zoneToLoad.X;
		Z = zoneToLoad.Z;
		MinHumanity = zoneToLoad.MinHumanity;
		MaxHumanity = zoneToLoad.MaxHumanity;
		WarningRadius = zoneToLoad.WarningRadius;
		KillRadius = zoneToLoad.KillRadius;
		WelcomeMessageColor = zoneToLoad.getWelcomeMessageColor();
		ShowWelcomeMsg = zoneToLoad.ShowWelcomeMsg;
		WelcomeMessage = zoneToLoad.WelcomeMessage;
		WelcomeIcon = zoneToLoad.WelcomeIcon;
		ShowWarningMsg = zoneToLoad.ShowWarningMsg;
		WarningMessage = zoneToLoad.WarningMessage;
		OverrideSafeZone = zoneToLoad.OverrideSafeZone;
		GodModPlayers = zoneToLoad.GodModPlayers;
		if (zoneToLoad.Guards){
			for ( int j = 0; j < zoneToLoad.Guards.Count(); j++ )
			{	
				float guardX = zoneToLoad.Guards.Get(j).X;
				float guardY = zoneToLoad.Guards.Get(j).Y; 
				float guardZ = zoneToLoad.Guards.Get(j).Z; 
				float orientation = zoneToLoad.Guards.Get(j).Orientation; 
				string skin = zoneToLoad.Guards.Get(j).Skin; 
				string weaponInHands = zoneToLoad.Guards.Get(j).WeaponInHands; 
				string weaponInHandsMag = zoneToLoad.Guards.Get(j).WeaponInHandsMag;
				TStringArray weaponInHandsAttachments = zoneToLoad.Guards.Get(j).WeaponInHandsAttachments; 
				TStringArray guardGear = zoneToLoad.Guards.Get(j).GuardGear;
				Guards.Insert(new ref HeroesAndBanditsGuard(guardX, guardY, guardZ, orientation, skin, weaponInHands, weaponInHandsMag, weaponInHandsAttachments, guardGear));
				Guards.Get(j).Spawn();
				//GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLaterByName(Guards.Get(j), "ReloadWeapon", 160000, false); //Reload gun 3 minutes after server start
				//GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLaterByName(Guards.Get(j), "ReloadWeaponTest", 220000, false); //Reload gun 3 minutes after server start	
			}
		}
		if (zoneToLoad.SubZones) //Check if any Sub Zones are defined before loading
		{
			for ( int i = 0; i < zoneToLoad.SubZones.Count(); i++ )
			{
		    	habPrint("Loading Sub Zone " + zoneToLoad.SubZones.Get(i).Name , "Verbose");
				string name = zoneToLoad.SubZones.Get(i).Name;
				int x = zoneToLoad.SubZones.Get(i).X;
				int z = zoneToLoad.SubZones.Get(i).Z;
				SubZones.Insert(new ref HeroesAndBanditsZone(name, x, z));
				SubZones.Get(i).Init(zoneToLoad.SubZones.Get(i), i, Index + 1);
			}
		}
		
	}
		
	void CheckPlayer(PlayerBase player){
		float playerHumanity = GetPlayerHumanity(player.GetIdentity().GetPlainId());
		if ( !player )
		{
			return;
		}
			
		if (player.IsAlive())				
		{//habPrint("Checking if Players is in Zone " + Name, "Debug");	
			if (validHumanity(playerHumanity) && vector.Distance(player.GetPosition(), getVector()) <= WarningRadius && player.m_HeroesAndBandits_WarningSent != k ){
				habPrint("Player: " + player.GetIdentity().GetName() + " ("+player.GetIdentity().GetPlainId()+") Entered: " + Name, "Verbose");
				player.enteredZone(ZoneID, Index);
				if ( ShowWelcomeMsg )
				{
					WelcomePlayer(Name, WelcomeMessage, WelcomeIcon, player.GetIdentity().GetPlainId(), WelcomeMessageColor);
				}
				if ( GodModPlayers && validHumanity(playerHumanity))
				{
					player.SetAllowDamage(false);
				}
			}
			else if (!validHumanity(playerHumanity) && isInZone(ZoneID, Index) && vector.Distance(player.GetPosition(), getVector()) <= KillRadius)
			{
				//Player Entered Kill Zone Kill Player if warning has been given
				player.leftZone(Index);
				habPrint("Killed Player: " + player.GetIdentity().GetName() + " ("+player.GetIdentity().GetPlainId()+") for Entering Zone: " + Name, "Always");
				if ( OverrideSafeZone )
				{
					player.SetAllowDamage(true);
				}
				FireWeaponClosestGuard(player.GetPosition());
				player.SetHealth(0.0);
			}
			else if (!validHumanity(playerHumanity) && player.m_HeroesAndBandits_WarningSent != k && vector.Distance(player.GetPosition(), getVector()) <= WarningRadius)
			{
				//Player Entered Warning Zone Issue Warning
				player.enteredZone(ZoneID, Index);
				habPrint("Player: " + player.GetIdentity().GetName() + " ("+player.GetIdentity().GetPlainId()+") Entered: " + Name, "Verbose");
				if ( ShowWarningMsg )
				{
					GetHeroesAndBandits().WarnPlayer(Name, WarningMessage, player.GetIdentity().GetPlainId());
				}		
			}
			else if (vector.Distance(player.GetPosition(), getVector()) > WarningRadius && player.m_HeroesAndBandits_WarningSent == k)
			{
				if ( GodModPlayers )
				{
					player.SetAllowDamage(true);
				}
				//Player Left Warning Radius
				habPrint("Player: " + player.GetIdentity().GetName() + " ("+player.GetIdentity().GetPlainId()+") Left: " + Name, "Verbose");
				player.leftZone(Index);			
			}
			if ( SubZones && player.isInZone(ZoneID, Index) )
			{ 
				for (int subI = 0; subI < SubZones.Count(); subI++)
				{
					SubZones.Get(subI).CheckPlayer(player);
				}
			}
		}
		
	}
	
    void HeroesAndBanditsZone(string name, float x, float z) 
	{

    }
	
	vector getVector(){
		return Vector( X, GetGame().SurfaceY(X, Z), Z );
	}
	
	bool validHumanity(float humanity){
			if ( MinHumanity != -1 && MaxHumanity != -1 && humanity >= MinHumanity && humanity <= MaxHumanity){
				return true;
			}else if (MinHumanity == -1 && MaxHumanity == -1){
				return true;
			}else if (MinHumanity == -1 && MaxHumanity != -1 && humanity <= MaxHumanity){
				return true;
			}else if (MinHumanity != -1 && MaxHumanity == -1 && humanity >= MinHumanity){
				return true;
			}
		return false;
	}
	
	void FireWeaponClosestGuard(vector playerPostion)
	{
		if (!Guards)//If no guards defined exit
		{
			return;
		}
		int closestGuardIndex = -1;
		float closestGuardDistance = 600;
		for ( int i = 0; i < Guards.Count(); i++ )
		{	
			float currentGuardDistance = vector.Distance( Guards.Get(i).getVector(), playerPostion);
			if ( currentGuardDistance < closestGuardDistance)
			{
				closestGuardIndex = i;
				closestGuardDistance = currentGuardDistance;
			}
		}
		if ( closestGuardIndex == -1 ){
			return;
		} else {
			Guards.Get(closestGuardIndex).FireWeapon();
		} 
	}
	
}

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
		EntityAI weaponInHands = Weapon_Base.Cast(Guard.GetHumanInventory().GetEntityInHands());
		EntityAI weaponInHandsMag = Guard.GetInventory().CreateAttachment(WeaponInHandsMag);
		if (weaponInHands.IsWeapon())
		{
			Guard.GetWeaponManager().AttachMagazine(Magazine.Cast(weaponInHandsMag));
		}
	}
	
	void ReloadWeaponTest()
	{
		habPrint("Reloading Gun Guard: " + Skin + " at " + " X:" + X + " Y:" + Y +" Z:" + Z, "Verbose");	
		EntityAI weaponInHands = Weapon_Base.Cast(Guard.GetHumanInventory().GetEntityInHands());
		EntityAI weaponInHandsMag = weaponInHands.GetInventory().CreateAttachment(WeaponInHandsMag);
		if (weaponInHands.IsWeapon())
		{
				habPrint("Unjam Gun: " + Skin + " at " + " X:" + X + " Y:" + Y +" Z:" + Z, "Verbose");	
				Guard.GetWeaponManager().Unjam();
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

