modded class ActionDestroyCombinationLock: ActionContinuousBase
{
	override void OnFinishProgressServer( ActionData action_data )
	{
		super.OnFinishProgressServer(action_data);
		PlayerBase sourcePlayer = PlayerBase.Cast(action_data.m_Player);
		Fence fence = Fence.Cast( action_data.m_Target.GetObject() );
		if ( fence ) {
			CombinationLock combination_lock = fence.GetCombinationLock();
			if ( combination_lock ) {
				if ( combination_lock.IsDamageDestroyed() ) {
					sourcePlayer.NewHABAction("CombinationLockRaid",action_data.m_Target.GetObject());
				}
			}
		}
	}
};
