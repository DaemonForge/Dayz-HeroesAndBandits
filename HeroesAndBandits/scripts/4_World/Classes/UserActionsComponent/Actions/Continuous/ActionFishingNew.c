modded class ActionFishingNewCB {


	override void HandleFishingResultSuccess()
	{	
		super.HandleFishingResultSuccess();
	
		if (m_ActionDataFishing){
			PlayerBase sourcePlayer = PlayerBase.Cast(m_ActionDataFishing.m_Player);
			if (sourcePlayer){
				sourcePlayer.NewHABAction("catchfish");
			}
		}
	}
	
};
