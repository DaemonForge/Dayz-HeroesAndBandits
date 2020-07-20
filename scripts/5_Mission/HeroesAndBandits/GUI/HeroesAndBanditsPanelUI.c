class HeroesAndBanditsPanelUI extends UIScriptedMenu
{	
	private bool m_HeroesAndBanditsPanelIsOpen;
	
	ImageWidget			m_LevelImage;
	RichTextWidget		m_Heading;
	RichTextWidget		m_Affinity;
	RichTextWidget		m_LevelName;
	RichTextWidget		m_Humanity;
	RichTextWidget		m_PlayerKills;
	RichTextWidget		m_ZombieKills;
	RichTextWidget		m_Medic;
	RichTextWidget		m_Raid;
	RichTextWidget		m_Hunts;
	RichTextWidget		m_Sucides;
	string				m_layout = "HeroesAndBandits/gui/layouts/HeroesAndBanditsPanel.layout";
	
	override Widget Init()
    {
		layoutRoot 		= GetGame().GetWorkspace().CreateWidgets( m_layout );
		m_LevelImage	= ImageWidget.Cast( layoutRoot.FindAnyWidget( "HABLevelImage" ) );
		m_Heading		= RichTextWidget.Cast( layoutRoot.FindAnyWidget( "HABHeading" ) );
		m_Affinity		= RichTextWidget.Cast( layoutRoot.FindAnyWidget( "HABAffinity" ) );
		m_LevelName		= RichTextWidget.Cast( layoutRoot.FindAnyWidget( "HABLevelName" ) );
		m_Humanity		= RichTextWidget.Cast( layoutRoot.FindAnyWidget( "HABHumanity" ) );
		m_PlayerKills	= RichTextWidget.Cast( layoutRoot.FindAnyWidget( "HABPlayerKills" ) );
		m_ZombieKills	= RichTextWidget.Cast( layoutRoot.FindAnyWidget( "HABZombieKills" ) );
		m_Medic			= RichTextWidget.Cast( layoutRoot.FindAnyWidget( "HABMedic" ) );
		m_Raid			= RichTextWidget.Cast( layoutRoot.FindAnyWidget( "HABRaid" ) );
		m_Hunts			= RichTextWidget.Cast( layoutRoot.FindAnyWidget( "HABHunt" ) );
		m_Sucides		= RichTextWidget.Cast( layoutRoot.FindAnyWidget( "HABSucides" ) );
		m_LevelImage.LoadImageFile( 0, g_HeroesAndBanditsPlayer.getLevel().LevelImage , true );
		
		m_Heading.SetText(GetHeroesAndBanditsSettings().GUIHeading);

		m_Affinity.SetText("#HAB_AFFINITY: " + g_HeroesAndBanditsPlayer.getAffinity().DisplayName);
		
		m_LevelName.SetText("#HAB_LEVEL: " + g_HeroesAndBanditsPlayer.getLevel().Name);
		
		if (GetHeroesAndBanditsSettings().Mode != 1){
			m_Humanity.SetText("#HAB_HUMANITY: " + g_HeroesAndBanditsPlayer.getHumanity());
		} else { // Temp fix till I can get it translated
			m_Humanity.SetText("#HAB_AFFINITY Points: " + g_HeroesAndBanditsPlayer.getHumanity());
		}
		
		if ( !GetHeroesAndBanditsSettings().HideKillsInGUI )
		{
			m_PlayerKills.SetText("#HAB_PLAYERKILLS: " + g_HeroesAndBanditsPlayer.getStat("Kill"));
		} else {
			m_PlayerKills.SetText(" ");
			//Print("[HeroesAndBandits] [DebugClient] Setting Player Kills: " + g_HeroesAndBanditsPlayer.getStat("Kill"));
		}
		
		m_ZombieKills.SetText("#HAB_ZOMBIEKILLS: " + g_HeroesAndBanditsPlayer.getStat("ZombieKill"));
		
		m_Medic.SetText("#HAB_MEDIC: " + g_HeroesAndBanditsPlayer.getStat("Medic"));
		
		m_Raid.SetText("#HAB_RAID: " + g_HeroesAndBanditsPlayer.getStat("Raid"));
		
		m_Hunts.SetText("#HAB_HUNTS: " + g_HeroesAndBanditsPlayer.getStat("Hunt"));
		
		m_Sucides.SetText("#HAB_SUCIDES: " + g_HeroesAndBanditsPlayer.getStat("Sucide"));
		
        return layoutRoot; 
    }


	void updateData()
	{
		m_LevelImage.LoadImageFile( 0, g_HeroesAndBanditsPlayer.getLevel().LevelImage , true );
		
		m_Heading.SetText(GetHeroesAndBanditsSettings().GUIHeading);
		
		m_Affinity.SetText("#HAB_AFFINITY: " + g_HeroesAndBanditsPlayer.getAffinity().DisplayName);
		
		m_LevelName.SetText("#HAB_LEVEL: " + g_HeroesAndBanditsPlayer.getLevel().Name);
		
		if (GetHeroesAndBanditsSettings().Mode != 1){
			m_Humanity.SetText("#HAB_HUMANITY: " + g_HeroesAndBanditsPlayer.getHumanity());
		} else { // Temp fix till I can get it translated
			m_Humanity.SetText("#HAB_AFFINITY Points: " + g_HeroesAndBanditsPlayer.getHumanity());
		}
		
		if ( !GetHeroesAndBanditsSettings().HideKillsInGUI )
		{
			m_PlayerKills.SetText("#HAB_PLAYERKILLS: " + g_HeroesAndBanditsPlayer.getStat("Kill"));
		} else {
			m_PlayerKills.SetText(" ");
		}
		
		m_ZombieKills.SetText("#HAB_ZOMBIEKILLS: " + g_HeroesAndBanditsPlayer.getStat("ZombieKill"));
		
		m_Medic.SetText("#HAB_MEDIC_ACTIONS: " + g_HeroesAndBanditsPlayer.getStat("Medic"));
		
		m_Raid.SetText("#HAB_RAID: " + g_HeroesAndBanditsPlayer.getStat("Raid"));
		
		m_Hunts.SetText("#HAB_HUNTS: " + g_HeroesAndBanditsPlayer.getStat("Hunt"));
		
		m_Sucides.SetText("#HAB_SUCIDES: " + g_HeroesAndBanditsPlayer.getStat("Sucide"));
	}
	
	
	bool IsOpen () {
		return m_HeroesAndBanditsPanelIsOpen;
	}

	void SetOpen (bool open) {
		m_HeroesAndBanditsPanelIsOpen = open;
	}
}