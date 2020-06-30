

class HeroesAndBanditsIconUI extends UIScriptedMenu
{	
	ImageWidget			m_Icon;
	
	override Widget Init()
    {
		layoutRoot 	= GetGame().GetWorkspace().CreateWidgets( "HeroesAndBandits/gui/layouts/HeroesAndBandits.layout" );
		m_Icon		= ImageWidget.Cast( layoutRoot.FindAnyWidget( "HeroesAndBanditsIcon" ) );
		m_Icon.LoadImageFile( 0, g_HeroesAndBanditsPlayer.getLevel().LevelImage , true );
		
        return layoutRoot;
    }


	void updateIcon(string imageSet)
	{
		m_Icon.LoadImageFile( 0, imageSet, true );
	}
}