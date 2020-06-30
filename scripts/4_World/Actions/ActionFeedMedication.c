moddel class ActionFeedTetracyclineAntibiotics
{
	override void OnFinishProgressServer( ActionData action_data )
	{	
		super.OnFinishProgressServer(action_data);
		PlayerBase sourcePlayer = PlayerBase.Cast(action_data.m_Player);
		string sourcePlayerID = sourcePlayer.GetIdentity().GetPlainId();
		GetHeroesAndBandits().NewPlayerAction(sourcePlayerID, "MedicFeedTetracycline");
	}
}

modded class ActionFeedPainkillerTablets
{
	override void OnFinishProgressServer( ActionData action_data )
	{	
		super.OnFinishProgressServer(action_data);
		PlayerBase sourcePlayer = PlayerBase.Cast(action_data.m_Player);
		string sourcePlayerID = sourcePlayer.GetIdentity().GetPlainId();
		GetHeroesAndBandits().NewPlayerAction(sourcePlayerID, "MedicFeedPainkiller");
	}
}

  
modded class ActionFeedCharcoalTablets
{
	override void OnFinishProgressServer( ActionData action_data )
	{	
		super.OnFinishProgressServer(action_data);
		PlayerBase sourcePlayer = PlayerBase.Cast(action_data.m_Player);
		string sourcePlayerID = sourcePlayer.GetIdentity().GetPlainId();
		GetHeroesAndBandits().NewPlayerAction(sourcePlayerID, "MedicFeedCharcoal");
	}
}

modded class ActionFeedVitaminBottle
{	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		super.OnFinishProgressServer(action_data);
		PlayerBase sourcePlayer = PlayerBase.Cast(action_data.m_Player);
		string sourcePlayerID = sourcePlayer.GetIdentity().GetPlainId();
		GetHeroesAndBandits().NewPlayerAction(sourcePlayerID, "MedicFeedVitamin");
	}
	
}