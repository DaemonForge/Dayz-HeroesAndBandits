modded class ActionGiveSalineTarget: ActionContinuousBase
{
	override void OnFinishProgressServer( ActionData action_data )
	{	
		super.OnFinishProgressServer(action_data);
		PlayerBase sourcePlayer = PlayerBase.Cast(action_data.m_Player);
		sourcePlayer.NewHABAction("MedicGiveSaline",action_data.m_Target.GetObject());
	}
};