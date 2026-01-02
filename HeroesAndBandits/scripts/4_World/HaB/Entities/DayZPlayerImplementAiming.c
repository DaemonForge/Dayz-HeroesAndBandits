// Check weapon block rules when player raises weapon to aim
modded class DayZPlayerImplementAiming
{
	override void OnRaiseBegin(DayZPlayerImplement player)
	{
		super.OnRaiseBegin(player);
		
		// Only check on client side - this is for the warning notification
		if (g_Game.IsDedicatedServer())
			return;
		
		// Get weapon through inventory system (newer DayZ versions)
		Weapon_Base weapon = Weapon_Base.Cast(player.GetHumanInventory().GetEntityInHands());
		if (!weapon)
			return;
		
		PlayerBase sourcePlayer = PlayerBase.Cast(player);
		if (!sourcePlayer)
			return;
		
		// Check if weapon is blocked
		if (m_HaBGeneralConfig && m_HaBGeneralConfig.IsWeaponBlocked(weapon.GetType(), sourcePlayer.Humanity()))
		{
			string reason = m_HaBGeneralConfig.GetWeaponBlockReason(weapon.GetType(), sourcePlayer.Humanity());
			if (reason != "")
			{
				// Use the cooldown system to prevent spam when raising/lowering repeatedly
				HABBlockNotify.SendIfAllowedClient("Weapon Restriction", reason, HABBlockType.Weapon);
			}
		}
	}
}
