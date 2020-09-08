#ifdef EXPANSIONMOD
modded class IngameHud extends Hud
{
	string 						m_CurrentTaggedPlayer_HABicon = "";
	string 						m_CurrentTaggedPlayer_LastRequested = "";
	
	void IngameHud()
	{
		GetRPCManager().AddRPC( "HaB", "RPCReceiveHABIcon", this, SingeplayerExecutionType.Both );
	}

	void RPCReceiveHABIcon( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
		if (GetHeroesAndBanditsSettings())
		{ 
			if (GetHeroesAndBanditsSettings().Expansion_EnableIconOnPlayerTag && m_PlayerTag && m_PlayerTagIcon)
			{
				Param2< string, string > data;
				if ( !ctx.Read( data ) ) return;
					habPrint("Icon for " + data.param2 + " icon " + data.param1 , "Debug");
			        m_CurrentTaggedPlayer_HABicon = data.param1;
					m_PlayerTagIcon.LoadImageFile( 0, m_CurrentTaggedPlayer_HABicon, true );	
			}
		}
	}
	
	override void ShowPlayerTag( float timeslice )
	{	
		if (GetHeroesAndBanditsSettings())
		{ 
			if (GetHeroesAndBanditsSettings().Expansion_EnableIconOnPlayerTag)
			{
				if ( m_PlayerTag && m_CurrentTaggedPlayer && m_CurrentTaggedPlayer.GetIdentity() )
				{ 
					if ( m_CurrentTaggedPlayer.GetIdentityName() != m_CurrentTaggedPlayer_LastRequested){ //perventing icon from being grabbed too manytimes
						GetRPCManager().SendRPC("HaB", "RPCRequestHABIcon", new Param1< string >( m_CurrentTaggedPlayer.GetIdentityName() ), true);
						m_CurrentTaggedPlayer_LastRequested = m_CurrentTaggedPlayer.GetIdentityName();
					}
				}
			}
		}
		super.ShowPlayerTag( timeslice );
		if (GetHeroesAndBanditsSettings() && m_PlayerTagText){
			if (GetHeroesAndBanditsSettings().Expansion_HideNameOnPlayerTag){
				m_PlayerTagText.SetText( "" );
			}
		}
	}
}
#endif