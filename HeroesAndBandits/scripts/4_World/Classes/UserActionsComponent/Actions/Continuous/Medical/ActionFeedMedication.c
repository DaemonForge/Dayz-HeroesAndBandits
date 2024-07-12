modded class ActionFeedTetracyclineAntibiotics: ActionForceConsume
{
	override void OnFinishProgressServer( ActionData action_data )
	{	
		super.OnFinishProgressServer(action_data);
		PlayerBase sourcePlayer = PlayerBase.Cast(action_data.m_Player);
		sourcePlayer.NewHABAction("medicfeedtetracycline",EntityAI.Cast(action_data.m_Target.GetObject()));
	}
};

modded class ActionFeedPainkillerTablets: ActionForceConsume
{
	override void OnFinishProgressServer( ActionData action_data )
	{	
		super.OnFinishProgressServer(action_data);
		PlayerBase sourcePlayer = PlayerBase.Cast(action_data.m_Player);
		sourcePlayer.NewHABAction("medicfeedpainkiller",EntityAI.Cast(action_data.m_Target.GetObject()));
	}
};

  
modded class ActionFeedCharcoalTablets: ActionForceConsume
{
	override void OnFinishProgressServer( ActionData action_data )
	{	
		super.OnFinishProgressServer(action_data);
		PlayerBase sourcePlayer = PlayerBase.Cast(action_data.m_Player);
		sourcePlayer.NewHABAction("medicfeedcharcoal",EntityAI.Cast(action_data.m_Target.GetObject()));
	}
};

modded class ActionFeedVitaminBottle: ActionForceConsume
{	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		super.OnFinishProgressServer(action_data);
		PlayerBase sourcePlayer = PlayerBase.Cast(action_data.m_Player);
		sourcePlayer.NewHABAction("medicfeedvitamin",EntityAI.Cast(action_data.m_Target.GetObject()));
	}
	
};