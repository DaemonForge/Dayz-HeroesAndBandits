modded class ActionBandageTarget: ActionBandageBase
{
	override void OnFinishProgressServer( ActionData action_data )
	{	
		super.OnFinishProgressServer(action_data);
		PlayerBase sourcePlayer = PlayerBase.Cast(action_data.m_Player);
		sourcePlayer.NewHABAction("MedicBandagePlayer", EntityAI.Cast(action_data.m_Target.GetObject()));
		PlayerBase targetPlayer = PlayerBase.Cast(action_data.m_Target.GetObject());
		if (targetPlayer){
			if (targetPlayer.GetBleedingManagerServer().GetBleedingSourcesCount() == 0){
				targetPlayer.habResetBleedingSource();
			}
		}
	}
};
