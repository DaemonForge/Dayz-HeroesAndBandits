/**
 * File: HaBGeneralConfig.c
 * Description: The general config for Heroes and Bandits
 */
static autoptr UDBGlobalHandler<HaBGeneralConfig> m_HaBGeneralConfigHandler = new UDBGlobalHandler<HaBGeneralConfig>("HAB_GENERAL");
static autoptr HaBGeneralConfig m_HaBGeneralConfig;
class HaBGeneralConfig extends Managed
{
	string ConfigVersion = "0";
	string BanditRole = "DISCORDROLEID";
	string HeroRole = "DISCORDROLEID";
	string PathHeroRole = "DISCORDROLEID";
	string PathBanditRole = "DISCORDROLEID";
	ref TIntArray Levels = {1000,2500,5000,10000,20000,40000,80000,200000,500000,1000000};
	ref TStringArray FlexActions = {"zombiekill", "huntanimal", "catchfish", "mineore" };
	ref TStringArray BambiBlockedItems = {"Shemagh_Bandit_ColorBase", "Shemagh_Scarf_ColorBase", "Shemagh_Facemask_ColorBase"};
	ref TStringArray HeroBlockedItems = {"Shemagh_Bandit_ColorBase", "Shemagh_Facemask_ColorBase"};
	ref TStringArray BanditBlockedItems = {"Shemagh_Scarf_ColorBase"};
    
    void HaBGeneralConfig()
    {
        // Initialization code
    }

    // Destructor: Called when the instance is destroyed (if needed)
    void ~HaBGeneralConfig()
    {
        // Cleanup code (if necessary)
    }

 	
}
