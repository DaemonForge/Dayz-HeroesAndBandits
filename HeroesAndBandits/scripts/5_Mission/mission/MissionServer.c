modded class MissionServer extends MissionBase
{
    void ~MissionServer()
	{
		
	}
	
	override void OnInit()
	{
		super.OnInit();
		OnHaBInit();
	}
	
	override void OnHaBInit(){
		m_HeroesAndBandits = new HeroesAndBandits;
		m_HeroesAndBandits.Init();
	}
	
	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);
	}
	
	
	
	override void InvokeOnDisconnect( PlayerBase player )
	{
		super.InvokeOnDisconnect(player);
	}
};
