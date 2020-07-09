#ifdef EXPANSIONMOD
modded class IngameHud
{
	protected ImageWidget		m_PlayerTagHABIcon;
	
	void IngameHud()
	{

	}

	//============================================
	// Override ShowPlayerTag
	//============================================
	override void ShowPlayerTag( float timeslice )
	{
		
		bool continueNow;
		if ( m_CurrentTaggedPlayer && m_CurrentTaggedPlayer.GetIdentity() && GetHeroesAndBanditsLevels())
		{
			if ( !m_PlayerTag )
			{
				m_PlayerTag = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/hud/expansion_hud_player_tag.layout");
				m_PlayerTagText = TextWidget.Cast( m_PlayerTag.FindAnyWidget( "TagText" ) );
				m_PlayerTagHABIcon = ImageWidget.Cast( m_PlayerTag.FindAnyWidget( "TagIcon" ) );
				m_PlayerTagHABIcon.SetSize(0, 0);
			} else if (!m_PlayerTagHABIcon) {
				m_PlayerTagHABIcon = ImageWidget.Cast( m_PlayerTag.FindAnyWidget( "TagIcon" ) );
				m_PlayerTagHABIcon.SetSize(0, 0);
			}
			
			m_PlayerSpineIndex = m_CurrentTaggedPlayer.GetBoneIndex( "Spine2" );
			vector player_pos = m_CurrentTaggedPlayer.GetBonePositionWS( m_PlayerSpineIndex );
			vector screen_pos = GetGame().GetScreenPosRelative( player_pos );
			
			if ( screen_pos[2] > 0 )
			{
				if ( screen_pos[0] > 0 && screen_pos[0] < 1 )
				{
					if ( screen_pos[1] > 0 && screen_pos[1] < 1 )
					{
						m_PlayerTagHABIcon.SetSize(32, 32);
						m_PlayerTagHABIcon.LoadImageFile( 0, GetHeroesAndBanditsLevels().Levels.Get(m_CurrentTaggedPlayer.GetHeroesAndBanditsLevelIndex()).LevelImage, true );
						super.ShowPlayerTag( timeslice );
					}
				}
			}
		}
		
		if ( m_PlayerTag )
		{
			float new_alpha = Math.Clamp( m_PlayerTagText.GetAlpha() - timeslice * 10, 0, 1 );
			m_PlayerTagText.SetAlpha( Math.Clamp( m_PlayerTagText.GetAlpha() - timeslice * 10, 0, 1 ) );
			if ( new_alpha == 0 )
			{
				m_PlayerTagHABIcon.SetSize(0, 0);
				m_CurrentTaggedPlayer = null;
			}
		}
		super.ShowPlayerTag( timeslice );
	}
}
#endif