/**
 * File: HABBlockRules.c
 * Description: Implementation of the smart block rules system for building placement
 */

modded class ItemBase
{
	// Override CanBePlaced to check HAB build block rules
	override bool CanBePlaced(Man player, vector position)
	{
		if (!super.CanBePlaced(player, position))
			return false;
		
		// Check HAB build block rules
		PlayerBase playerBase;
		if (Class.CastTo(playerBase, player))
		{
			if (m_HaBGeneralConfig && m_HaBGeneralConfig.IsBuildBlocked(GetType(), playerBase.Humanity()))
			{
				// Notify the player with cooldown
				if (g_Game.IsServer() && playerBase.GetIdentity())
				{
					string reason = m_HaBGeneralConfig.GetBuildBlockReason(GetType(), playerBase.Humanity());
					if (reason != "")
					{
						HABBlockNotify.SendIfAllowed("Build Restriction", reason, playerBase.GetIdentity(), HABBlockType.Build);
					}
				}
				return false;
			}
		}
		
		return true;
	}
}
