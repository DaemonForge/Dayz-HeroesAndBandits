class habConverter
{
	ref static map<string, string> GunToSound = new ref map<string, string>;
	ref static map<string, string> GunToMag= new ref map<string, string>;
	ref static map<string, float> GunToDmg = new ref map<string, float>;
	ref static map<string, TStringArray> GunToAttachments = new ref map<string, TStringArray>;
	
	void habConverter(){
		GunToSound.Insert("M4A1", "M4_Shot_SoundSet");
		GunToMag.Insert("M4A1", "Mag_STANAGCoupled_30Rnd");
		GunToDmg.Insert("M4A1", 24);
		GunToAttachments.Insert("M4A1", {"M4_RISHndgrd", "M4_OEBttstck", "M68Optic"});
		
		GunToSound.Insert("M4A1_Green", "M4_Shot_SoundSet");
		GunToMag.Insert("M4A1_Green", "Mag_STANAGCoupled_30Rnd");
		GunToDmg.Insert("M4A1_Green", 24);
		GunToAttachments.Insert("M4A1_Green", {"M4_RISHndgrd", "M4_OEBttstck", "M68Optic"});
		
		GunToSound.Insert("M4A1_Black", "M4_Shot_SoundSet");
		GunToMag.Insert("M4A1_Black", "Mag_STANAGCoupled_30Rnd");
		GunToDmg.Insert("M4A1_Black", 24);
		GunToAttachments.Insert("M4A1_Black", {"M4_RISHndgrd", "M4_OEBttstck", "M68Optic"});
		
		GunToSound.Insert("AK74", "AK_Shot_SoundSet");
		GunToMag.Insert("AK74", "Mag_AK74_30Rnd");
		GunToDmg.Insert("AK74", 28);
		GunToAttachments.Insert("AK74", {"AK74_Hndgrd", "AK74_WoodBttstck"});
		
		GunToSound.Insert("AK74_Black", "AK_Shot_SoundSet");
		GunToMag.Insert("AK74_Black", "Mag_AK74_30Rnd_Black");
		GunToDmg.Insert("AK74_Black", 28);
		GunToAttachments.Insert("AK74_Black", {"AK74_Hndgrd", "AK74_WoodBttstck"});
		
		GunToSound.Insert("AK74_Green", "AK_Shot_SoundSet");
		GunToMag.Insert("AK74_Green", "Mag_AK74_30Rnd_Green");
		GunToDmg.Insert("AK74_Green", 28);
		GunToAttachments.Insert("AK74_Green", {"AK74_Hndgrd", "AK74_WoodBttstck"});
		
		GunToSound.Insert("UMP45", "UMP45_Shot_SoundSet");
		GunToMag.Insert("UMP45", "Mag_UMP_25Rnd");
		GunToDmg.Insert("UMP45", 16);
		GunToAttachments.Insert("UMP45", {"ReflexOptic"});
		
		GunToSound.Insert("SKS", "SKS_Shot_SoundSet");
		GunToMag.Insert("SKS", "Mag_CLIP762x39_10Rnd");
		GunToDmg.Insert("SKS", 65);
		GunToAttachments.Insert("SKS", {"PUScopeOptic"});
		
		GunToSound.Insert("SKS_Black", "SKS_Shot_SoundSet");
		GunToMag.Insert("SKS_Black", "Mag_CLIP762x39_10Rnd");
		GunToDmg.Insert("SKS_Black", 65);
		GunToAttachments.Insert("SKS_Black", {"PUScopeOptic"});
		
		GunToSound.Insert("SKS_Black", "SKS_Shot_SoundSet");
		GunToMag.Insert("SKS_Black", "Mag_CLIP762x39_10Rnd");
		GunToDmg.Insert("SKS_Black", 65);
		GunToAttachments.Insert("SKS_Black", {"PUScopeOptic"});
		
		GunToSound.Insert("SVD", "SVD_Shot_SoundSet");
		GunToMag.Insert("SVD", "");
		GunToDmg.Insert("SVD", 80);
		GunToAttachments.Insert("SVD", {"KobraOptic"});
		
		GunToSound.Insert("FAL", "FNFAL_Shot_SoundSet");
		GunToMag.Insert("FAL", "Mag_FAL_20Rnd");
		GunToDmg.Insert("FAL", 80);
		GunToAttachments.Insert("FAL", {"M68Optic"});
		
		GunToSound.Insert("FAL", "FNFAL_Shot_SoundSet");
		GunToMag.Insert("FAL", "Mag_FAL_20Rnd");
		GunToDmg.Insert("FAL", 80);
		GunToAttachments.Insert("FAL", {"M68Optic"});
		
		GunToSound.Insert("Expansion_AWM", "SVD_Shot_SoundSet");
		GunToMag.Insert("Expansion_AWM", "Mag_Expansion_AWM_5rnd");
		GunToDmg.Insert("Expansion_AWM", 80);
		GunToAttachments.Insert("Expansion_AWM", {"M68Optic"});
		
		GunToSound.Insert("Expansion_G36", "M4_Shot_SoundSet");
		GunToMag.Insert("Expansion_G36", "Mag_STANAG_30Rnd");
		GunToDmg.Insert("Expansion_G36", 25);
		GunToAttachments.Insert("Expansion_G36", {"ACOGOptic"});
		
		GunToSound.Insert("ussr_m200", "m200_silencerHomeMade_SoundSet");
		GunToMag.Insert("ussr_m200", "Mag_ussrm200_7Rnd");
		GunToDmg.Insert("ussr_m200", 100);
		GunToAttachments.Insert("ussr_m200", {"M200Optic", "M200Suppressor"});
		
		GunToSound.Insert("ussr_m200_black", "m200_Shot_SoundSet");
		GunToMag.Insert("ussr_m200_black", "Mag_ussrm200_7Rnd");
		GunToDmg.Insert("ussr_m200_black", 100);
		GunToAttachments.Insert("ussr_m200_black", {"M200Optic"});
	}
	
	static ref HeroesAndBanditsSettings ConvertSettings(ref HeroesAndBanditsSimpleConfig simpConfig){
		HeroesAndBanditsSettings tempSettings = new ref HeroesAndBanditsSettings();
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
	
	static ref HeroesAndBanditsConfigZones ConvertZonesConfig(ref HeroesAndBanditsSimpleConfig simpConfig){
		HeroesAndBanditsConfigZones tempSettings = new ref HeroesAndBanditsConfigZones();

		
		return tempSettings;
	}
	
	
	static ref habZone ConvertZone(ref HABSimpleZones simpZone){
		habZone tempZone;
		
		return tempZone;
	}
	
	static ref habGuard ConvertGuard(ref habSimpleGuard simpGuard){
		habGuard tempGuard;
		
		return tempGuard;
	}
	
	
	static ref HeroesAndBanditsConfigLevels ConvertLevelsConfig(ref habSimpleGuard simpGuard){
		HeroesAndBanditsConfigLevels tempSettings = new ref HeroesAndBanditsConfigLevels();
		
		return tempSettings;
	}
	
	static ref HeroesAndBanditsConfigActions ConvertActions(ref HeroesAndBanditsSimpleConfig simpConfig){
		HeroesAndBanditsConfigActions tempSettings = new ref HeroesAndBanditsConfigActions();
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