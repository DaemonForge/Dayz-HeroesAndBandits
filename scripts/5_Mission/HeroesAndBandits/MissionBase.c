modded class MissionBase {
	
    override UIScriptedMenu CreateScriptedMenu (int id) {
        UIScriptedMenu menu = NULL;
        menu = super.CreateScriptedMenu (id);
        if (!menu) {
            switch (id) {
                case HEROESANDBANDITS_PANEL_MENU:
                    menu = new HeroesAndBanditsPanelUI;
                    break;
            }
            if (menu) {
                menu.SetID (id);
            }
        }
        return menu;
    }
}