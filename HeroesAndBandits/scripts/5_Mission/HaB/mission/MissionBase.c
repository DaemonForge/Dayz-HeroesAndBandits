modded class MissionBase extends MissionBaseWorld
{
    override UIScriptedMenu CreateScriptedMenu(int id) {
        UIScriptedMenu menu = NULL;
        menu = super.CreateScriptedMenu(id);
        if (!menu) {
            switch (id) {
                case HAB_PANEL_MENU:
                    menu = new HAB_MainPanel;
                    break;
            }
            if (menu) {
                menu.SetID(id);
            }
        }
        return menu;
    }
	
	
	override void UFrameworkReady(){
		Print("[HAB] UFrameworkReady called - loading configs...");
		// Defer config loading slightly to ensure UFramework endpoints are fully initialized
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.LoadHaBConfigs, 500, false);
		super.UFrameworkReady();
	}
	
	void LoadHaBConfigs(){
		// Guard against UFramework not being ready
		if (!UF()){
			Print("[HAB] [Warn] LoadHaBConfigs - UF() is null, retrying in 1s...");
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.LoadHaBConfigs, 1000, false);
			return;
		}
		
		Print("[HAB] Loading configs from UFramework...");
		m_HaBGeneralConfigHandler.Load(this, "CBLoadHaBGeneralConfig");
		m_HABPanelConfigHandler.Load(this, "CBLoadHABPanelConfig");
		
		// Register Mod Settings wizard with UFramework
		if (GetGame().IsServer())
		{
			RegisterHABModSettings();
		}
	}
	
	void RegisterHABModSettings()
	{
		string html = "";
		string line;
		
		FileHandle file = OpenFile("HeroesAndBandits/data/hab-settings.html", FileMode.READ);
		if (file)
		{
			while (FGets(file, line) >= 0)
			{
				if (html.Length() > 0)
				{
					html = html + "\n";
				}
				html = html + line;
			}
			CloseFile(file);
		}
		
		if (html == "")
		{
			Print("[HAB] [Warn] Could not load hab-settings.html from mod data folder");
			return;
		}
		
		autoptr TStringArray globals = new TStringArray;
		globals.Insert("HAB_GENERAL");
		globals.Insert("HAB_PANEL");
		globals.Insert("HAB_ACTIONS");
		globals.Insert("HAB_ACTIONS_HERO");
		globals.Insert("HAB_ACTIONS_BANDIT");
		globals.Insert("HAB_ACTIONS_BAMBI");
		
		UF().Settings().Register("heroes-and-bandits", "Heroes & Bandits", "DaemonForge", html, globals);
		
		Print("[HAB] Mod Settings wizard registration requested");
	}
	
	
	void CBLoadHABPanelConfig(int cid, int status, string oid, HABPanelConfig data){
		Print("[HAB] Panel config callback - status: " + status.ToString());
		if (status == UF_SUCCESS){
			Class.CastTo(m_HABPanelConfig, data);
			Print("[HAB] Panel config loaded successfully");
		}
		else if (status == UF_EMPTY){
			Print("[HAB] Panel config empty - creating defaults...");
			m_HABPanelConfig = new HABPanelConfig();
			CreateDefaultPanelConfig();
			m_HABPanelConfigHandler.Save(m_HABPanelConfig);
			Print("[HAB] Panel config saved to globals");
		}
		else {
			Print("[HAB] Panel config load failed with status: " + status.ToString());
		}
	}
	
	void CreateDefaultPanelConfig(){
		if (!m_HABPanelConfig) return;
		
		// ========== BAMBI SECTIONS ==========
		autoptr HABPanelSection bambiSec1 = new HABPanelSection();
		bambiSec1.Title = "Welcome";
		bambiSec1.Content.Insert("Welcome to the Heroes and Bandits system!");
		bambiSec1.Content.Insert("You are currently a Bambi - a neutral survivor.");
		bambiSec1.Content.Insert("Every action you take affects your Humanity score.");
		bambiSec1.Content.Insert("Positive actions lead to the Hero path.");
		bambiSec1.Content.Insert("Negative actions lead to the Bandit path.");
		m_HABPanelConfig.Bambi.Sections.Insert(bambiSec1);
		
		autoptr HABPanelSection bambiSec2 = new HABPanelSection();
		bambiSec2.Title = "How It Works";
		bambiSec2.Content.Insert("Your Humanity starts at 0 as a fresh spawn.");
		bambiSec2.Content.Insert("Helping players: +Humanity (bandaging, giving blood, CPR)");
		bambiSec2.Content.Insert("Killing zombies: Small +Humanity gain");
		bambiSec2.Content.Insert("Hunting animals: Small +Humanity gain");
		bambiSec2.Content.Insert("Killing innocent players: Large -Humanity loss");
		bambiSec2.Content.Insert("Killing Heroes: Very large -Humanity loss");
		bambiSec2.Content.Insert("Killing Bandits: +Humanity gain (justice!)");
		m_HABPanelConfig.Bambi.Sections.Insert(bambiSec2);
		
		autoptr HABPanelSection bambiSec3 = new HABPanelSection();
		bambiSec3.Title = "Tips";
		bambiSec3.Content.Insert("Check the Stats tab to see your current Humanity");
		bambiSec3.Content.Insert("The Leaderboards show top Heroes and Bandits");
		bambiSec3.Content.Insert("Some items and recipes are locked by affinity");
		bambiSec3.Content.Insert("Your path affects how other players see you");
		bambiSec3.Content.Insert("Link your Discord for special roles!");
		m_HABPanelConfig.Bambi.Sections.Insert(bambiSec3);
		
		// ========== HERO SECTIONS ==========
		autoptr HABPanelSection heroSec1 = new HABPanelSection();
		heroSec1.Title = "Hero Status";
		heroSec1.Content.Insert("Congratulations, Hero!");
		heroSec1.Content.Insert("You have proven yourself a protector of survivors.");
		heroSec1.Content.Insert("Your positive Humanity reflects your good deeds.");
		heroSec1.Content.Insert("Continue helping others to maintain your status.");
		m_HABPanelConfig.Hero.Sections.Insert(heroSec1);
		
		autoptr HABPanelSection heroSec2 = new HABPanelSection();
		heroSec2.Title = "Hero Perks";
		heroSec2.Content.Insert("Access to Hero-exclusive items and clothing");
		heroSec2.Content.Insert("Hero-only crafting recipes unlocked");
		heroSec2.Content.Insert("Bonus Humanity for medical actions");
		heroSec2.Content.Insert("Recognized as a trusted survivor");
		heroSec2.Content.Insert("Special Hero Discord role (if linked)");
		m_HABPanelConfig.Hero.Sections.Insert(heroSec2);
		
		autoptr HABPanelSection heroSec3 = new HABPanelSection();
		heroSec3.Title = "Hero Actions";
		heroSec3.Content.Insert("Bandage wounded players: +Humanity");
		heroSec3.Content.Insert("Give blood transfusions: +Humanity");
		heroSec3.Content.Insert("Perform CPR: +Humanity");
		heroSec3.Content.Insert("Share food and supplies: Good karma");
		heroSec3.Content.Insert("Defend other players from Bandits: +Humanity");
		heroSec3.Content.Insert("Kill Bandits: +Humanity (bringing justice)");
		m_HABPanelConfig.Hero.Sections.Insert(heroSec3);
		
		autoptr HABPanelSection heroSec4 = new HABPanelSection();
		heroSec4.Title = "Warning";
		heroSec4.Content.Insert("Warning: Killing innocent players will cost you!");
		heroSec4.Content.Insert("Friendly fire causes major Humanity loss.");
		heroSec4.Content.Insert("You can fall back to Bambi or even Bandit status.");
		heroSec4.Content.Insert("Stay true to the Hero path!");
		m_HABPanelConfig.Hero.Sections.Insert(heroSec4);
		
		// ========== BANDIT SECTIONS ==========
		autoptr HABPanelSection banditSec1 = new HABPanelSection();
		banditSec1.Title = "Bandit Status";
		banditSec1.Content.Insert("You have embraced the Bandit path.");
		banditSec1.Content.Insert("Your actions have earned you a fearsome reputation.");
		banditSec1.Content.Insert("Other survivors will be wary of you.");
		banditSec1.Content.Insert("The wasteland is yours to dominate.");
		m_HABPanelConfig.Bandit.Sections.Insert(banditSec1);
		
		autoptr HABPanelSection banditSec2 = new HABPanelSection();
		banditSec2.Title = "Bandit Perks";
		banditSec2.Content.Insert("Access to Bandit-exclusive items and gear");
		banditSec2.Content.Insert("Bandit-only crafting recipes unlocked");
		banditSec2.Content.Insert("Feared by other players");
		banditSec2.Content.Insert("Special Bandit Discord role (if linked)");
		m_HABPanelConfig.Bandit.Sections.Insert(banditSec2);
		
		autoptr HABPanelSection banditSec3 = new HABPanelSection();
		banditSec3.Title = "Consequences";
		banditSec3.Content.Insert("Heroes may hunt you for justice");
		banditSec3.Content.Insert("Some helpful items are restricted");
		banditSec3.Content.Insert("Other Bandits may see you as competition");
		banditSec3.Content.Insert("Your reputation precedes you everywhere");
		m_HABPanelConfig.Bandit.Sections.Insert(banditSec3);
		
		autoptr HABPanelSection banditSec4 = new HABPanelSection();
		banditSec4.Title = "Path to Redemption";
		banditSec4.Content.Insert("Redemption is possible, but difficult.");
		banditSec4.Content.Insert("Help other players to slowly regain Humanity.");
		banditSec4.Content.Insert("Medical actions provide Humanity bonuses.");
		banditSec4.Content.Insert("It takes many good deeds to undo the bad.");
		banditSec4.Content.Insert("The choice is yours: darkness or redemption?");
		m_HABPanelConfig.Bandit.Sections.Insert(banditSec4);
	}
	
	
	void CBLoadHaBGeneralConfig(int cid, int status, string oid, HaBGeneralConfig data){
		if (status == UF_SUCCESS){
			Class.CastTo(m_HaBGeneralConfig,data);
			HEROROLE = m_HaBGeneralConfig.HeroRole;
			BANDITROLE = m_HaBGeneralConfig.BanditRole;
			HEROPATHROLE = m_HaBGeneralConfig.PathHeroRole;
			BANDITPATHROLE = m_HaBGeneralConfig.PathBanditRole;
			HeroesAndBandits.UpdateLevels(m_HaBGeneralConfig.Levels);
		}
		if (status == UF_EMPTY){
			m_HaBGeneralConfig = new HaBGeneralConfig();
			m_HaBGeneralConfigHandler.Save(m_HaBGeneralConfig);
		}
	}
}