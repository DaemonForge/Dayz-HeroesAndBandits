

class HeroesAndBanditsIconUI extends UIScriptedMenu
{	
	ImageWidget			m_Icon;
	string              m_layout;
	
	override Widget Init()
    {
		layoutRoot 	= GetGame().GetWorkspace().CreateWidgets( m_layout );
		m_Icon		= ImageWidget.Cast( layoutRoot.FindAnyWidget( "HeroesAndBanditsIcon" ) );
		m_Icon.LoadImageFile( 0, "HeroesAndBandits/gui/images/BambiNotification.paa" );
		
        return layoutRoot;
    }
	
	void updateLocation(int location)
	{
		if ( location == 1 )
		{
			m_layout = "HeroesAndBandits/gui/images/HeroesAndBandits.layout";
		} else {
			m_layout = "HeroesAndBandits/gui/images/HeroesAndBandits.layout";
		}
	}

	void updateIcon(string imageSet)
	{
		m_Icon.LoadImageFile( 0, imageSet, true );
	}
}