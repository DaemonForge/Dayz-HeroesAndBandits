modded class MissionGameplay extends MissionBase
{
	
	autoptr HABStatusBarIconWidget m_HABStatusBarIconWidget;

	override void OnMissionStart()
    {
        super.OnMissionStart();
		m_HABStatusBarIconWidget = new HABStatusBarIconWidget;
		m_HABStatusBarIconWidget.Init();
    }
	
	override void OnMissionFinish()
    {
        super.OnMissionFinish();
		
		if (m_HABStatusBarIconWidget)
			m_HABStatusBarIconWidget.Destroy();
    }
	
	override void OnUpdate (float timeslice) {
        super.OnUpdate(timeslice);
		if (m_HABStatusBarIconWidget){
			bool shouldHide  = (IsControlDisabled() || IsPaused() || m_Hud.IsHideHudPlayer() || !m_Hud.GetHudState()) ;
			m_HABStatusBarIconWidget.OnUpdate( shouldHide );
		}
    }
};
