
class HeroesAndBanditsPanelUI extends UIScriptedMenu
{	
	private bool m_HeroesAndBanditsPanelIsOpen;
	
	ImageWidget			m_LevelImage;
	RichTextWidget		m_Heading;
	RichTextWidget		m_PlayerName;
	RichTextWidget		m_LevelName;
	RichTextWidget		m_Humanity;
	
	override Widget Init()
    {
		layoutRoot 		= GetGame().GetWorkspace().CreateWidgets( "HeroesAndBandits/gui/layouts/HeroesAndBanditsPanel.layout" );
		m_LevelImage	= ImageWidget.Cast( layoutRoot.FindAnyWidget( "HABLevelImage" ) );
		m_Heading		= RichTextWidget.Cast( layoutRoot.FindAnyWidget( "HABHeading" ) );
		m_PlayerName	= RichTextWidget.Cast( layoutRoot.FindAnyWidget( "HABPlayerName" ) );
		m_LevelName		= RichTextWidget.Cast( layoutRoot.FindAnyWidget( "HABLevelName" ) );
		m_Humanity		= RichTextWidget.Cast( layoutRoot.FindAnyWidget( "HABHumanity" ) );
		Print("[HeroesAndBandits] [DebugClient] Init Panel ");
		m_LevelImage.LoadImageFile( 0, g_HeroesAndBanditsLevel.LevelImage , true );
		Print("[HeroesAndBandits] [DebugClient] Loading Image: " + g_HeroesAndBanditsLevel.LevelImage);
		m_Heading.SetText("#HAB_TITLE");
		m_PlayerName.SetText(GetGame().GetPlayer().GetName());
		Print("[HeroesAndBandits] [DebugClient] Setting Player Name: " + GetGame().GetPlayer().GetName());
		m_LevelName.SetText(g_HeroesAndBanditsLevel.Name);
		Print("[HeroesAndBandits] [DebugClient] Setting Level Name: " + g_HeroesAndBanditsLevel.Name);
		m_Humanity.SetText("#HAB_HUMANITY: " + g_HeroesAndBanditsPlayer.Humanity);
		Print("[HeroesAndBandits] [DebugClient] Setting Humanity: " + g_HeroesAndBanditsPlayer.Humanity);
        return layoutRoot; 
    }


	void updateData()
	{
		Print("[HeroesAndBandits] [DebugClient] Update Panel ");
		m_LevelImage.LoadImageFile( 0, g_HeroesAndBanditsLevel.LevelImage , true );
		Print("[HeroesAndBandits] [DebugClient] Loading Image: " + g_HeroesAndBanditsLevel.LevelImage);
		m_Heading.SetText("#HAB_TITLE");
		m_PlayerName.SetText(GetGame().GetPlayer().GetName());
		Print("[HeroesAndBandits] [DebugClient] Setting Player Name: " + GetGame().GetPlayer().GetName());
		m_LevelName.SetText(g_HeroesAndBanditsLevel.Name);
		Print("[HeroesAndBandits] [DebugClient] Setting Level Name: " + g_HeroesAndBanditsLevel.Name);
		m_Humanity.SetText("#HAB_HUMANITY: " + g_HeroesAndBanditsPlayer.Humanity);
		Print("[HeroesAndBandits] [DebugClient] Setting Humanity: " + g_HeroesAndBanditsPlayer.Humanity);
	}
	
	
	bool IsOpen () {
		return m_HeroesAndBanditsPanelIsOpen;
	}

	void SetOpen (bool open) {
		m_HeroesAndBanditsPanelIsOpen = open;
	}
}