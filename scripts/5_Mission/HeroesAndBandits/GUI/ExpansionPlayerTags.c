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
		Param2< string, string > data;
		if ( !ctx.Read( data ) ) return;
	        m_CurrentTaggedPlayer_HABicon = data.param1;
			m_PlayerTagHABIcon.LoadImageFile( 0, m_CurrentTaggedPlayer_HABicon, true );
			m_PlayerTagHABIcon.SetSize(32, 32);
	}
	
	//============================================
	// Override RefreshPlayerTags
	//============================================
	override void RefreshPlayerTags()
	{
		if ( GetGame().GetPlayer() )
		{
			
			int viewrange = GetExpansionSettings().GetGeneral().PlayerTagViewRange;
			bool found = false;
			vector head_pos = GetGame().GetCurrentCameraPosition();
			float distance;
			foreach ( Man player : ClientData.m_PlayerBaseList )
			{
				vector target_player = player.GetPosition();
				distance = vector.Distance( head_pos, target_player );
				
				target_player[1] = target_player[1] + 1.2;
				
				if ( distance <= viewrange && player != GetGame().GetPlayer() )
				{
					vector screen_pos = GetGame().GetScreenPosRelative( target_player );
					vector end_pos = head_pos + GetGame().GetCurrentCameraDirection() * 25;
					RaycastRVParams params = new RaycastRVParams( head_pos, end_pos, GetGame().GetPlayer(), 0 );
					params.sorted = true;
					
					array<ref RaycastRVResult> results = new array<ref RaycastRVResult>;
					DayZPhysics.RaycastRVProxy( params, results );
					if ( results.Count() > 0 )
					{
						if ( results.Get( 0 ).obj == player )
						{
							m_CurrentTaggedPlayer = PlayerBase.Cast( player );
							m_CurrentTaggedPlayer.SetSynchDirty()
							found = true;
						}
					}
				}
			}
			if ( !found )
			{
				m_CurrentTaggedPlayer = null;
			}
		}
	}
	
	//============================================
	// Override ShowPlayerTag
	//============================================
	override void ShowPlayerTag( float timeslice )
	{
		if ( m_CurrentTaggedPlayer && m_CurrentTaggedPlayer.GetIdentity())
		{
				if ( !m_PlayerTag )
				{
					m_PlayerTag = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/hud/expansion_hud_player_tag.layout");
					m_PlayerTagText = TextWidget.Cast( m_PlayerTag.FindAnyWidget( "TagText" ) );	
				}
				m_PlayerTagHABIcon = ImageWidget.Cast( m_PlayerTag.FindAnyWidget( "TagIcon" ) );
				m_PlayerTagHABIcon.SetSize(32, 32);
				if (GetHeroesAndBanditsLevels()){}
					m_PlayerTagHABIcon.LoadImageFile( 0, GetHeroesAndBanditsLevels().getLevelByIndex(m_CurrentTaggedPlayer.GetHeroesAndBanditsLevelIndex()).LevelImage, true );
				}
				super.ShowPlayerTag( timeslice );
		}
		
		if ( m_PlayerTag )
		{
			float new_alpha = Math.Clamp( m_PlayerTagText.GetAlpha() - timeslice * 10, 0, 1 );
			m_PlayerTagHABIcon.SetAlpha( Math.Clamp( m_PlayerTagText.GetAlpha() - timeslice * 10, 0, 1 ) );
			if ( new_alpha == 0 )
			{
				m_PlayerTagHABIcon.SetSize(0, 0);
				m_CurrentTaggedPlayer_HABicon = "";
				m_CurrentTaggedPlayer_LastRequested = "";
			}
			
		}
		super.ShowPlayerTag( timeslice );
	}
}
#endif