class habConverter
{
	static ref map<string, string> GunToSound = new ref map<string, string>;
	static ref map<string, string> GunToMag= new ref map<string, string>;
	static ref map<string, float> GunToDmg = new ref map<string, float>;

	void habConverter(){
	}
	
	static void init(){
		if (GunToSound.Count() <= 0){
			GunToSound.Insert("M4A1", "M4_Shot_SoundSet");
			GunToMag.Insert("M4A1", "Mag_STANAGCoupled_30Rnd");
			GunToDmg.Insert("M4A1", 18);
			
			GunToSound.Insert("M4A1_Green", "M4_Shot_SoundSet");
			GunToMag.Insert("M4A1_Green", "Mag_STANAGCoupled_30Rnd");
			GunToDmg.Insert("M4A1_Green", 18);
			
			GunToSound.Insert("M4A1_Black", "M4_Shot_SoundSet");
			GunToMag.Insert("M4A1_Black", "Mag_STANAGCoupled_30Rnd");
			GunToDmg.Insert("M4A1_Black", 18);
			
			GunToSound.Insert("AK74", "AK_Shot_SoundSet");
			GunToMag.Insert("AK74", "Mag_AK74_30Rnd");
			GunToDmg.Insert("AK74", 28);
			
			GunToSound.Insert("AK74_Black", "AK_Shot_SoundSet");
			GunToMag.Insert("AK74_Black", "Mag_AK74_30Rnd_Black");
			GunToDmg.Insert("AK74_Black", 28);
			
			GunToSound.Insert("AK74_Green", "AK_Shot_SoundSet");
			GunToMag.Insert("AK74_Green", "Mag_AK74_30Rnd_Green");
			GunToDmg.Insert("AK74_Green", 28);
			
			GunToSound.Insert("UMP45", "UMP45_Shot_SoundSet");
			GunToMag.Insert("UMP45", "Mag_UMP_25Rnd");
			GunToDmg.Insert("UMP45", 16);
			
			GunToSound.Insert("SKS", "SKS_Shot_SoundSet");
			GunToMag.Insert("SKS", "Mag_CLIP762x39_10Rnd");
			GunToDmg.Insert("SKS", 65);
			
			GunToSound.Insert("SKS_Black", "SKS_Shot_SoundSet");
			GunToMag.Insert("SKS_Black", "Mag_CLIP762x39_10Rnd");
			GunToDmg.Insert("SKS_Black", 65);
			
			GunToSound.Insert("SKS_Black", "SKS_Shot_SoundSet");
			GunToMag.Insert("SKS_Black", "Mag_CLIP762x39_10Rnd");
			GunToDmg.Insert("SKS_Black", 65);
			
			GunToSound.Insert("SVD", "SVD_Shot_SoundSet");
			GunToMag.Insert("SVD", "");
			GunToDmg.Insert("SVD", 80);
			
			GunToSound.Insert("FAL", "FNFAL_Shot_SoundSet");
			GunToMag.Insert("FAL", "Mag_FAL_20Rnd");
			GunToDmg.Insert("FAL", 80);
			
			GunToMag.Insert("FAL", "Mag_FAL_20Rnd");
			GunToDmg.Insert("FAL", 80);
			
			GunToSound.Insert("Expansion_AWM", "SVD_Shot_SoundSet");
			GunToMag.Insert("Expansion_AWM", "Mag_Expansion_AWM_5rnd");
			GunToDmg.Insert("Expansion_AWM", 80);
			
			GunToSound.Insert("Expansion_G36", "M4_Shot_SoundSet");
			GunToMag.Insert("Expansion_G36", "Mag_STANAG_30Rnd");
			GunToDmg.Insert("Expansion_G36", 20);
			
			GunToSound.Insert("ussr_m200", "m200_silencerHomeMade_SoundSet");
			GunToMag.Insert("ussr_m200", "Mag_ussrm200_7Rnd");
			GunToDmg.Insert("ussr_m200", 100);
			
			GunToSound.Insert("ussr_m200_black", "m200_Shot_SoundSet");
			GunToMag.Insert("ussr_m200_black", "Mag_ussrm200_7Rnd");
			GunToDmg.Insert("ussr_m200_black", 100);
		}
	}
	
	static ref HeroesAndBanditsSettings ConvertSettings(ref HeroesAndBanditsSimpleConfig simpConfig){
		ref HeroesAndBanditsSettings tempSettings = new ref HeroesAndBanditsSettings();
		tempSettings.KillFeed = simpConfig.KillFeed;
		tempSettings.SucideFeed = simpConfig.SucideFeed;
		tempSettings.BanditCanRemoveMask = simpConfig.BanditCanRemoveMask;
		tempSettings.BanditCanRemoveArmBand = simpConfig.BanditCanRemoveArmBand;
		tempSettings.BanditMasks = simpConfig.BanditMasks;
		tempSettings.BanditArmBands = simpConfig.BanditArmBands;
		
		tempSettings.HeroCanRemoveMask = simpConfig.HeroCanRemoveMask;
		tempSettings.HeroCanRemoveArmBand = simpConfig.HeroCanRemoveArmBand;
		tempSettings.HeroMasks = simpConfig.HeroMasks;
		tempSettings.HeroArmBands = simpConfig.HeroArmBands;
		tempSettings.Expansion_EnableIconOnPlayerTag = simpConfig.Expansion_EnableIconOnPlayerTag;
		tempSettings.AllowStatCommand = false;
		tempSettings.AllowHumanityCommand = false;
		tempSettings.VerboseLogs = true;
		return tempSettings;
	}
	
	static ref HeroesAndBanditsConfigZones ConvertZones(ref HeroesAndBanditsSimpleConfig simpConfig){
		ref HeroesAndBanditsConfigZones tempSettings = new ref HeroesAndBanditsConfigZones();
		if (simpConfig.Zones.Count() > 0){
			tempSettings.ZoneCheckTimer = 3;
			for (int i = 0; i < simpConfig.Zones.Count(); i++){
				tempSettings.Zones.Insert(ConvertZone(simpConfig.Zones.Get(i)));
			}
		} else {
			tempSettings.ZoneCheckTimer = 0;
		}
		return tempSettings;
	}
	
	
	static ref habZone ConvertZone(ref HABSimpleZone simpZone){
		string Name = simpZone.Name;
		float X = simpZone.X;
		float Z = simpZone.Z;
		float Radius = simpZone.Radius;
		float WarnRadius = simpZone.Radius * 1.15;
		float MinHumanity = simpZone.MinHumanity;
		float MaxHumanity = simpZone.MaxHumanity;
		if (MinHumanity == -1 && MaxHumanity == -1){
			Radius = Radius;
			WarnRadius = Radius;
		}
		if (MinHumanity == 0 && MaxHumanity == 0){
			Radius = Radius;
			WarnRadius = Radius;
		}
		string WelcomeMessage =  simpZone.WelcomeMessage;
		string WarningMessage =  simpZone.WarningMessage;
		bool OverrideSafeZone =  simpZone.OverrideSafeZone;
		bool GodModPlayers =  simpZone.GodModPlayers;
		bool PreventWeaponRaise =  simpZone.PreventWeaponRaise;
		bool PreventActions =  simpZone.PreventActions;
		bool PreventTrade =  simpZone.PreventTrade;
		bool KillAggressors = simpZone.KillAggressors;
		float RespawnTimer = simpZone.RespawnTimer;
		int GuardDifficulty =  simpZone.GuardDifficulty;
		ref habZone tempZone =  new ref  habZone(Name, X, Z, WarnRadius, Radius, WarningMessage, OverrideSafeZone, GodModPlayers);
		tempZone.MinHumanity = MinHumanity;
		tempZone.MaxHumanity = MaxHumanity;
		tempZone.convertHumanityToAffinty();
		if (WelcomeMessage == ""){
			tempZone.ShowWelcomeMsg = false;
			tempZone.WelcomeMessage = "";
		} else {
			tempZone.ShowWelcomeMsg = true;
			tempZone.WelcomeMessage = WelcomeMessage;
		}
		if (WarningMessage == ""){
			tempZone.ShowWarningMsg = false;
			tempZone.WarningMessage = WarningMessage;
		} else {
			tempZone.ShowWarningMsg = true;
			tempZone.WarningMessage = WarningMessage;
		}
		tempZone.OverrideSafeZone = OverrideSafeZone;
		tempZone.GodModPlayers = GodModPlayers;
		tempZone.PreventWeaponRaise = PreventWeaponRaise;
		tempZone.PreventActions = PreventActions;
		tempZone.BlockTrader = PreventTrade;
		tempZone.KillAggressors = KillAggressors;
		float DamagePerTickMin = 28;
		float DamagePerTickRand = 35;
		float GunTickMulitplier = 2.0;
		float HitChance = 1;
		float CanBeKilled = false;
		float RequireLineOfSight = false;
		//5 OP can't be killed Can Shoot Throw Objects, and 100% HitChance, 
		//4 Require Line Of Sight 92% HitChance can't be killed, medium high fire rate 
		//3 Require Line Of Sight 90% HitChance can be killed, high fire rate,
		//2 Require Line Of Sight 85% HitChance can be killed, medium high fire rate  
		//1 Require Line Of Sight 80% HitChance can be killed, low high fire rate 
		if (GuardDifficulty == 5){
			GunTickMulitplier = 2.0;
			HitChance = 1.0;
			CanBeKilled = false;
			RequireLineOfSight = true;
		} else if (GuardDifficulty == 4){
			GunTickMulitplier = 2.0;
			HitChance = 0.92;
			CanBeKilled = false;
			RequireLineOfSight = true;
		} else if (GuardDifficulty == 3){
			GunTickMulitplier = 3.0;
			HitChance = 0.90;
			CanBeKilled = true;
			RequireLineOfSight = true;
		} else if (GuardDifficulty == 2){
			GunTickMulitplier = 2.0;
			HitChance = 0.85;
			CanBeKilled = true;
			RequireLineOfSight = true;
		} else if (GuardDifficulty == 1){
			GunTickMulitplier = 1.0;
			HitChance = 0.80;
			CanBeKilled = true;
			RequireLineOfSight = true;
		}
		for (int i = 0; i < simpZone.Guards.Count(); i++){
			ref habGuard tmpGuard = new ref habGuard(simpZone.Guards.Get(i).X, simpZone.Guards.Get(i).Y, simpZone.Guards.Get(i).Z,simpZone.Guards.Get(i).Orientation,simpZone.Guards.Get(i).Skin);
			string WeaponInHands = simpZone.Guards.Get(i).WeaponInHands;
			habConverter().init();
			string GunSound =  habConverter().GunToSound.Get(WeaponInHands);
			string Mag =  habConverter().GunToMag.Get(WeaponInHands);
			float Dmg =  habConverter().GunToDmg.Get(WeaponInHands);
			TStringArray WeaponInHandsAttachments =  simpZone.Guards.Get(i).WeaponInHandsAttachments;
			habPrint("WeaponInHands: " + WeaponInHands + " GunSound: " + GunSound + " Mag: " + Mag + " Dmg: " + Dmg, "Verbose");
			if ( GunSound && Mag && Dmg ) {
				tmpGuard.WeaponInHands = WeaponInHands;
				tmpGuard.WeaponInHandsMag = Mag;
				tmpGuard.WeaponInHandsAttachments = WeaponInHandsAttachments;
				tmpGuard.GunSound = GunSound;
				tmpGuard.DamagePerTickMin = Dmg;
				tmpGuard.DamagePerTickRand = Dmg * 1.2;
			} else {
				tmpGuard.WeaponInHands = "M4A1";
				tmpGuard.WeaponInHandsMag = "Mag_STANAGCoupled_30Rnd";
				tmpGuard.WeaponInHandsAttachments = {"M4_RISHndgrd", "M4_OEBttstck", "M68Optic"};
				tmpGuard.GunSound = "M4_Shot_SoundSet";
				tmpGuard.DamagePerTickMin = 18;
				tmpGuard.DamagePerTickRand = 21.6;
			}
			tmpGuard.GunTickMulitplier = GunTickMulitplier;
			tmpGuard.HitChance = HitChance;
			tmpGuard.CanBeKilled = CanBeKilled;
			tmpGuard.RequireLineOfSight = RequireLineOfSight;
			tmpGuard.RespawnTimer = RespawnTimer;
			tempZone.Guards.Insert(tmpGuard);		
		}
		
		return tempZone;
	}
		
	
	static ref HeroesAndBanditsConfigLevels ConvertLevels(ref HeroesAndBanditsSimpleConfig simpConfig){
		ref HeroesAndBanditsConfigLevels tempSettings = new ref HeroesAndBanditsConfigLevels();
		tempSettings.addAffinity("hero", "#HAB_HERO", "HeroesAndBandits/gui/images/Hero.paa");
		tempSettings.addAffinity("bandit", "#HAB_BANDIT", "HeroesAndBandits/gui/images/Bandit.paa");
		
		ref TStringArray banditOnlyItems = {};
		ref TStringArray heroOnlyItems = {};
		ref TStringArray combinedItems = {};
		bool BanditCanRemoveMask = simpConfig.BanditCanRemoveMask;
		bool BanditCanRemoveArmBand = simpConfig.BanditCanRemoveArmBand;
		ref TStringArray BanditMasks = simpConfig.BanditMasks;
		ref TStringArray BanditArmBands = simpConfig.BanditArmBands;
		bool HeroCanRemoveMask = simpConfig.HeroCanRemoveMask;
		bool HeroCanRemoveArmBand = simpConfig.HeroCanRemoveArmBand;
		ref TStringArray HeroMasks = simpConfig.HeroMasks;
		ref TStringArray HeroArmBands = simpConfig.HeroArmBands;
		if (!BanditCanRemoveMask){
			banditOnlyItems.InsertAll(BanditMasks);
			combinedItems.InsertAll(BanditMasks);
		}
		if (!BanditCanRemoveArmBand){
			banditOnlyItems.InsertAll(BanditArmBands);
			combinedItems.InsertAll(BanditArmBands);
		}
		if (!HeroCanRemoveMask){
			heroOnlyItems.InsertAll(HeroMasks);
			combinedItems.InsertAll(HeroMasks);
		
		}
		if (!HeroCanRemoveArmBand){
			heroOnlyItems.InsertAll(HeroArmBands);
			combinedItems.InsertAll(HeroArmBands);
		
		}
		
		if (simpConfig.HeroOnlyItems.Count() > 0){
			heroOnlyItems.InsertAll(simpConfig.HeroOnlyItems);
			combinedItems.InsertAll(simpConfig.HeroOnlyItems);
		
		}
		if (simpConfig.BanditOnlyItems.Count() > 0){
			heroOnlyItems.InsertAll(simpConfig.BanditOnlyItems);
			combinedItems.InsertAll(simpConfig.BanditOnlyItems);
		
		}
		if (banditOnlyItems.Count() > 0){
			tempSettings.getAffinity("hero").addItemBlackList(-1, -1, "all", banditOnlyItems );
		}
		if (heroOnlyItems.Count() > 0){
			tempSettings.getAffinity("bandit").addItemBlackList(-1, -1, "all", heroOnlyItems );
		}
		if (combinedItems.Count() > 0){
			tempSettings.DefaultAffinity.addItemBlackList(-1, -1, "all", combinedItems );
		}
		float MaxHumanity; 
		float MinHumanity;
		float TempHumanity;
		string Affinity;
		string Image;
		string ImagePath = "HeroesAndBandits/gui/images/";
		string ImageHeroFile = "HeroNotificationlv";
		string ImageBanditFile = "BanditNotificationlv";
		string ImageFileExt = ".paa";
		int lastIndex = simpConfig.Levels.Count() - 1;
		int count = lastIndex / 2;
		if (count > 5){
			habPrint("[SIMPLE CONFIG ERROR] Too many levels added you must use advanced configs Level Images will not be correct","Always");
			count = 5;
		}
		for (int i = 0; i < simpConfig.Levels.Count(); i++){
			bool isNotDefault = true;
			if (i == 0){
				MaxHumanity = -1;
				MinHumanity = simpConfig.Levels.Get(i).Humanity;
				TempHumanity = simpConfig.Levels.Get(i).Humanity;
				Affinity = "hero";
				Image = ImagePath + ImageHeroFile + count + ImageFileExt;
				count--;
			} else if (i == lastIndex){
				MinHumanity = 0 - simpConfig.Levels.Get(i).Humanity;
				MaxHumanity = -1;
				Affinity = "bandit";
				Image = ImagePath + ImageHeroFile + count + ImageFileExt;
				count++;
			} else if (simpConfig.Levels.Get(i).Humanity == 0){
				MinHumanity = simpConfig.Levels.Get(i+1).Humanity;
				MaxHumanity = TempHumanity;
				isNotDefault = false;
				Affinity = "bambi";
				count++;
			} else if (simpConfig.Levels.Get(i).Humanity > 0){
				MaxHumanity = simpConfig.Levels.Get(i).Humanity;
				MinHumanity = TempHumanity;
				TempHumanity = simpConfig.Levels.Get(i).Humanity;
				Affinity = "hero";
				Image = ImagePath + ImageHeroFile + count + ImageFileExt;
				count--;
			} else if (simpConfig.Levels.Get(i).Humanity < 0){
				MinHumanity = 0 - simpConfig.Levels.Get(i).Humanity;
				MaxHumanity = 0 - simpConfig.Levels.Get(i+1).Humanity;
				Affinity = "bandit";
				Image = ImagePath + ImageHeroFile + count + ImageFileExt;
				count++;
			}
			if (isNotDefault){
				tempSettings.addLevel(simpConfig.Levels.Get(i).Name, Affinity, Image, MinHumanity, MaxHumanity);
			} else {
				tempSettings.DefaultLevel.Name = simpConfig.Levels.Get(i).Name;
				tempSettings.DefaultLevel.MinPoints = MinHumanity;
				tempSettings.DefaultLevel.MaxPoints = MaxHumanity;
			}
		}
		return tempSettings;
	}
	
	static ref HeroesAndBanditsConfigActions ConvertActions(ref HeroesAndBanditsSimpleConfig simpConfig){
		ref HeroesAndBanditsConfigActions tempSettings = new ref HeroesAndBanditsConfigActions();
		for (int i = 0; i < simpConfig.Actions.Count(); i++){
			string Name = simpConfig.Actions.Get(i).Name;
			string Affinity = "none"; //bandit / hero / none
			string SecondaryAffinity = "none";
			float Points = 0;
			bool NotifiyPlayer; 
			if (simpConfig.Actions.Get(i).Humanity < 0){
				Affinity = "bandit";
				Points = 0 - simpConfig.Actions.Get(i).Humanity;
				NotifiyPlayer = true;
			} else if (simpConfig.Actions.Get(i).Humanity > 0){
				Affinity = "hero";
				Points = simpConfig.Actions.Get(i).Humanity;
				NotifiyPlayer = true;
			}  else {
				NotifiyPlayer = false;
			} 
			tempSettings.addAction(Name, Affinity, SecondaryAffinity, Points, NotifiyPlayer);
		}
		
		return tempSettings;
	}

}