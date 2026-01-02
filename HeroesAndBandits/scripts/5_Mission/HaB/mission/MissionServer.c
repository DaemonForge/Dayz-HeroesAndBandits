modded class MissionServer extends MissionBase
{
    void ~MissionServer()
	{
		
	}
	
	override void OnInit()
	{
		super.OnInit();
		
		#ifdef UniversalComms
		// Register Heroes and Bandits chat channels
		UCChannelRegistry.Register(new HAB_HeroChannel());
		UCChannelRegistry.Register(new HAB_BanditChannel());
		#endif
	}
	
	override void UFrameworkReady(){
		super.UFrameworkReady();
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
