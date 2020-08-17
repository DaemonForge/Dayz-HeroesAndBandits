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
	bool RequireLineOfSight = true;
	bool StopTracking = true;
	bool InteruptRotate = false;
	string AmmoType = "";
	vector DefaultDirection;
	bool ReadyForTracking = true;
	
    void HeroesAndBanditsGuard(float x, float y, float z, float orientation, string skin, string weaponInHands, string weaponInHandsMag, TStringArray weaponInHandsAttachments, TStringArray guardGear) 
	{
		X = x;
		Y = y;
		Z = z;
		Orientation = orientation;
		Skin = skin;
		WeaponInHands = weaponInHands;
		WeaponInHandsMag = weaponInHandsMag;
		WeaponInHandsAttachments = weaponInHandsAttachments;
		GuardGear = guardGear;
		
    }
	
	void Spawn()
	{		
		if(!RespawnTriggered){
			return;
		}
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
			if(GuardGear && GuardGear.Count() > 0){
				for ( int i =0; i < GuardGear.Count(); i++ )
				{
					Guard.GetInventory().CreateAttachment(GuardGear.Get(i));
				}
			}
			if (Guard.GetHumanInventory().GetEntityInHands()){
				habPrint("Item spawned in Guard: " + Skin + " at " + " X:" + X + " Y:" + Y +" Z:" + Z + " Item: " + Guard.GetHumanInventory().GetEntityInHands().GetDisplayName() +  " Removing it", "Exception");	
				Guard.GetHumanInventory().GetEntityInHands().Delete(); //Remove any Items in Hand
			}
			weaponInHands = Weapon_Base.Cast(Guard.GetHumanInventory().CreateInHands(WeaponInHands));
			if (weaponInHands){
				int muzzleType = weaponInHands.GetCurrentMuzzle();
				AmmoType = GetGame().ConfigGetTextOut( "CfgMagazines " + weaponInHands.GetChamberAmmoTypeName(muzzleType) + " ammo" );
				if(WeaponInHandsAttachments && WeaponInHandsAttachments.Count() > 0){
					for ( int j =0; j < WeaponInHandsAttachments.Count(); j++ )
					{
						weaponInHands.GetInventory().CreateAttachment(WeaponInHandsAttachments.Get(j));
					}
				}
			}
		}
		ClosestPlayerDistance = 600;
		ClosestPlayerID = "";
		WeaponIsRaised = false;
		IsTrackingPlayer = false;
		vector guardOrientation = Vector(Orientation, 0, 0);
			
		obj.SetOrientation(guardOrientation);
		DefaultDirection = Guard.GetDirection();
		RespawnTriggered = false;
		habPrint("Guard: " + Skin + " at " + " X:" + X + " Y:" + Y +" Z:" + Z + " Finished Spawning", "Debug");	
	}
	
	void ReloadWeapon()
	{
		habPrint("Reloading Gun Guard: " + Skin + " at " + " X:" + X + " Y:" + Y +" Z:" + Z, "Debug");	
		Weapon weapon = Weapon.Cast(Guard.GetHumanInventory().GetEntityInHands());
		DayZPlayer GuardZ = Guard;
			if (weapon && GuardZ && WeaponInHandsMag != "") {	
				Guard.GetInventory().CreateAttachment(WeaponInHandsMag);
				ref array<Magazine> mag_Array = new array<Magazine>;
				DayZPlayerUtils.FindMagazinesForAmmo(Guard, WeaponInHandsMag, mag_Array);
				{	
					Magazine mag_obj = Magazine.Cast(mag_Array.Get(0));	
					Guard.GetDayZPlayerInventory().PostWeaponEvent( new WeaponEventAttachMagazine(GuardZ, mag_obj) );
				}
			}
	}
	
	
	void RaiseWeapon()
	{
		if (IsAlive()){
			habPrint("Raise Gun Guard: " + Skin + " at " + " X:" + X + " Y:" + Y +" Z:" + Z, "Debug");	
			if (Guard && !WeaponIsRaised)
			{	
				GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater(this.DelayedRaiseWeapon, 1300, false);
				Guard.habAIRaiseWeaponServer();
			}
		}
	}
	
	void DelayedRaiseWeapon(){
		WeaponIsRaised = true;
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
		habPrint("Attempt Fire Weapon Guard: " + Skin + " at " + " X:" + X + " Y:" + Y +" Z:" + Z, "Debug");	
		PlayerBase player = PlayerBase.Cast(inPlayer);
		if (!player)
		{
			return;
		}
		bool lineOfSight = !RequireLineOfSight;
		bool possibleHits = HasLineOfSight(player) ;
		float dirDiff = GetRotateDiff(Guard.GetDirection(), vector.Direction(Guard.GetPosition(), player.GetPosition()));
		if (possibleHits > 0 && dirDiff < 1.5 && dirDiff > -1.5){
			habPrint("Guard tried to shoot with possibleHits: " + possibleHits + " dirDiff: " + dirDiff + " RequireLineOfSight: " + RequireLineOfSight,"Debug");
			lineOfSight = true;
		} else {
			habPrint("Guard tried to shoot but couldn't possibleHits: " + possibleHits + " dirDiff: " + dirDiff + " RequireLineOfSight: " + RequireLineOfSight,"Debug");
		}
		string hitZone = "";
		TStringArray FullHitZone = {"LeftForeArmRoll","RightArm","LeftArm","RightLeg","LeftLeg","RightForeArmRoll","Torso","Neck","Head","Pelvis","Spine","RightArmExtra","LeftArmExtra","LeftKneeExtra","RightKneeExtra"};
		TStringArray HeadHitZone = {"Neck","Head"};
		TStringArray RightHitZone = {"RightForeArmRoll","RightArm","RightLeg","Torso","RightArmExtra","RightKneeExtra"};
		TStringArray LeftHitZone = {"LeftForeArmRoll","LeftArm","LeftLeg","Torso","LeftArmExtra","LeftKneeExtra"};
		if ( possibleHits == 1 ){
			hitZone = HeadHitZone.GetRandomElement();
		} else if ( possibleHits == 2 ){
			hitZone = RightHitZone.GetRandomElement();
		} else if ( possibleHits == 4 ){
			hitZone = LeftHitZone.GetRandomElement();
		} else {
			hitZone = FullHitZone.GetRandomElement();
		}
		
		if (IsAlive() && player.IsAlive() && lineOfSight && WeaponIsRaised ){
			habPrint("Firing Gun Guard: " + Skin + " at " + " X:" + X + " Y:" + Y +" Z:" + Z, "Verbose");	
			EntityAI weaponInHands = EntityAI.Cast(Guard.GetHumanInventory().GetEntityInHands());
			if (weaponInHands.IsWeapon())
			{
				GetRPCManager().SendRPC("HaB", "RPCPlayGunShotSound", new Param2< string, vector >( GunSound, GetPosition() ), false);//Doing this way for now going to look at more ways later :)
				//WeaponManager(PlayerBase.Cast(Guard)).Fire(Weapon_Base.Cast(weaponInHands));
				Guard.habAIFireWeaponServer();
				float dmg = DamagePerTickMin;
				if (DamagePerTickMin < DamagePerTickRand){
					dmg = Math.RandomFloat(DamagePerTickMin, DamagePerTickRand);
				}
				float hitchance = HitChance + 0.001;//Pervent any 0% hit chance
				float hitrand = Math.RandomFloat(0,1);
				if ( CalculateAccuracy(player) > hitrand ){
					GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater(player.habHitByAI, 75 ,false, dmg, this, hitZone, AmmoType);//so the gun sound plays slightly(less than 1/10 of a second) before the hit 
				}
			}
		}
	}
	
	
	//Returns a binary hit table ( 1 Left Arm | 1 Right Arm | 1 Head)
	int HasLineOfSight(PlayerBase inPlayer){
		PlayerBase player = PlayerBase.Cast(inPlayer);
		if (!player)
		{
			return false;
		}
		int found = 0;
		
		int head_index_guard = Guard.GetBoneIndexByName("head");
		vector guard_head_pos = Guard.GetPosition();
		guard_head_pos[1] = guard_head_pos[1] + 1.6;
		if ( head_index_guard != -1 ){ guard_head_pos = Guard.GetBonePositionWS(head_index_guard); } else { habPrint("head_index_guard: " + head_index_guard, "Debug");}
		
		
		int head_index_player = player.GetBoneIndexByName("head");
		int RightArm_index_player = player.GetBoneIndexByName("RightArmExtra");
		int LeftArm_index_player = player.GetBoneIndexByName("LeftArmExtra");
		
		vector player_head_pos = player.GetPosition();
		player_head_pos[1] = player_head_pos[1] + 1.6;
		vector player_RightArm_pos = player.GetPosition();
		player_RightArm_pos[1] = player_RightArm_pos[1] + 1.4;
		vector player_LeftArm_pos = player.GetPosition();
		player_LeftArm_pos[1] = player_LeftArm_pos[1] + 1.4;
		
		if ( head_index_player != -1 ){ player_head_pos = player.GetBonePositionWS(head_index_player); } else { habPrint("head_index_player: " + head_index_player, "Debug");}
		if ( RightArm_index_player != -1 ){ player_RightArm_pos = player.GetBonePositionWS(RightArm_index_player); } else { habPrint("RightArm_index_player: " + RightArm_index_player, "Debug");}
		if ( LeftArm_index_player != -1 ){ player_LeftArm_pos = player.GetBonePositionWS(LeftArm_index_player); } else { habPrint("LeftArm_index_player: " + LeftArm_index_player, "Debug");}
		
		vector head_direction = vector.Direction(guard_head_pos, player_head_pos);
		vector RightArm_direction = vector.Direction(guard_head_pos, player_RightArm_pos);
		vector LeftArm_direction = vector.Direction(guard_head_pos, player_LeftArm_pos);
		
		float head_distance = vector.Distance(guard_head_pos, player_head_pos) + 0.05;
		float RightArm_distance = vector.Distance(guard_head_pos, player_RightArm_pos) + 0.05;
		float LeftArm_distance = vector.Distance(guard_head_pos, player_LeftArm_pos) + 0.05;
		
		vector end_head_pos = guard_head_pos + (head_direction * head_distance);
		vector end_RightArm_pos = guard_head_pos + (RightArm_direction * RightArm_distance);
		vector end_LeftArm_pos = guard_head_pos + (LeftArm_direction * LeftArm_distance);
		
		RaycastRVParams params_head = new RaycastRVParams( guard_head_pos, end_head_pos, Guard, 0 );
		params_head.sorted = true;
		params_head.flags = CollisionFlags.ALLOBJECTS;
		array<ref RaycastRVResult> results_head = new array<ref RaycastRVResult>;
		DayZPhysics.RaycastRVProxy( params_head, results_head );
		
		RaycastRVParams params_RightArm = new RaycastRVParams( guard_head_pos, end_RightArm_pos, Guard, 0 );
		params_RightArm.sorted = true;
		params_RightArm.flags = CollisionFlags.ALLOBJECTS;
		array<ref RaycastRVResult> results_RightArm = new array<ref RaycastRVResult>;
		DayZPhysics.RaycastRVProxy( params_RightArm, results_RightArm );
		
		RaycastRVParams params_LeftArm = new RaycastRVParams( guard_head_pos, end_LeftArm_pos, Guard, 0 );
		params_LeftArm.sorted = true;
		params_LeftArm.flags = CollisionFlags.ALLOBJECTS;
		array<ref RaycastRVResult> results_LeftArm = new array<ref RaycastRVResult>;
		DayZPhysics.RaycastRVProxy( params_LeftArm, results_LeftArm );
		
		int headStartIndex = results_head.Count() - 1;
		for (int i = headStartIndex;  i >= 0; i--){
			if ( Object.Cast(results_head.Get( i ).obj)  ){
				if ( results_head.Get( i ).obj == player){
					habPrint("RaycastRVProxy results_head (Found Player) id: " + i + " obj: " + results_head.Get( i ).obj.GetType() + " at pos: " + results_head.Get( i ).obj.GetPosition() + " distance from gaurd: " + vector.Distance(guard_head_pos, results_head.Get( i ).obj.GetPosition()), "Debug");
					found = found + 1;
					break;
				} else if (!results_head.Get( i ).obj.IsBush()) {
					habPrint("RaycastRVProxy results_head (Is Not Bush) id: " + i + " obj: " + results_head.Get( i ).obj.GetType() + " at pos: " + results_head.Get( i ).obj.GetPosition() + " distance from gaurd: " + vector.Distance(guard_head_pos, results_head.Get( i ).obj.GetPosition()), "Debug");
					break;
				} else {
					habPrint("RaycastRVProxy results_head (Is Bush) id: " + i + " obj: " + results_head.Get( i ).obj.GetType() + " at pos: " + results_head.Get( i ).obj.GetPosition() + " distance from gaurd: " + vector.Distance(guard_head_pos, results_head.Get( i ).obj.GetPosition()), "Debug");
				}
			}
		}
		int RightArmStartIndex = results_RightArm.Count() - 1;
		for (int j =  RightArmStartIndex; j >= 0; j--){
			if ( Object.Cast(results_RightArm.Get( j ).obj) ){
				if ( results_RightArm.Get( j ).obj == player){
					//habPrint("RaycastRVProxy results_RightArm (Found Player) id: " + j + " obj: " + results_RightArm.Get( j ).obj.GetType() + " at pos: " + results_RightArm.Get( j ).obj.GetPosition() + " distance from gaurd: " + vector.Distance(guard_head_pos, results_RightArm.Get( j ).obj.GetPosition()), "Debug");
					found = found + 2;
					break;
				} else if (!results_RightArm.Get( j ).obj.IsBush()) {
					//habPrint("RaycastRVProxy results_RightArm (Is Not Bush) id: " + j + " obj: " + results_RightArm.Get( j ).obj.GetType() + " at pos: " + results_RightArm.Get( j ).obj.GetPosition() + " distance from gaurd: " + vector.Distance(guard_head_pos, results_RightArm.Get( j ).obj.GetPosition()), "Debug");
					break;
				} else {
					//habPrint("RaycastRVProxy results_RightArm (Is Bush) id: " + j + " obj: " + results_RightArm.Get( j ).obj.GetType() + " at pos: " + results_RightArm.Get( j ).obj.GetPosition() + " distance from gaurd: " + vector.Distance(guard_head_pos, results_RightArm.Get( j ).obj.GetPosition()), "Debug");
				}
			}
		}
		int LeftArmStartIndex = results_LeftArm.Count() - 1;
		for (int k =  LeftArmStartIndex; k >= 0; k--){
			if ( Object.Cast(results_LeftArm.Get( k ).obj) ){
				if ( results_LeftArm.Get( k ).obj == player){
					//habPrint("RaycastRVProxy results_LeftArm (Found Player) id: " + k + " obj: " + results_LeftArm.Get( k ).obj.GetType() + " at pos: " + results_LeftArm.Get( k ).obj.GetPosition() + " distance from gaurd: " + vector.Distance(guard_head_pos, results_LeftArm.Get( k ).obj.GetPosition()), "Debug");
					found = found + 4;
					break;
				} else if (!results_LeftArm.Get( k ).obj.IsBush()) {
					//habPrint("RaycastRVProxy results_LeftArm ( Can't Shoot Through) id: " + k + " obj: " + results_LeftArm.Get( k ).obj.GetType() + " at pos: " + results_LeftArm.Get( k ).obj.GetPosition() + " distance from gaurd: " + vector.Distance(guard_head_pos, results_LeftArm.Get( k ).obj.GetPosition()), "Debug");
					break;
				} else {
					//habPrint("RaycastRVProxy results_LeftArm ( Can Shoot Through) id: " + k + " obj: " + results_LeftArm.Get( k ).obj.GetType() + " at pos: " + results_LeftArm.Get( k ).obj.GetPosition() + " distance from gaurd: " + vector.Distance(guard_head_pos, results_LeftArm.Get( k ).obj.GetPosition()), "Debug");
				}
			}
		}
		habPrint("Found Player " + player.GetIdentity().GetName() + " with LineofSight: " + found, "Debug");
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
			StopTracking = true;
			InteruptRotate = false;
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater(this.Spawn, RespawnTimer * 1000 ,false);
		}
		return Guard.IsAlive();
	}

	float CalculateAccuracy(PlayerBase inPlayer){
		PlayerBase player = PlayerBase.Cast(inPlayer);
		if (!player){
			return HitChance;
		}
		float distance = vector.Distance(Guard.GetPosition(), player.GetPosition());
		float newAccuracy = HitChance;
		float calcDistance = 30;
		while (calcDistance  < distance){
			calcDistance = calcDistance * 2;
			newAccuracy = newAccuracy * HitChance;
		}
		vector runTowardsDirection = vector.Direction(player.GetPosition(), Guard.GetPosition());
		vector runAwayDirection = vector.Direction(Guard.GetPosition(), player.GetPosition());
		vector playerDirection = player.GetDirection();
		float playerSpeed = player.GetCommand_Move().GetCurrentMovementSpeed();
		float awayDif = GetRotateDiff(playerDirection, runTowardsDirection);
		if ( awayDif < 0 ){ awayDif = -awayDif;}
		habPrint("Away Dir: " + awayDif ,"Debug");
		float toDif = GetRotateDiff(playerDirection, runAwayDirection);
		if ( toDif < 0 ){ toDif = -toDif;}
		habPrint("To Dir: " + toDif ,"Debug");
		if (playerSpeed < 0.5 || toDif < 1.5 ||  toDif > -1.5){
			habPrint("Player Direction: " + playerDirection + " Player Speed: " + playerSpeed + " runTowardsDirection: " + runTowardsDirection + " runAwayDirection: " + runAwayDirection ,"Debug");
		} else {
			habPrint("Accuracy Reduction player moving Player Direction: " + playerDirection + " Player Speed: " + playerSpeed + " runTowardsDirection: " + runTowardsDirection + " runAwayDirection: " + runAwayDirection ,"Debug");	
			newAccuracy = newAccuracy * HitChance;
		}
		return newAccuracy;
	}
	
	void RotateToFace(vector direction, float maxTimeSeconds = 2, int tickInterval = 100){
		float toDif = GetRotateDiff( Guard.GetDirection(), direction);
		if ( toDif < 0 ){ toDif = -toDif;}
		if ( toDif  < 0.8){
			return;
		}
		int TickCount = maxTimeSeconds * 1000 / tickInterval;
		RotateToFaceTick(direction, TickCount, tickInterval);
	}
	
	float GetRotateDiff(vector curDirection, vector optimalDirection){
		float curX = curDirection[0];
		float optimalX = optimalDirection[0];
		float curZ = curDirection[2];
		float optimalZ = optimalDirection[2];
		float dif = vector.Distance(Vector(curX, 0, curZ), Vector(optimalX, 0,optimalZ)); 
		vector dir = vector.Direction(Vector(curX, 0, curZ), Vector(optimalX, 0,optimalZ));
		if ( dir[0] < 0){
			
			dif = -dif;
		}
		
		habPrint("curDirection: " + curDirection + " optimalDirection: " + optimalDirection + " dif: " + dif + " dir: " + dir, "Debug");
		return dif;
	}
	
	float GetRotateDiffX(vector curDirection, vector optimalDirection){
		float curX = curDirection[0];
		float optimalX = optimalDirection[0];
		float difX = vector.Distance(Vector(curX, 0, 0), Vector(optimalX, 0,0)); 
		vector dirX = vector.Direction(Vector(curX, 0, 0), Vector(optimalX, 0,0));
		if ( dirX[0] < 0){
			
			difX = -difX;
		}
		
		habPrint("curDirection: " + curDirection + " optimalDirection: " + optimalDirection + " difX: " + difX + " dirX: " + dirX, "Debug");
		return difX;
	}
	
	float GetRotateDiffZ(vector curDirection, vector optimalDirection){
		float curZ = curDirection[2];
		float optimalZ = optimalDirection[2];
		float difZ = vector.Distance(Vector(0, 0, curZ), Vector(0, 0,optimalZ)); 
		vector dirZ = vector.Direction(Vector(0, 0, curZ), Vector(0, 0,optimalZ));
		if ( dirZ[2] < 0){
			
			difZ = -difZ;
		}
		
		habPrint("curDirection: " + curDirection + " optimalDirection: " + optimalDirection + " difZ: " + difZ + " dirZ: " + dirZ, "Debug");
		return difZ;
	}
	
	protected void RotateToFaceTick(vector direction, int maxCount = 25, int tickInterval = 80){
		vector curDir = Guard.GetDirection();
		float dirX = GetRotateDiffX(curDir, direction);
		float dirZ = GetRotateDiffZ(curDir, direction);
		float newX = curDir[0];
		float newZ = curDir[2];
		
		
		float dir  = GetRotateDiff(curDir, direction);
		if ( dir < 1 || dir > -1){
			Guard.habAIAimWeaponServer( 0 );
		} else if (dir >= 1 ){
			Guard.habAIAimWeaponServer( 10 );
		} else if (dir <= -1 ){
			Guard.habAIAimWeaponServer( 10 );
		}
		
		if ( dirX < 0.5 || dirX > -0.5){
			newX = direction[0];
			maxCount = 0;
		} else if (dirX >= 0.5 ){
			newX = newX + 0.5;
		} else if (dirX <= -0.5 ){
			newX = newX - 0.5;
		}
		if ( dirZ < 0.08 || dirZ > -0.08){
			newZ = direction[2];
			maxCount = 0;
		} else if (dirZ >= 0.08 ){
			newZ = newZ + 0.08;
		} else if (dirZ <= -0.08 ){
			newZ = newZ - 0.08;
		}
		Guard.SetDirection(Vector( newX,direction[1],newZ));
		if (GetRotateDiff(curDir, direction) < 0.4){
			maxCount = 0;
			Guard.SetDirection(Vector( direction[0],direction[1],direction[2]));
		}
		if (maxCount > 0 && tickInterval > 10 && !InteruptRotate){
			float newCount = maxCount - 1;
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater(this.RotateToFaceTick, tickInterval, false, direction, tickInterval, newCount );

		}
		if (maxCount != 0 && InteruptRotate){ //Currently only way to intrupt a rotate is via TrackPlayer
			InteruptRotate = false;
		}
	}
	
	bool IsReadyToTrack(){
		return ReadyForTracking && IsAlive();
	}
	
	void MakeReadyToTrack(){
		ReadyForTracking = true;
	}

	void TrackPlayer(PlayerBase inPlayer, float timeSeconds = 0, float intervalMiliSeconds = 80)
	{
		ReadyForTracking = false;
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
		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater(this.MakeReadyToTrack, interval - intervalMiliSeconds, false);
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
		InteruptRotate = true;
		if (player.IsAlive() && player.GetIdentity().GetPlainId() == GetClosetPlayerID()){
			SetClosetPlayerDistance( vector.Distance(guardPostion,playerPostion), player.GetIdentity().GetPlainId());
			RotateToFaceTick(vector.Direction(guardPostion, playerPostion), 0, 0);
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
			RotateToFace(DefaultDirection, 3, 80);
			SetClosetPlayerDistance( 600, "");
			LowerWeapon();
		} else if (StopTracking && GetClosetPlayerID() != player.GetIdentity().GetPlainId()) {
			StopTracking = false;
		} else {
			StopTracking = true;
		}
	}
}



