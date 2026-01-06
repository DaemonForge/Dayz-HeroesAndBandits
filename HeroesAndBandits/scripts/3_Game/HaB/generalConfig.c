/**
 * File: HaBGeneralConfig.c
 * Description: The general config for Heroes and Bandits
 * 
 * Block Rules System:
 * The block rules allow condition-based blocking of items, recipes, weapons, and building.
 * 
 * Condition Format:
 *   "<VALUE" or ">VALUE" where VALUE can be:
 *   - Plain number (e.g., 1000): Compares against ABSOLUTE humanity (affects both heroes and bandits)
 *   - + prefix (e.g., +1000): Compares against positive humanity (hero-specific)
 *   - - prefix (e.g., -1000): Compares against negative humanity (bandit-specific)
 *   
 * Examples:
 *   - "<1000"   = Blocked if |humanity| < 1000 (Bambis can't use, requires Hero 1 OR Bandit 1)
 *   - ">+999"   = Blocked if humanity > 999 (Heroes can't use)
 *   - "<-999"   = Blocked if humanity < -999 (Bandits can't use)
 *   - "<+1000"  = Blocked if humanity < +1000 (only Heroes with 1000+ humanity can use)
 *   - ">-1000"  = Blocked if humanity > -1000 (only Bandits with -1000 or less can use)
 *   
 * Block Types (Type field):
 *   - 0 (Equip): Blocks equipping/wearing the item
 *   - 1 (Recipe): Blocks crafting the recipe
 *   - 2 (Weapon): Blocks firing the weapon
 *   - 3 (Build): Blocks placing/building the item
 */
static autoptr UDBGlobalHandler<HaBGeneralConfig> m_HaBGeneralConfigHandler = new UDBGlobalHandler<HaBGeneralConfig>("HAB_GENERAL");
static autoptr HaBGeneralConfig m_HaBGeneralConfig;
class HaBGeneralConfig extends Managed
{
	string ConfigVersion = "1";
	string BanditRole = "DISCORDROLEID";
	string HeroRole = "DISCORDROLEID";
	string PathHeroRole = "DISCORDROLEID";
	string PathBanditRole = "DISCORDROLEID";
	ref TIntArray Levels = {1000,2500,5000,10000,20000,40000,80000,200000,500000,1000000};
	ref TStringArray FlexActions = {"zombiekill", "huntanimal", "catchfish", "mineore" };
	
	// Leaderboard Settings
	// Array of player GUIDs (Bohemia Interactive ID from GetIdentity().GetId()) to exclude from leaderboards
	ref TStringArray LeaderboardBlacklist = {"ABC123def456GHI789jkl012MNO345pqr678STU901"};
	
	// UniversalComms Settings
	bool UCRequireRadio = false; // If true, players must have a working radio to use Hero/Bandit chat channels
	
	// Block Rules (condition-based blocking by humanity value)
	// Type: 0=Equip, 1=Recipe, 2=Weapon, 3=Build
	// Condition: "<1000" (bambis blocked), ">+999" (heroes blocked), "<-999" (bandits blocked)
	ref array<ref HABBlockRule> BlockRules;
    
    void HaBGeneralConfig()
    {
        BlockRules = new array<ref HABBlockRule>;
        
        // Default block rules
        // Condition shortcuts:
        //   "<1000"  = Requires Hero OR Bandit (|humanity| >= 1000)
        //   ">-1000" = Bandit only (humanity <= -1000)
        //   "<+1000" = Hero only (humanity >= +1000)
        //   "<-999"  = Not for Bandits (blocks humanity < -999)
        
        // Bandit-only gear (blocks bambis and heroes)
        AddDefaultRule(HABBlockType.Equip, "Shemagh_Bandit_ColorBase", ">-1000", "Bandit gear only");
        AddDefaultRule(HABBlockType.Equip, "Shemagh_Facemask_ColorBase", ">-1000", "Bandit gear only");
        
        // Hero-only gear (blocks bambis and bandits)
        AddDefaultRule(HABBlockType.Equip, "Shemagh_Scarf_ColorBase", "<+1000", "Hero gear only");
        
        // Recipes requiring any affinity (blocks bambis)
        AddDefaultRule(HABBlockType.Recipe, "CraftWoodenCrate", "<1000", "Requires Hero or Bandit status");
        AddDefaultRule(HABBlockType.Recipe, "CraftWatchtowerKit", "<1000", "Requires Hero or Bandit status");
        
        // Bandit-only recipes (blocks bambis and heroes)
        AddDefaultRule(HABBlockType.Recipe, "MakePipeBomb", ">-1000", "Bandits only");
        AddDefaultRule(HABBlockType.Recipe, "MakePipeBombShell", ">-1000", "Bandits only");
        AddDefaultRule(HABBlockType.Recipe, "MakeBBFuse", ">-1000", "Bandits only");
        
        // Blocked for bandits only (bambis and heroes can craft)
        AddDefaultRule(HABBlockType.Recipe, "CraftBloodBagIV", "<-999", "Bandits cannot craft medical supplies");
    }
    
    private void AddDefaultRule(HABBlockType type, string target, string condition, string message = "")
    {
        HABBlockRule rule = new HABBlockRule();
        rule.Type = type;
        rule.Target = target;
        rule.Condition = condition;
        rule.BlockMessage = message;
        BlockRules.Insert(rule);
    }

    void ~HaBGeneralConfig()
    {
        if (BlockRules)
            BlockRules.Clear();
    }
	
	// Check if an item is blocked for the given humanity
	bool IsItemBlocked(string className, float humanity)
	{
		if (!BlockRules) return false;
		foreach (HABBlockRule rule : BlockRules)
		{
			if (rule.Type == HABBlockType.Equip && rule.IsBlocked(humanity))
			{
				if (IsClassMatch(className, rule.Target))
					return true;
			}
		}
		return false;
	}
	
	// Check if a recipe is blocked for the given humanity
	bool IsRecipeBlocked(string recipeName, float humanity)
	{
		if (!BlockRules) return false;
		foreach (HABBlockRule rule : BlockRules)
		{
			if (rule.Type == HABBlockType.Recipe && rule.IsBlocked(humanity))
			{
				// IsClassMatch is case-insensitive
				if (IsClassMatch(recipeName, rule.Target))
					return true;
			}
		}
		return false;
	}
	
	// Check if a weapon is blocked for the given humanity
	bool IsWeaponBlocked(string className, float humanity)
	{
		if (!BlockRules) return false;
		foreach (HABBlockRule rule : BlockRules)
		{
			if (rule.Type == HABBlockType.Weapon && rule.IsBlocked(humanity))
			{
				if (IsClassMatch(className, rule.Target))
					return true;
			}
		}
		return false;
	}
	
	// Check if building/placing is blocked for the given humanity
	bool IsBuildBlocked(string className, float humanity)
	{
		if (!BlockRules) return false;
		foreach (HABBlockRule rule : BlockRules)
		{
			if (rule.Type == HABBlockType.Build && rule.IsBlocked(humanity))
			{
				if (IsClassMatch(className, rule.Target))
					return true;
			}
		}
		return false;
	}
	
	// Get the block reason for an item
	string GetItemBlockReason(string className, float humanity)
	{
		if (!BlockRules) return "";
		foreach (HABBlockRule rule : BlockRules)
		{
			if (rule.Type == HABBlockType.Equip && rule.IsBlocked(humanity))
			{
				if (IsClassMatch(className, rule.Target))
					return rule.GetBlockMessage(humanity);
			}
		}
		return "";
	}
	
	// Get the block reason for a recipe
	string GetRecipeBlockReason(string recipeName, float humanity)
	{
		if (!BlockRules) return "";
		foreach (HABBlockRule rule : BlockRules)
		{
			if (rule.Type == HABBlockType.Recipe && rule.IsBlocked(humanity))
			{
				// IsClassMatch is case-insensitive
				if (IsClassMatch(recipeName, rule.Target))
					return rule.GetBlockMessage(humanity);
			}
		}
		return "";
	}
	
	// Get the block reason for a weapon
	string GetWeaponBlockReason(string className, float humanity)
	{
		if (!BlockRules) return "";
		foreach (HABBlockRule rule : BlockRules)
		{
			if (rule.Type == HABBlockType.Weapon && rule.IsBlocked(humanity))
			{
				if (IsClassMatch(className, rule.Target))
					return rule.GetBlockMessage(humanity);
			}
		}
		return "";
	}
	
	// Get the block reason for building
	string GetBuildBlockReason(string className, float humanity)
	{
		if (!BlockRules) return "";
		foreach (HABBlockRule rule : BlockRules)
		{
			if (rule.Type == HABBlockType.Build && rule.IsBlocked(humanity))
			{
				if (IsClassMatch(className, rule.Target))
					return rule.GetBlockMessage(humanity);
			}
		}
		return "";
	}
	
	// Helper to check if a class name matches a target (supports inheritance)
	// Uses case-insensitive comparison to avoid config typo issues
	// Also checks config.cpp inheritance chain for items defined only in config
	protected bool IsClassMatch(string className, string target)
	{
		// Convert both to lowercase for case-insensitive comparison
		string classNameLower = className;
		string targetLower = target;
		classNameLower.ToLower();
		targetLower.ToLower();
		
		// Direct match
		if (classNameLower == targetLower)
			return true;
		
		// Check config.cpp inheritance using g_Game.IsKindOf
		// This handles items defined in config.cpp (CfgVehicles, CfgWeapons, etc.)
		if (g_Game && g_Game.IsKindOf(className, target))
			return true;
		
		// Fallback: Check script inheritance (for script-defined classes)
		typename targetType = target.ToType();
		typename classType = className.ToType();
		
		if (targetType && classType)
		{
			// Check if classType inherits from targetType
			return classType.IsInherited(targetType);
		}
		
		return false;
	}
}
