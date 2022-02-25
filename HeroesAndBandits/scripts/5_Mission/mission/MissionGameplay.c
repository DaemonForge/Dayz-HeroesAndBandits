modded class MissionGameplay extends MissionBase
{

	override void OnMissionStart()
    {
        super.OnMissionStart();
		m_HABStatusBarIconWidget = new HABStatusBarIconWidget;
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
			m_HABStatusBarIconWidget.HABOnUpdate(timeslice, shouldHide );
		}
		
        Input input = GetGame().GetInput();
        if (input.LocalPress("UAUIBack", false)) {
            if (GetGame().GetUIManager().IsMenuOpen(HAB_PANEL_MENU)) {
				GetGame().GetUIManager().CloseMenu(HAB_PANEL_MENU);
            }
        }
        if (input.LocalPress("UAHeroesAndBanditsPanel", false)) {
			if (GetGame().GetUIManager().GetMenu() == NULL && !GetGame().GetUIManager().IsMenuOpen(HAB_PANEL_MENU)) {
				GetGame().GetUIManager().EnterScriptedMenu(HAB_PANEL_MENU,NULL);
            } else if (GetGame().GetUIManager().IsMenuOpen(HAB_PANEL_MENU)) {
				GetGame().GetUIManager().CloseMenu(HAB_PANEL_MENU);
            }
        }
    }
	
	
	
}
