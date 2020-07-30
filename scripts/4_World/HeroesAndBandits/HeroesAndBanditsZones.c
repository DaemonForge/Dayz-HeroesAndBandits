
class HeroesAndBanditsZone
{
	int Index; //0 top level zones, 1 sub zone, 2 sub zone of a sub zone etc.
	int ZoneID;
    string Name;
	float X;
	float Z;
	float Radius;
	bool ShowWarningMsg;
	string WarningMessage;
	bool ShowWelcomeMsg;
	string WelcomeMessage;
	string WelcomeIcon;
	int WelcomeMessageColor;
	float KillRadius;
    float MinHumanity;
    float MaxHumanity;
	bool OverrideSafeZone;
	bool GodModPlayers;
	bool PreventWeaponRaise;
	bool PerventActions;
	ref array< ref habZoneAffinity > Affinities = new ref array< ref habZoneAffinity >;
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
		Radius = zoneToLoad.WarningRadius;
		KillRadius = zoneToLoad.KillRadius;
		WelcomeMessageColor = zoneToLoad.getWelcomeMessageColor();
		ShowWelcomeMsg = zoneToLoad.ShowWelcomeMsg;
		WelcomeMessage = zoneToLoad.WelcomeMessage;
		WelcomeIcon = zoneToLoad.WelcomeIcon;
		ShowWarningMsg = zoneToLoad.ShowWarningMsg;
		WarningMessage = zoneToLoad.WarningMessage;
		OverrideSafeZone = zoneToLoad.OverrideSafeZone;
		GodModPlayers = zoneToLoad.GodModPlayers;
		PreventWeaponRaise = zoneToLoad.PreventWeaponRaise;
		PerventActions = zoneToLoad.PerventActions;
		Affinities = zoneToLoad.Affinities;
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
				Guards.Get(j).GunSound = zoneToLoad.Guards.Get(j).GunSound;
				Guards.Get(j).DamagePerTickMin = zoneToLoad.Guards.Get(j).DamagePerTickMin;
				Guards.Get(j).DamagePerTickRand = zoneToLoad.Guards.Get(j).DamagePerTickRand;
				Guards.Get(j).GunTickMulitplier  = zoneToLoad.Guards.Get(j).GunTickMulitplier;
				Guards.Get(j).HitChance  = zoneToLoad.Guards.Get(j).HitChance;

				Guards.Get(j).Spawn();
				//GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLaterByName(Guards.Get(j), "ReloadWeapon", 60000, false); //Reload gun 3 minutes after server start
				//GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLaterByName(Guards.Get(j), "ReloadWeaponTest", 440000, false); //Reload gun 3 minutes after server start	
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
				SubZones.Get(i).Init(zoneToLoad.SubZones.Get(i), i + 1, Index + 1);
			}
		}
		
	}
		
	void CheckPlayer(PlayerBase inPlayer, bool allowSpinOff = true){
		PlayerBase player = PlayerBase.Cast(inPlayer);
		HeroesAndBanditsPlayer pdata = GetHeroesAndBandits().GetPlayer(player.GetIdentity().GetPlainId());
		if ( !player )
		{
			return;
		}
			
		if (player.IsAlive())				
		{
			
			HeroesAndBanditsGuard guard = GetClosestGuard(player.GetPosition());
			/*habPrint("Checking if Player: " + player.GetIdentity().GetName() + " ("+player.GetIdentity().GetPlainId()+") is in Zone " + Name, "Debug");	
			if ( GetHeroesAndBanditsSettings().DebugLogs ){
				player.m_HeroesAndBandits_InZones.Debug();
			}*/
			if (validPlayer(pdata) && vector.Distance(player.GetPosition(), getVector()) <= Radius && !player.habIsInZone(ZoneID, Index)){
				habPrint("Player: " + player.GetIdentity().GetName() + " ("+player.GetIdentity().GetPlainId()+") Entered: " + Name, "Verbose");
				player.habEnteredZone(ZoneID, Index);
				if ( GetHeroesAndBanditsSettings().DebugLogs ){
					player.m_HeroesAndBandits_InZones.Debug();
				}
				if ( ShowWelcomeMsg )
				{
					GetHeroesAndBandits().WelcomePlayer(Name, WelcomeMessage, WelcomeIcon, player.GetIdentity().GetPlainId(), WelcomeMessageColor);
				}
				if ( GodModPlayers && validPlayer(pdata) && !player.habCheckGodMod() )
				{
					player.SetAllowDamage(false);
				}
				if (PreventWeaponRaise && !player.habCanRaiseWeapon() &&  validPlayer(pdata)){
					player.habSetCanRaiseWeapon(false, Index);
				}
			}
			else if (!validPlayer(pdata) && player.habIsInZone(ZoneID, Index) && vector.Distance(player.GetPosition(), getVector()) <= KillRadius && KillRadius != 0)
			{
				if ( OverrideSafeZone && !player.habCheckGodMod() )
				{
					player.SetAllowDamage(true);
				}
				if (guard && !player.habCheckGodMod()){
					if ( (guard.GetClosetPlayerID() == player.GetIdentity().GetPlainId() || guard.GetClosetPlayerDistance() > vector.Distance(player.GetPosition(),guard.GetPosition())) && allowSpinOff){
						float maxDelayAim = GetHeroesAndBanditsZones().ZoneCheckTimer * 1000;
						for (float delayAim = 140; delayAim <= maxDelayAim; delayAim++){
							delayAim = delayAim + 140;
							GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater(this.TrackPlayer, delayAim, false, player, guard);
						}
					}
					FireWeaponClosestGuard(player.GetPosition(), player);
					if ( guard.GunTickMulitplier >= 2 && allowSpinOff){
						float maxDelayGun = GetHeroesAndBanditsZones().ZoneCheckTimer * 1000;
						float delayadd = maxDelayGun / guard.GunTickMulitplier;
						for (float delay = delayadd; delay < maxDelayGun; delay++){
							GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater(this.CheckPlayer, delay, false, player, false);
							delay = delay + delayadd - 1;
						}
					}
				} else {
					player.habSetKilledByZone(Name);
					player.SetHealth(0.0);
				}
				//Player Entered Kill Zone Kill Player if warning has been given
				if (!player.IsAlive()){
					player.habLeftZone(0); //Removed from all zones
					habPrint("Killed Player: " + player.GetIdentity().GetName() + " ("+player.GetIdentity().GetPlainId()+") for Entering Zone: " + Name, "Always");
				}
				if ( GetHeroesAndBanditsSettings().DebugLogs ){
					player.m_HeroesAndBandits_InZones.Debug();
				}

				if ( PreventWeaponRaise ){
					player.habSetCanRaiseWeapon(true, -1);
				}
				
			}
			else if (!validPlayer(pdata) && !player.habIsInZone(ZoneID, Index) && vector.Distance(player.GetPosition(), getVector()) <= Radius)
			{
				//Player Entered Warning Zone Issue Warning
				player.habEnteredZone(ZoneID, Index);
				if ( GetHeroesAndBanditsSettings().DebugLogs ){
					player.m_HeroesAndBandits_InZones.Debug();
				}
				habPrint("Player: " + player.GetIdentity().GetName() + " ("+player.GetIdentity().GetPlainId()+") Entered: " + Name, "Verbose");
				if ( ShowWarningMsg )
				{
					GetHeroesAndBandits().WarnPlayer(Name, WarningMessage, player.GetIdentity().GetPlainId());
				}
				if ((guard.GetClosetPlayerID() == player.GetIdentity().GetPlainId() || guard.GetClosetPlayerDistance() > vector.Distance(player.GetPosition(),guard.GetPosition())) && allowSpinOff ){
					float maxDelayAim2 = GetHeroesAndBanditsZones().ZoneCheckTimer * 1000;
					for (float delayAim2 = 200; delay <= maxDelayAim2; delayAim2++){
						GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater(this.TrackPlayer, delayAim2, false, player, guard);
						delayAim2 = delayAim2 + 200;
					}
				}
				
			}
			else if (vector.Distance(player.GetPosition(), getVector()) > Radius && player.habIsInZone(ZoneID, Index))
			{
				if ( GodModPlayers  && !player.habCheckGodMod() )
				{
					player.SetAllowDamage(true);
				}
				if ( PreventWeaponRaise ){
					player.habSetCanRaiseWeapon(true, -1);
				}
				//Player Left Warning Radius
				habPrint("Player: " + player.GetIdentity().GetName() + " ("+player.GetIdentity().GetPlainId()+") Left: " + Name, "Verbose");
				player.habLeftZone(Index);		
				if ( GetHeroesAndBanditsSettings().DebugLogs ){
					player.m_HeroesAndBandits_InZones.Debug();
				}	
			}
			if ( SubZones && player.habIsInZone(ZoneID, Index) )
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
		Name = name;
		X = x;
		Z = z;
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
	
	bool validPlayer(HeroesAndBanditsPlayer pdata){
		
		if (GetHeroesAndBanditsSettings().Mode != 1 && Affinities.Count() > 0){
			string affinity = pdata.getAffinity().Name;
			for (int i = 0; i < Affinities.Count(); i++){
				if (Affinities.Get(i).Check(pdata.getAffinityPoints(affinity) , affinity))
				{
					return true;
				}
			}
		} 
		if (GetHeroesAndBanditsSettings().Mode != 0 && Affinities.Count() > 0){
			bool DefaultAffinty = false;
			for (int j = 0; j < pdata.Affinities.Count(); j++){
				if ( ( pdata.Affinities.Get(j).Name == "bandit" || pdata.Affinities.Get(j).Name == "hero" ) && GetHeroesAndBanditsSettings().Mode == 2 ){
					//Skipping for Hero and Bandit as it was checked above if in mixed mode
				} else {
					for (int k = 0; k < Affinities.Count(); k++){
						if ( Affinities.Get(i).Check(pdata.Affinities.Get(j).Points,  pdata.Affinities.Get(j).Name)){
							return true;
						} else if (Affinities.Get(i).Affinity == GetHeroesAndBanditsLevels().DefaultAffinity.Name){
							DefaultAffinty = true;
						}
					}
				}
			}
			if (DefaultAffinty && pdata.getAffinity() == GetHeroesAndBanditsLevels().DefaultAffinity)
			{
				return true;
			}
		}
		return false;
	}
	
	void FireWeaponClosestGuard(vector playerPostion, PlayerBase inPlayer)
	{
		PlayerBase player = PlayerBase.Cast(inPlayer);
		if (!Guards || !player)//If no guards defined exit
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
			Guards.Get(closestGuardIndex).SetOrientation(vector.Direction(Guards.Get(closestGuardIndex).GetPosition(),playerPostion).Normalized());
			Guards.Get(closestGuardIndex).RaiseWeapon();
			Guards.Get(closestGuardIndex).FireWeapon();
			string zoneImage = WelcomeIcon;
			float dmg = Guards.Get(closestGuardIndex).DamagePerTickMin;
			if (Guards.Get(closestGuardIndex).DamagePerTickMin < Guards.Get(closestGuardIndex).DamagePerTickRand){
				dmg = Math.RandomFloat(Guards.Get(closestGuardIndex).DamagePerTickMin, Guards.Get(closestGuardIndex).DamagePerTickRand);
			}
			float hitchance = Guards.Get(closestGuardIndex).HitChance + 0.01;
			float hitrand = Math.RandomFloat(0,1);
			if ( hitchance > hitrand ){
				GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater(player.habHitByAI, 50 ,false, Name, dmg, Guards.Get(closestGuardIndex), "chest", "m4a1", zoneImage);
			}
		} 
	}
	
	void TrackPlayer(PlayerBase inPlayer, HeroesAndBanditsGuard inGuard)
	{
		PlayerBase player = PlayerBase.Cast(inPlayer);
		HeroesAndBanditsGuard guard = HeroesAndBanditsGuard.Cast(inGuard);
		if (!guard || !player)//If no guards defined exit
		{
			return;
		}
		vector playerPostion = player.GetPosition();
		vector guardPostion = guard.GetPosition();
		if (player.IsAlive() && player.GetIdentity().GetPlainId() == guard.GetClosetPlayerID()){
			guard.SetClosetPlayerDistance( vector.Distance(guardPostion,playerPostion), player.GetIdentity().GetPlainId());
			guard.SetOrientation(vector.Direction(guardPostion,playerPostion).Normalized());
		} else if (!player.IsAlive() && player.GetIdentity().GetPlainId() == guard.GetClosetPlayerID()) {
			guard.SetClosetPlayerDistance( 600, "");
		} else if ( player.IsAlive() && vector.Distance(guardPostion,playerPostion) < guard.GetClosetPlayerDistance()){
			guard.SetClosetPlayerDistance( vector.Distance(guardPostion,playerPostion), player.GetIdentity().GetPlainId());
		} 
	}
	
	HeroesAndBanditsGuard GetClosestGuard(vector playerPostion)
	{
		if (!Guards)//If no guards defined exit
		{
			return null;
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
			return null;
		} else {
			return Guards.Get(closestGuardIndex);
		} 
	}
	
	HeroesAndBanditsZone GetChild( array< int > inZones){
		int maxIndex = inZones.Count() - 1;
		if (maxIndex == Index){
			return this;
		} else {
			return SubZones.Get(inZones.Get(Index + 1)).GetChild( inZones );
		}
	}
}