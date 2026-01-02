modded class WeaponManager
{
	override void Fire(Weapon_Base wpn)
	{
		PlayerBase sourcePlayer = PlayerBase.Cast( m_player );
		if (!sourcePlayer)
		{
			super.Fire(wpn);
			return;
		}
		
		// Check HAB weapon block rules - must block on BOTH client and server
		// Client blocks the action locally, server blocks authoritatively
		if (m_HaBGeneralConfig && m_HaBGeneralConfig.IsWeaponBlocked(wpn.GetType(), sourcePlayer.Humanity()))
		{
			// Server sends the notification
			if (g_Game.IsServer() && sourcePlayer.GetIdentity())
			{
				string reason = m_HaBGeneralConfig.GetWeaponBlockReason(wpn.GetType(), sourcePlayer.Humanity());
				if (reason != "")
				{
					HABBlockNotify.SendIfAllowed("Weapon Restriction", reason, sourcePlayer.GetIdentity(), HABBlockType.Weapon);
				}
			}
			return; // Block the fire action on both client and server
		}
		
		super.Fire(wpn);
	}
}