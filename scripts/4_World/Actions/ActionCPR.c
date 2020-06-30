modded class ActionCPR
{
	override void OnFinishProgressServer( ActionData action_data )
	{	
		super.OnFinishProgressServer(action_data);
		PlayerBase tagetPlayer = PlayerBase.Cast(action_data.m_Target.GetObject());
		float shock = tagetPlayer.GetHealth("","Shock");
		
		if ( shock < PlayerConstants.UNCONSCIOUS_THRESHOLD )
		{
			PlayerBase sourcePlayer = PlayerBase.Cast(action_data.m_Player);
			string sourcePlayerID = sourcePlayer.GetIdentity().GetPlainId();
			GetHeroesAndBandits().NewPlayerAction(sourcePlayerID, "MedicGiveCPR");
		}
	}
}