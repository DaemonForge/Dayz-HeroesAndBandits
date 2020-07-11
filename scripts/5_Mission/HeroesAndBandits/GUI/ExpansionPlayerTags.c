#ifdef EXPANSIONMOD

modded class IngameHud
{
	string 						m_CurrentTaggedPlayer_HABicon = "";
	string 						m_CurrentTaggedPlayer_LastRequested = "";
	protected ImageWidget		m_PlayerTagHABIcon;
	
	void IngameHud()
	{
		GetRPCManager().AddRPC( "HaB", "RPCReceiveHABIcon", this, SingeplayerExecutionType.Both );
	}

	void RPCReceiveHABIcon( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
		if (GetHeroesAndBanditsSettings())
		{ 
			if (GetHeroesAndBanditsSettings().EnableLevelImageOnExpansionPlayerTag)
			{
				Param2< string, string > data;
				if ( !ctx.Read( data ) ) return;
			        m_CurrentTaggedPlayer_HABicon = data.param1;
					m_PlayerTagHABIcon.LoadImageFile( 0, m_CurrentTaggedPlayer_HABicon, true );
					m_PlayerTagHABIcon.SetSize(32, 32);
			}
		}
	}
	
	//============================================
	// Override ShowPlayerTag
	//============================================
	override void ShowPlayerTag( float timeslice )
	{	
		if (GetHeroesAndBanditsSettings())
		{ 
			if (GetHeroesAndBanditsSettings().EnableLevelImageOnExpansionPlayerTag)
			{
		
				if ( m_CurrentTaggedPlayer && m_CurrentTaggedPlayer.GetIdentity() )
				{ 
					if ( m_CurrentTaggedPlayer.GetIdentityName() != m_CurrentTaggedPlayer_LastRequested){ //perventing icon from being grabbed too manytimes
						GetRPCManager().SendRPC("HaB", "RPCRequestHABIcon", new Param1< string >( m_CurrentTaggedPlayer.GetIdentityName() ), true);
						m_CurrentTaggedPlayer_LastRequested = m_CurrentTaggedPlayer.GetIdentityName();
					}
						if ( !m_PlayerTag )
						{
							m_PlayerTag = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/hud/expansion_hud_player_tag.layout");
							m_PlayerTagText = TextWidget.Cast( m_PlayerTag.FindAnyWidget( "TagText" ) );	
						}
						m_PlayerTagHABIcon = ImageWidget.Cast( m_PlayerTag.FindAnyWidget( "TagIcon" ) );
						m_PlayerTagHABIcon.SetSize(32, 32);
						super.ShowPlayerTag( timeslice );
						m_PlayerSpineIndex = m_CurrentTaggedPlayer.GetBoneIndex( "Spine2" );
						vector player_pos = m_CurrentTaggedPlayer.GetBonePositionWS( m_PlayerSpineIndex );
						vector screen_pos = GetGame().GetScreenPosRelative( player_pos );
						
						if ( screen_pos[2] > 0 )
						{
							if ( screen_pos[0] > 0 && screen_pos[0] < 1 )
							{
								if ( screen_pos[1] > 0 && screen_pos[1] < 1 )
								{
									m_PlayerTagHABIcon.SetAlpha( Math.Clamp( m_PlayerTagHABIcon.GetAlpha() + timeslice * 10, 0, 1 ) );
									return;
								}
							}
						}
				} 
				
				if ( m_PlayerTag ) {
					float new_alpha = Math.Clamp( m_PlayerTagHABIcon.GetAlpha() - timeslice * 10, 0, 1 );
					m_PlayerTagHABIcon.SetAlpha( Math.Clamp( m_PlayerTagHABIcon.GetAlpha() - timeslice * 10, 0, 1 ) );
					if ( new_alpha == 0 )
					{
						m_PlayerTagHABIcon.SetSize(0, 0);
						m_CurrentTaggedPlayer_HABicon = "";
						m_CurrentTaggedPlayer_LastRequested = "";
					}
					
				}
			}
		}
		super.ShowPlayerTag( timeslice );
	}
}
#endif