modded class ActionFeedTetracyclineAntibiotics: ActionForceConsume
{
	override void OnFinishProgressServer( ActionData action_data )
	{	
		super.OnFinishProgressServer(action_data);
		PlayerBase sourcePlayer = PlayerBase.Cast(action_data.m_Player);
		sourcePlayer.NewHABAction("MedicFeedTetracycline",EntityAI.Cast(action_data.m_Target.GetObject()));
	}
};

modded class ActionFeedPainkillerTablets: ActionForceConsume
{
	override void OnFinishProgressServer( ActionData action_data )
	{	
		super.OnFinishProgressServer(action_data);
		PlayerBase sourcePlayer = PlayerBase.Cast(action_data.m_Player);
		sourcePlayer.NewHABAction("MedicFeedPainkiller",EntityAI.Cast(action_data.m_Target.GetObject()));
	}
};

  
modded class ActionFeedCharcoalTablets: ActionForceConsume
{
	override void OnFinishProgressServer( ActionData action_data )
	{	
		super.OnFinishProgressServer(action_data);
		PlayerBase sourcePlayer = PlayerBase.Cast(action_data.m_Player);
		sourcePlayer.NewHABAction("MedicFeedCharcoal",EntityAI.Cast(action_data.m_Target.GetObject()));
	}
};

modded class ActionFeedVitaminBottle: ActionForceConsume
{	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		super.OnFinishProgressServer(action_data);
		PlayerBase sourcePlayer = PlayerBase.Cast(action_data.m_Player);
		sourcePlayer.NewHABAction("MedicFeedVitamin",EntityAI.Cast(action_data.m_Target.GetObject()));
	}
	
};