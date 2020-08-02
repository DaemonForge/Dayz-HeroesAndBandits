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
	string GunSound;
	float GunTickMulitplier;
	float DamagePerTickMin;
	float DamagePerTickRand;
	float HitChance;
	string ZoneName = "";
	bool IsTrackingPlayer = false;
	bool WeaponIsRaised = false;
	float ClosestPlayerDistance = 600;
	string ClosestPlayerID = "";
	float RespawnTimer = 60;
	bool RespawnTriggered = true;
	bool CanBeKilled = false;
	bool RequireLightOfSight = true;
	bool StopTracking = true;
	string AmmoType = "";
	
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
		if (PlayerBase.Cast(Guard)){
			Guard.Delete();
		}
		habPrint("Spawning Guard: " + Skin + " at " + " X:" + X + " Y:" + Y +" Z:" + Z, "Verbose");	
		Object obj = GetGame().CreateObject( Skin , getVector(), false, false, true );
		obj.SetPosition(getVector());
		Weapon_Base weaponInHands;
		EntityAI weaponInHandsMag;
		if (Class.CastTo(Guard, obj))
		{	
			Guard.habSetGuard();
			Guard.SetAllowDamage(CanBeKilled);
			for ( int i =0; i < GuardGear.Count(); i++ )
			{
				Guard.GetInventory().CreateAttachment(GuardGear.Get(i));
			}
			if (Guard.GetHumanInventory().GetEntityInHands()){
				habPrint("Item spawned in Guard: " + Skin + " at " + " X:" + X + " Y:" + Y +" Z:" + Z + " Item: " + Guard.GetHumanInventory().GetEntityInHands().GetDisplayName() +  " Removing it", "Exception");	
				Guard.GetHumanInventory().GetEntityInHands().Delete(); //Remove any Items in Hand
			}
			weaponInHands = Weapon_Base.Cast(Guard.GetHumanInventory().CreateInHands(WeaponInHands));
			if (weaponInHands){
				int muzzleType = weaponInHands.GetCurrentMuzzle();
				AmmoType = GetGame().ConfigGetTextOut( "CfgMagazines " + weaponInHands.GetChamberAmmoTypeName(muzzleType) + " ammo" );
				for ( int j =0; j < WeaponInHandsAttachments.Count(); j++ )
				{
					weaponInHands.GetInventory().CreateAttachment(WeaponInHandsAttachments.Get(j));
				}
			}
				
		}
		vector guardOrientation = Vector(Orientation, 0, 0);
			
		obj.SetOrientation(guardOrientation);
		RespawnTriggered = false;
	}
	
	void ReloadWeapon()
	{
		habPrint("Reloading Gun Guard: " + Skin + " at " + " X:" + X + " Y:" + Y +" Z:" + Z, "Verbose");	
		Weapon_Base weaponInHands = Weapon_Base.Cast(Guard.GetHumanInventory().GetEntityInHands());
		if (weaponInHands)
		{
			EntityAI mag = EntityAI.Cast(weaponInHands.GetInventory().CreateAttachment(WeaponInHandsMag));
			habPrint("AttachMag", "Debug"); 
			weaponInHands.AttachMagazine(weaponInHands.GetCurrentMuzzle(), Magazine.Cast(mag));
			habPrint("pushToChamberFromAttachedMagazine", "Debug");
			pushToChamberFromAttachedMagazine( weaponInHands, weaponInHands.GetCurrentMuzzle() );
		}
	}
	
	
	void RaiseWeapon()
	{
		if (IsAlive()){
			habPrint("Raise Gun Guard: " + Skin + " at " + " X:" + X + " Y:" + Y +" Z:" + Z, "Verbose");	
			if (Guard && !WeaponIsRaised)
			{	
				WeaponIsRaised = true;
				Guard.habAIRaiseWeaponServer();
			}
		}
	}
	
	void LowerWeapon()
	{
		if (IsAlive()){
			habPrint("Lower Gun Guard: " + Skin + " at " + " X:" + X + " Y:" + Y +" Z:" + Z, "Verbose");	
			if (Guard && WeaponIsRaised)
			{	
				WeaponIsRaised = false;
				Guard.habAILowerWeaponServer();
			}
		}
	}
	
	
	void SetClosetPlayerDistance( float distance, string playerID ){
		ClosestPlayerDistance = distance;
		ClosestPlayerID = playerID;
	}
	
	float GetClosetPlayerDistance(){
		return ClosestPlayerDistance;
	}
	
	string GetClosetPlayerID(){
		return ClosestPlayerID;
	}
	
	void FireWeapon(PlayerBase inPlayer)
	{
		PlayerBase player = PlayerBase.Cast(inPlayer);
		if (!player)
		{
			return;
		}
		if (IsAlive() && player.IsAlive() && (HasLineOfSight(player) && RequireLightOfSight)){
			habPrint("Firing Gun Guard: " + Skin + " at " + " X:" + X + " Y:" + Y +" Z:" + Z, "Verbose");	
			EntityAI weaponInHands = EntityAI.Cast(Guard.GetHumanInventory().GetEntityInHands());
			if (weaponInHands.IsWeapon())
			{
				GetRPCManager().SendRPC("HaB", "RPCPlayGunShotSound", new Param2< string, vector >( GunSound, GetPosition() ), false);//Doing this way for now going to look at more ways later :)
				WeaponManager(PlayerBase.Cast(Guard)).Fire(Weapon_Base.Cast(weaponInHands));
				Guard.habAIFireWeaponServer();
				float dmg = DamagePerTickMin;
				if (DamagePerTickMin < DamagePerTickRand){
					dmg = Math.RandomFloat(DamagePerTickMin, DamagePerTickRand);
				}
				float hitchance = HitChance + 0.001;//Pervent any 0% hit chance
				float hitrand = Math.RandomFloat(0,1);
				if ( CalculateAccuracy(hitchance, vector.Distance(GetPosition(),player.GetPosition())) > hitrand ){
					TStringArray hitZone = {"LeftForeArmRoll","RightArm","LeftArm","RightLeg","LeftLeg","RightForeArmRoll","Torso","Neck","Head","Pelvis","Spine","RightArmExtra","LeftArmExtra","LeftKneeExtra","RightKneeExtra"};
					GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater(player.habHitByAI, 75 ,false, dmg, this, hitZone.GetRandomElement(), AmmoType);//so the gun sound plays slightly(less than 1/10 of a second) before the hit 
				}
			}
		}
	}
	
	bool HasLineOfSight(PlayerBase inPlayer){
		PlayerBase player = PlayerBase.Cast(inPlayer);
		if (!player)
		{
			return false;
		}
		bool found = false;
		int head_index_guard = Guard.GetBoneIndexByName("head");
		int head_index_player = player.GetBoneIndexByName("head");
		int spine_index_player = player.GetBoneIndexByName("LeftArmExtra");
		int RightArm_index_player = player.GetBoneIndexByName("RightArmExtra");
		int LeftArm_index_player = player.GetBoneIndexByName("LeftArmExtra");
		vector guard_head_pos = Guard.GetPosition();
		guard_head_pos[1] = guard_head_pos[1] + 1.6;
		vector player_head_pos = player.GetPosition();
		player_head_pos[1] = player_head_pos[1] + 1.6;
		vector player_Spine_pos = player.GetPosition();
		player_Spine_pos[1] = player_Spine_pos[1] + 1.4;
		if ( head_index_guard != -1 ){ guard_head_pos = Guard.GetBonePositionWS(head_index_guard); } else { habPrint("head_index_guard: " + head_index_guard, "Debug");}
		if ( head_index_player != -1 ){ player_head_pos = player.GetBonePositionWS(head_index_player); } else { habPrint("head_index_player: " + head_index_player, "Debug");}
		if ( spine_index_player != -1 ){ player_Spine_pos = player.GetBonePositionWS(spine_index_player); } else { habPrint("spine_index_player: " + spine_index_player, "Debug");}
		vector head_direction = vector.Direction(guard_head_pos, player_head_pos);
		vector body_direction = vector.Direction(guard_head_pos, player_Spine_pos);
		float head_distance = vector.Distance(guard_head_pos, player_head_pos) + 0.05;
		float body_distance = vector.Distance(guard_head_pos, player_Spine_pos) + 0.05;
		vector end_head_pos = guard_head_pos + (head_direction * head_distance);
		vector end_body_pos = guard_head_pos + (body_direction * body_distance);
		RaycastRVParams params_head = new RaycastRVParams( guard_head_pos, end_head_pos, Guard, 0 );
		RaycastRVParams params_body = new RaycastRVParams( guard_head_pos, end_body_pos, Guard, 0 );
		params_head.sorted = true;
		params_body.sorted = true;
		params_head.flags = CollisionFlags.ALLOBJECTS;
		params_body.flags = CollisionFlags.ALLOBJECTS;
		//params.type = ObjIntersectNone;
		array<ref RaycastRVResult> results_head = new array<ref RaycastRVResult>;
		array<ref RaycastRVResult> results_body = new array<ref RaycastRVResult>;
		DayZPhysics.RaycastRVProxy( params_head, results_head );
		DayZPhysics.RaycastRVProxy( params_body, results_body );
		habPrint("RaycastRVProxy guard_head_pos: " + guard_head_pos + " end_head_pos: " + end_head_pos + " results_head.Count(): " + results_head.Count(), "Debug");
		float player_head_Distance = -1;
		float closest_head_ObjectDistance = -1;
		int headStartIndex = results_head.Count() - 1;
		for (int i = headStartIndex;  i >= 0; i--){
			if ( Object.Cast(results_head.Get( i ).obj)  ){
				if ( results_head.Get( i ).obj == player){
					habPrint("RaycastRVProxy results_head (Found Player) id: " + i + " obj: " + results_head.Get( i ).obj.GetType() + " at pos: " + results_head.Get( i ).obj.GetPosition() + " distance from gaurd: " + vector.Distance(guard_head_pos, results_head.Get( i ).obj.GetPosition()), "Debug");
					player_head_Distance = vector.Distance(guard_head_pos, results_head.Get( i ).obj.GetPosition());
					found = true;
					break;
				} else if (!results_head.Get( i ).obj.IsBush()) {
					habPrint("RaycastRVProxy results_head (Is Not Bush) id: " + i + " obj: " + results_head.Get( i ).obj.GetType() + " at pos: " + results_head.Get( i ).obj.GetPosition() + " distance from gaurd: " + vector.Distance(guard_head_pos, results_head.Get( i ).obj.GetPosition()), "Debug");
					closest_head_ObjectDistance = vector.Distance(guard_head_pos, results_body.Get( i ).obj.GetPosition());
					break;
				} else {
					habPrint("RaycastRVProxy results_head (Is Bush) id: " + i + " obj: " + results_head.Get( i ).obj.GetType() + " at pos: " + results_head.Get( i ).obj.GetPosition() + " distance from gaurd: " + vector.Distance(guard_head_pos, results_head.Get( i ).obj.GetPosition()), "Debug");
				}
			}
		}
		habPrint("RaycastRVProxy guard_head_pos: " + guard_head_pos + " end_body_pos: " + end_body_pos + " results_body.Count(): " + results_body.Count(), "Debug");
		float player_body_Distance = -1;
		float closest_body_ObjectDistance = -1;
		int bodyStartIndex = results_body.Count() - 1;
		for (int j =  bodyStartIndex; j >= 0; j--){
			if ( Object.Cast(results_body.Get( j ).obj) ){
				if ( results_body.Get( j ).obj == player){
					habPrint("RaycastRVProxy results_body (Found Player) id: " + j + " obj: " + results_body.Get( j ).obj.GetType() + " at pos: " + results_body.Get( j ).obj.GetPosition() + " distance from gaurd: " + vector.Distance(guard_head_pos, results_body.Get( j ).obj.GetPosition()), "Debug");
					player_body_Distance = vector.Distance(guard_head_pos, results_body.Get( j ).obj.GetPosition());
					found = true;
					break;
				} else if (!results_body.Get( j ).obj.IsBush() && !results_body.Get( j ).obj.IsTree()) {
					habPrint("RaycastRVProxy results_body (Is Not Bush) id: " + j + " obj: " + results_body.Get( j ).obj.GetType() + " at pos: " + results_body.Get( j ).obj.GetPosition() + " distance from gaurd: " + vector.Distance(guard_head_pos, results_body.Get( j ).obj.GetPosition()), "Debug");
					closest_body_ObjectDistance = vector.Distance(guard_head_pos, results_body.Get( j ).obj.GetPosition());
					break;
				} else {
					habPrint("RaycastRVProxy results_body (Is Bush) id: " + j + " obj: " + results_body.Get( j ).obj.GetType() + " at pos: " + results_body.Get( j ).obj.GetPosition() + " distance from gaurd: " + vector.Distance(guard_head_pos, results_body.Get( j ).obj.GetPosition()), "Debug");
				}
			}
		}
		return found;
	}
		
	vector getVector(){
		return Vector( X, Y, Z );
	}
	
	vector GetPosition(){
		return Vector( X, Y, Z );
	}
	
	void SetDirection(vector new_Direction){
		if (IsAlive()){
			habPrint("Setting Direction Guard: " + Skin + " at " + " X:" + X + " Y:" + Y +" Z:" + Z + " to " + new_Direction, "Debug");	
			Guard.SetDirection(new_Direction);
		}
	}
	
	void SetOrientation(vector new_Orientation){
		if (IsAlive()){
			habPrint("Rotating Guard: " + Skin + " at " + " X:" + X + " Y:" + Y +" Z:" + Z + " to " + new_Orientation, "Debug");	
			Guard.SetOrientation(new_Orientation);
		}
	}
	
	string GetWeaponName(){
		EntityAI weaponInHands = EntityAI.Cast(Guard.GetHumanInventory().GetEntityInHands());
		string weaponName = "";
		if (weaponInHands){
			weaponName = weaponInHands.GetDisplayName();
		}
		return weaponName;
	}
	
	bool IsAlive(){
		if (!Guard.IsAlive() && !RespawnTriggered){
			RespawnTriggered = true;
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater(this.Spawn, RespawnTimer * 1000 ,false);
		}
		return Guard.IsAlive();
	}
	
	float CalculateAccuracy(float accuracy, float distance){
		float newAccuracy = accuracy;
		float calcDistance = 30;
		while (calcDistance  < distance){
			calcDistance = calcDistance * 2;
			newAccuracy = newAccuracy * accuracy;
		}
		return newAccuracy;
	}
	
	void TrackPlayer(PlayerBase inPlayer, float timeSeconds = 0, float intervalMiliSeconds = 150)
	{
		StopTracking = false;
		PlayerBase player = PlayerBase.Cast(inPlayer);
		if (!player || intervalMiliSeconds < 10)
		{
			return;
		}
		RaiseWeapon();
		float MaxTime = timeSeconds * 1000;
		float interval = 0;
		this.TrackingPlayerTick(player);
		while ( interval <= MaxTime ){
			interval = interval + intervalMiliSeconds;
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater(this.TrackingPlayerTick, interval, false, player);
		}
		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater(this.UnTrackPlayer, interval + intervalMiliSeconds, false, player);
	}
	
	protected void TrackingPlayerTick(PlayerBase inPlayer){
		
		PlayerBase player = PlayerBase.Cast(inPlayer);
		if (!player)
		{
			return;
		}
		vector playerPostion = player.GetPosition();
		vector guardPostion = GetPosition();
		
		if (player.IsAlive() && player.GetIdentity().GetPlainId() == GetClosetPlayerID()){
			SetClosetPlayerDistance( vector.Distance(guardPostion,playerPostion), player.GetIdentity().GetPlainId());
			SetDirection(vector.Direction(guardPostion,playerPostion).Normalized());
		} else if (!player.IsAlive() && player.GetIdentity().GetPlainId() == GetClosetPlayerID()) {
			UnTrackPlayer(player);
		} else if ( player.IsAlive() && vector.Distance(guardPostion,playerPostion) < GetClosetPlayerDistance()){
			SetClosetPlayerDistance( vector.Distance(guardPostion,playerPostion), player.GetIdentity().GetPlainId());
		} 
	}

	void UnTrackPlayer(PlayerBase inPlayer){
		
		PlayerBase player = PlayerBase.Cast(inPlayer);
		if (!player)
		{
			return;
		}
		if (StopTracking && player.GetIdentity().GetPlainId() == GetClosetPlayerID()){
			vector guardOrientation = Vector(Orientation, 0, 0);
			SetOrientation(guardOrientation);
			SetClosetPlayerDistance( 600, "");
			LowerWeapon();
		} else if (StopTracking && GetClosetPlayerID() != player.GetIdentity().GetPlainId()) {
			StopTracking = false;
		} else {
			StopTracking = true;
		}
	}
}



