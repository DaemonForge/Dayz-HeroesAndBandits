modded class MissionBase extends MissionBaseWorld
{
    override UIScriptedMenu CreateScriptedMenu(int id) {
        UIScriptedMenu menu = NULL;
        menu = super.CreateScriptedMenu(id);
        if (!menu) {
            switch (id) {
                case HAB_PANEL_MENU:
                    menu = new HAB_MainPanel;
                    break;
            }
            if (menu) {
                menu.SetID(id);
            }
        }
        return menu;
    }
	
	
	override void UFrameworkReady(){
		m_HaBGeneralConfigHandler.Load(this, "CBLoadHaBGeneralConfig");
		super.UFrameworkReady();
	}
	
	
	void CBLoadHaBGeneralConfig(int cid, int status, string oid, HaBGeneralConfig data){
		if (status == UF_SUCCESS){
			Class.CastTo(m_HaBGeneralConfig,data);
			HEROROLE = m_HaBGeneralConfig.HeroRole;
			BANDITROLE = m_HaBGeneralConfig.BanditRole;
			HEROPATHROLE = m_HaBGeneralConfig.PathHeroRole;
			BANDITPATHROLE = m_HaBGeneralConfig.PathBanditRole;
			HeroesAndBandits.UpdateLevels(m_HaBGeneralConfig.Levels);
		}
		if (status == UF_EMPTY){
			m_HaBGeneralConfig = new HaBGeneralConfig();
			m_HaBGeneralConfigHandler.Save(m_HaBGeneralConfig);
		}
	}
}