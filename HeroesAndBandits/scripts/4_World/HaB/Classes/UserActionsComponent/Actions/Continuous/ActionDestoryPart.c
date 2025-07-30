modded class ActionDestroyPart: ActionContinuousBase
{
	override void OnFinishProgressServer( ActionData action_data )
	{	
		super.OnFinishProgressServer(action_data);
		PlayerBase sourcePlayer = PlayerBase.Cast(action_data.m_Player);
		sourcePlayer.NewHABAction("PartRaid", EntityAI.Cast(action_data.m_Target.GetObject()));
	}
};
