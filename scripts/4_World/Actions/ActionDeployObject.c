modded class ActionDeployObject
{	
	
	override void OnFinishProgressServer( ActionData action_data )
	{
		PlayerBase sourcePlayer;
		if (action_data.m_MainItem.IsInherited(TrapBase))
		{
			TrapBase trap = TrapBase.Cast(action_data.m_MainItem);
			sourcePlayer = PlayerBase.Cast(action_data.m_Player);
			if ( sourcePlayer && trap)
			{
				trap.habSetActivatedBy(sourcePlayer.GetIdentity().GetPlainId());
			}
		}
		
		#ifdef EXPANSIONMOD
			if (action_data.m_MainItem.IsInherited(ExpansionExplosive))
			{
				ExpansionExplosive explosive = ExpansionExplosive.Cast(action_data.m_MainItem);
				sourcePlayer = PlayerBase.Cast(action_data.m_Player);
				if ( sourcePlayer && explosive)
				{
					habPrint( sourcePlayer.GetIdentity().GetPlainId() + " Placeing C4" , "Debug");
					explosive.habSetActivatedBy(sourcePlayer.GetIdentity().GetPlainId());
				}
			}
		#endif
		super.OnFinishProgressServer(action_data);
	}
	
}