/**
 * File: ActionBuildPart.c
 * Description: Override build part action to check HAB build block rules
 */

modded class ActionBuildPart
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!super.ActionCondition(player, target, item))
			return false;
		
		// Check HAB build block rules on the target object being built
		Object targetObj = target.GetObject();
		if (targetObj && m_HaBGeneralConfig)
		{
			if (m_HaBGeneralConfig.IsBuildBlocked(targetObj.GetType(), player.Humanity()))
			{
				string reason = m_HaBGeneralConfig.GetBuildBlockReason(targetObj.GetType(), player.Humanity());
				if (reason != "")
				{
					// Notify on server (sends to client)
					if (g_Game.IsServer() && player.GetIdentity())
					{
						HABBlockNotify.SendIfAllowed("Build Restriction", reason, player.GetIdentity(), HABBlockType.Build);
					}
					// Also notify on client directly for immediate feedback
					else if (!g_Game.IsDedicatedServer())
					{
						HABBlockNotify.SendIfAllowedClient("Build Restriction", reason, HABBlockType.Build);
					}
				}
				return false;
			}
		}
		
		return true;
	}
}

modded class ActionBuildShelter
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!super.ActionCondition(player, target, item))
			return false;
		
		// Check HAB build block rules on the target object being built
		Object targetObj = target.GetObject();
		if (targetObj && m_HaBGeneralConfig)
		{
			if (m_HaBGeneralConfig.IsBuildBlocked(targetObj.GetType(), player.Humanity()))
			{
				string reason = m_HaBGeneralConfig.GetBuildBlockReason(targetObj.GetType(), player.Humanity());
				if (reason != "")
				{
					// Notify on server (sends to client)
					if (g_Game.IsServer() && player.GetIdentity())
					{
						HABBlockNotify.SendIfAllowed("Build Restriction", reason, player.GetIdentity(), HABBlockType.Build);
					}
					// Also notify on client directly for immediate feedback
					else if (!g_Game.IsDedicatedServer())
					{
						HABBlockNotify.SendIfAllowedClient("Build Restriction", reason, HABBlockType.Build);
					}
				}
				return false;
			}
		}
		
		return true;
	}
}
