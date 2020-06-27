modded class ActionDeployObject
{	
	
	override void OnFinishProgressServer( ActionData action_data )
	{
		if (action_data.m_MainItem.IsInherited(TrapBase))
		{
			TrapBase trap = TrapBase.Cast(action_data.m_MainItem);
			PlayerBase sourcePlayer = PlayerBase.Cast(action_data.m_Player);
			if ( sourcePlayer && trap)
			{
				trap.habSetActivatedBy(sourcePlayer.GetIdentity().GetPlainId());
			}
		}
		
		#ifdef EXPANSIONMOD
			if (action_data.m_MainItem.IsInherited(ExpansionExplosiveBase))
			{
				ExpansionExplosiveBase explosive = ExpansionExplosiveBase.Cast(action_data.m_MainItem);
				PlayerBase sourcePlayer = PlayerBase.Cast(action_data.m_Player);
				if ( sourcePlayer && explosive)
				{
					explosive.habSetActivatedBy(sourcePlayer.GetIdentity().GetPlainId());
				}
			}
		#endif
		super.OnFinishProgressServer(action_data);
	}
	
}