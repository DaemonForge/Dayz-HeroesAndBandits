modded class ActionDeployObject: ActionContinuousBase
{	
	
	override void OnFinishProgressServer( ActionData action_data )
	{
		PlayerBase sourcePlayer;
		if (action_data.m_MainItem.IsInherited(TrapBase)) {
			TrapBase trap = TrapBase.Cast(action_data.m_MainItem);
			sourcePlayer = PlayerBase.Cast(action_data.m_Player);
			if ( sourcePlayer && trap) {
				trap.habSetActivatedBy(sourcePlayer.GetIdentity().GetId());
			}
		}
		
		#ifdef EXPANSIONMOD
			if (action_data.m_MainItem.IsInherited(ExpansionExplosive)) {
				ExpansionExplosive explosive = ExpansionExplosive.Cast(action_data.m_MainItem);
				sourcePlayer = PlayerBase.Cast(action_data.m_Player);
				if ( sourcePlayer && explosive) {
					explosive.habSetActivatedBy(sourcePlayer.GetIdentity().GetId());
				}
			}
		#endif
		super.OnFinishProgressServer(action_data);
	}
	
}