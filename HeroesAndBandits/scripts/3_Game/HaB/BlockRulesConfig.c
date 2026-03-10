/**
 * File: BlockRulesConfig.c
 * Description: Block rule types and helper classes for Heroes and Bandits smart blocking system
 * The actual block rules are stored in HaBGeneralConfig (HAB_GENERAL config)
 */

// Block rule types
enum HABBlockType
{
	Equip = 0,   // Block equipping/wearing
	Recipe = 1,  // Block crafting
	Weapon = 2,  // Block firing
	Build = 3    // Block placing/building
}

// Notification cooldown helper to prevent spam
class HABBlockNotify
{
	private static ref map<string, float> s_LastNotifyTime;
	private static const float COOLDOWN = 18.0; // seconds
	
	static void Init()
	{
		if (!s_LastNotifyTime)
			s_LastNotifyTime = new map<string, float>();
	}
	
	// Returns true if notification should be shown (cooldown expired)
	static bool CanNotify(string playerId, int blockType)
	{
		Init();
		
		string key = playerId + "_" + blockType.ToString();
		float currentTime = g_Game.GetTickTime();
		
		float lastTime;
		if (s_LastNotifyTime.Find(key, lastTime))
		{
			if (currentTime - lastTime < COOLDOWN)
				return false;
		}
		
		s_LastNotifyTime.Set(key, currentTime);
		return true;
	}
	
	// Send notification with cooldown check (server-side)
	static void SendIfAllowed(string title, string message, PlayerIdentity identity, int blockType)
	{
		if (!identity)
			return;
		
		if (CanNotify(identity.GetId(), blockType))
		{
			UUtil.SendNotificationEx(title, message, identity);
		}
	}
	
	// Send notification with cooldown check (client-side)
	// Uses a client-specific key since we don't have player ID on client
	static void SendIfAllowedClient(string title, string message, int blockType)
	{
		Init();
		
		// Use "client" as the player ID since this is local
		string key = "client_" + blockType.ToString();
		float currentTime = g_Game.GetTickTime();
		
		float lastTime;
		if (s_LastNotifyTime.Find(key, lastTime))
		{
			if (currentTime - lastTime < COOLDOWN)
				return;
		}
		
		s_LastNotifyTime.Set(key, currentTime);
		
		// Use UFramework notification system
		UUtil.SendNotificationEx(title, message, NULL, "set:dayz_gui image:tutorial_lockpicking");
	}
}

class HABBlockRule
{
	int Type;           // HABBlockType value
	string Target;      // Class name or recipe name to block
	string Condition;   // Condition string like "<1000", "<+1000", or ">-1000"
	string BlockMessage; // Optional custom message to show when blocked
	
	void HABBlockRule()
	{
		Type = HABBlockType.Equip;
		Target = "";
		Condition = "";
		BlockMessage = "";
	}
	
	// Check if this rule blocks for the given humanity value
	bool IsBlocked(float humanity)
	{
		return HABBlockRuleHelper.EvaluateCondition(Condition, humanity);
	}
	
	// Get the block message (custom or auto-generated)
	string GetBlockMessage(float humanity)
	{
		if (BlockMessage != "")
			return BlockMessage;
		return HABBlockRuleHelper.GetBlockReason(Condition);
	}
}

class HABBlockRuleHelper
{
	// Parse and evaluate a condition string against a humanity value
	// Returns true if the condition means BLOCKED
	static bool EvaluateCondition(string condition, float humanity)
	{
		if (!condition || condition.Length() < 2)
			return false;
		
		string trimmed = condition;
		trimmed.Trim();
		
		if (trimmed.Length() < 2)
			return false;
		
		// Get the operator (< or >)
		string op = trimmed.Substring(0, 1);
		string valueStr = trimmed.Substring(1, trimmed.Length() - 1);
		
		// Check if this is a signed comparison (+ or - prefix on the number)
		bool isSignedComparison = false;
		bool isPositiveCheck = false;
		bool isNegativeCheck = false;
		
		if (valueStr.Length() > 0)
		{
			string firstChar = valueStr.Substring(0, 1);
			if (firstChar == "+")
			{
				isSignedComparison = true;
				isPositiveCheck = true;
				valueStr = valueStr.Substring(1, valueStr.Length() - 1);
			}
			else if (firstChar == "-")
			{
				isSignedComparison = true;
				isNegativeCheck = true;
				// Keep the minus for parsing
			}
		}
		
		// Parse the threshold value
		float threshold = valueStr.ToFloat();
		
		// Apply the appropriate comparison
		if (isSignedComparison)
		{
			// Signed comparison: compare directly against humanity
			if (op == "<")
			{
				// Blocked if humanity < threshold
				return (humanity < threshold);
			}
			else if (op == ">")
			{
				// Blocked if humanity > threshold
				return (humanity > threshold);
			}
		}
		else
		{
			// Unsigned comparison: compare against absolute value of humanity
			float absHumanity = Math.AbsFloat(humanity);
			
			if (op == "<")
			{
				// Blocked if |humanity| < threshold
				return (absHumanity < threshold);
			}
			else if (op == ">")
			{
				// Blocked if |humanity| > threshold
				return (absHumanity > threshold);
			}
		}
		
		return false;
	}
	
	// Get a human-readable description of why something is blocked
	static string GetBlockReason(string condition)
	{
		if (!condition || condition.Length() < 2)
			return "";
		
		string trimmed = condition;
		trimmed.Trim();
		
		string op = trimmed.Substring(0, 1);
		string valueStr = trimmed.Substring(1, trimmed.Length() - 1);
		
		// Check if this is a signed comparison
		bool isSignedComparison = false;
		bool isPositiveCheck = false;
		bool isNegativeCheck = false;
		string displayValue = valueStr;
		
		if (valueStr.Length() > 0)
		{
			string firstChar = valueStr.Substring(0, 1);
			if (firstChar == "+")
			{
				isSignedComparison = true;
				isPositiveCheck = true;
				displayValue = valueStr.Substring(1, valueStr.Length() - 1);
			}
			else if (firstChar == "-")
			{
				isSignedComparison = true;
				isNegativeCheck = true;
			}
		}
		
		float threshold = displayValue.ToFloat();
		if (isNegativeCheck)
			threshold = valueStr.ToFloat(); // Parse with the negative sign
		
		if (isSignedComparison)
		{
			// Signed comparison messages
			if (op == "<")
			{
				if (isPositiveCheck)
				{
					return "Requires +Humanity (" + displayValue + " or higher)";
				}
				else // isNegativeCheck
				{
					return "Requires Humanity below " + valueStr;
				}
			}
			else if (op == ">")
			{
				if (isNegativeCheck)
				{
					return "Requires -Humanity (" + valueStr + " or lower)";
				}
				else // isPositiveCheck
				{
					return "Blocked for Humanity above +" + displayValue;
				}
			}
		}
		else
		{
			// Unsigned comparison messages (absolute value)
			if (op == "<")
			{
				return "Requires Humanity of " + displayValue + " or higher (Hero or Bandit)";
			}
			else if (op == ">")
			{
				return "Blocked for Humanity above " + displayValue + " (either direction)";
			}
		}
		
		return "";
	}
}
