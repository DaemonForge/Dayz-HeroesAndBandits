modded class MissionBase extends MissionBaseWorld
{
    override UIScriptedMenu CreateScriptedMenu(int id) {
        UIScriptedMenu menu = NULL;
        menu = super.CreateScriptedMenu(id);
        if (!menu) {
            switch (id) {
                case HAB_PANEL_MENU:
                    menu = new HAB_OG_Panel;
                    break;
            }
            if (menu) {
                menu.SetID(id);
            }
        }
        return menu;
    }
}