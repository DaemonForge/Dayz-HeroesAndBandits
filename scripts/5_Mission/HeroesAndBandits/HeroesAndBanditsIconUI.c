

class HeroesAndBanditsIconUI extends UIScriptedMenu
{	
	ImageWidget			m_Icon;
		
	override Widget Init()
    {
		layoutRoot 	= GetGame().GetWorkspace().CreateWidgets( "HeroesAndBandits/gui/images/HeroesAndBandits.layout" );
		m_Icon		= ImageWidget.Cast( layoutRoot.FindAnyWidget( "HeroesAndBanditsIcon" ) );
				
		m_Icon.LoadImageFile( 0, "set:HeroesAndBandits image:Bambi" );
        return layoutRoot;
    }

	void updateIcon(string imageSet)
	{
		m_Icon.LoadImageFile( 0, imageSet );
	}
}