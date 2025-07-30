modded class WeaponManager
{
	override void Fire(Weapon_Base wpn)
	{
		PlayerBase sourcePlayer = PlayerBase.Cast( m_player );
		int mi = wpn.GetCurrentMuzzle();
		if ( GetGame().IsServer() ) {
			if(!wpn.IsChamberFiredOut(mi) && !wpn.IsJammed() && !wpn.IsChamberEmpty(mi) && sourcePlayer && sourcePlayer.GetIdentity()){
				//GunFired TODO
			}
		}
		super.Fire(wpn);
	}
}