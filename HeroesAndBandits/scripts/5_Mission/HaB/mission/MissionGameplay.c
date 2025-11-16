modded class MissionGameplay extends MissionBase
{

	override void OnMissionStart()
    {
        super.OnMissionStart();
		Print("OnMissionStart - Creating HABStatusBarIconWidget");
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
			bool shouldHide  = (IsControlDisabled() || IsPaused() || m_Hud.IsHideHudPlayer() || m_Hud.GetHudVisibility().IsContextFlagActive(IngameHudVisibility.HUD_HIDE_FLAGS)) ;
			m_HABStatusBarIconWidget.HABOnUpdate(timeslice, shouldHide );
		}
		
        Input input = g_Game.GetInput();
        if (input.LocalPress("UAUIBack", false)) {
            if (g_Game.GetUIManager().IsMenuOpen(HAB_PANEL_MENU)) {
				g_Game.GetUIManager().CloseMenu(HAB_PANEL_MENU);
            }
        }
        if (input.LocalPress("UAHeroesAndBanditsPanel", false)) {
			if (g_Game.GetUIManager().GetMenu() == NULL && !g_Game.GetUIManager().IsMenuOpen(HAB_PANEL_MENU)) {
				g_Game.GetUIManager().EnterScriptedMenu(HAB_PANEL_MENU,NULL);
            } else if (g_Game.GetUIManager().IsMenuOpen(HAB_PANEL_MENU)) {
				g_Game.GetUIManager().CloseMenu(HAB_PANEL_MENU);
            }
        }
    }
	
	
	
}
