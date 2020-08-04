modded class ActionGiveBloodTarget: ActionContinuousBase
{
	override void OnFinishProgressServer( ActionData action_data )
	{	

		super.OnFinishProgressServer(action_data);
		
		ActionGiveBloodData action_data_b = ActionGiveBloodData.Cast( action_data );
		
		PlayerBase player_target = PlayerBase.Cast(action_data_b.m_Target.GetObject());
		int bloodtypetarget = player_target.GetStatBloodType().Get();
		bool bloodmatch = BloodTypes.MatchBloodCompatibility(action_data_b.m_ItemBloodType, bloodtypetarget);
		
		if ( bloodmatch )
		{
			PlayerBase sourcePlayer = PlayerBase.Cast(action_data.m_Player);
			string sourcePlayerID = sourcePlayer.GetIdentity().GetPlainId();
			GetHeroesAndBandits().NewPlayerAction(sourcePlayerID, "MedicGiveBlood");
		} else
		{ //TODO
			
		}
	}
}