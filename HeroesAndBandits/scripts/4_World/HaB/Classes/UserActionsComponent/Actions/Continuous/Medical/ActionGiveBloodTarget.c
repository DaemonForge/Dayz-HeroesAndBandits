modded class ActionGiveBloodTarget: ActionContinuousBase
{
	override void OnFinishProgressServer( ActionData action_data )
	{	

		super.OnFinishProgressServer(action_data);
		
		ActionGiveBloodData action_data_b = ActionGiveBloodData.Cast( action_data );
		
		PlayerBase player_target = PlayerBase.Cast(action_data_b.m_Target.GetObject());
		int bloodtypetarget = player_target.GetStatBloodType().Get();
		bool bloodmatch = BloodTypes.MatchBloodCompatibility(action_data_b.m_ItemBloodType, bloodtypetarget);
		
		PlayerBase sourcePlayer = PlayerBase.Cast(action_data.m_Player);
		if ( bloodmatch )
		{
			sourcePlayer.NewHABAction("MedicGiveBlood",EntityAI.Cast(action_data.m_Target.GetObject()));
		} else
		{ //TODO
			sourcePlayer.NewHABAction("MedicGiveBadBlood",EntityAI.Cast(action_data.m_Target.GetObject()));
		}
	}
}