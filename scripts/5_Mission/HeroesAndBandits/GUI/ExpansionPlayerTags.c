#ifdef EXPANSIONMOD
modded class IngameHud
{
	
	void IngameHud()
	{

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

		if ( m_CurrentTaggedPlayer && m_CurrentTaggedPlayer.GetIdentity() )
		{
			if ( !m_PlayerTag )
			{
				m_PlayerTag = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/hud/expansion_hud_player_tag.layout");
				m_PlayerTagText = TextWidget.Cast( m_PlayerTag.FindAnyWidget( "TagText" ) );
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
						m_PlayerTagText.SetAlpha( Math.Clamp( m_PlayerTagText.GetAlpha() + timeslice * 10, 0, 1 ) );
						m_PlayerTag.SetPos( 0.55, 0.55 );
						m_PlayerTagText.SetText( m_CurrentTaggedPlayer.GetIdentityName() );
						//! m_PlayerTagText.SetSize( 1, 1 - screen_pos[2] / 25  );
						return;
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
				m_PlayerTagText.SetText( "" );
				m_CurrentTaggedPlayer = null;
			}
		}
	}
}
#endif