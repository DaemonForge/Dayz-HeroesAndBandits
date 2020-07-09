#ifdef EXPANSIONMOD

modded class IngameHud
{
	string 						m_CurrentTaggedPlayer_HABicon;
	protected ImageWidget		m_PlayerTagHABIcon;
	
	void IngameHud()
	{
		GetRPCManager().AddRPC( "HaB", "RPCReceiveHABIcon", this, SingeplayerExecutionType.Both );
	}

	void RPCReceiveHABIcon( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
		Param2< string, string > data;
		if ( !ctx.Read( data ) ) return;
		if (GetGame().GetPlayer().GetIdentity().GetPlainId() == data.param1){
	        m_CurrentTaggedPlayer_HABicon = data.param1;
			m_PlayerTagHABIcon.LoadImageFile( 0, m_CurrentTaggedPlayer_HABicon, true );
		}
	}
	
	//============================================
	// Override RefreshPlayerTags
	//============================================
	override void RefreshPlayerTags()
	{
		super.RefreshPlayerTags();
		if ( m_CurrentTaggedPlayer ){
			GetRPCManager().SendRPC("HaB", "RPCRequestHABIcon", new Param1< string >(m_CurrentTaggedPlayer.GetIdentity().GetPlainId() ), false);
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
				super.ShowPlayerTag( timeslice );
		}
		
		if ( m_PlayerTag )
		{
			float new_alpha = Math.Clamp( m_PlayerTagText.GetAlpha() - timeslice * 10, 0, 1 );
			m_PlayerTagHABIcon.SetAlpha( Math.Clamp( m_PlayerTagText.GetAlpha() - timeslice * 10, 0, 1 ) );
			if ( new_alpha == 0 )
			{
				m_PlayerTagHABIcon.SetSize(0, 0);
			}
			
		}
		super.ShowPlayerTag( timeslice );
	}
}
#endif