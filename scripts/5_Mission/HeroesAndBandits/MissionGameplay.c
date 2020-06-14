modded class MissionGameplay
{
	ref HeroesAndBanditsIconUI				m_HeroesAndBanditsIconUI;
	string									m_HeroesAndBanditsCurrentIcon;
	
	override void OnInit()
	{
		super.OnInit();
		m_HeroesAndBanditsIconUI = new HeroesAndBanditsIconUI;
		m_HeroesAndBanditsIconUI.Init();
		if ( !m_HeroesAndBanditsCurrentIcon ) {
			m_HeroesAndBanditsCurrentIcon = "set:HeroesAndBandits image:Bambi";
		}
		GetRPCManager().AddRPC( "HaB", "RPCUpdateHABIcon", this, SingeplayerExecutionType.Both );
	}
	
	
	void RPCUpdateHABIcon( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
		Param2< string, string > data  //Player ID, Icon
		if ( !ctx.Read( data ) ) return;
		string newIcon = data.param2;
		string playerID = data.param1;
		Print("[HeroesAndBandits] [DebugClient] " + playerID + " send icon " + newIcon);
		if ( type == CallType.Server )
    	{	
			if ( !newIcon || !playerID )
			{
				return;
			}
			if ( newIcon != m_HeroesAndBanditsCurrentIcon)
			{
				m_HeroesAndBanditsCurrentIcon = newIcon;
				GetGame().GetCallQueue( CALL_CATEGORY_GUI ).CallLaterByName(this,"UpdateHABIcon", 1000, false);
			} 
			else 
			{
				return;
			}
		}
	}
	
	void UpdateHABIcon(){
		if ( m_HeroesAndBanditsIconUI ) 
		{
			m_HeroesAndBanditsIconUI.updateIcon(m_HeroesAndBanditsCurrentIcon);
		}
	}
}