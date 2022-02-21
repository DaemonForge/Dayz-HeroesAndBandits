modded class MissionServer extends MissionBase
{
    void ~MissionServer()
	{
		
	}
	
	override void OnInit()
	{
		super.OnInit();
	}
	
	override void UniversalApiReady(){
		super.UniversalApiReady();
		OnHaBInit();
	}
	
	override void OnHaBInit(){
		HABActionConfigs.Init();
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
