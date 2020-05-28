

modded class MissionServer
{
    void ~MissionServer()
	{
		habPrint("Saving All Player Data From ~MissionServer", "Debug");
		GetHeroesAndBandits().SaveAllPlayers();
	}
		
	override void OnInit()
	{
		super.OnInit();
		GetHeroesAndBanditsConfig();
		GetHeroesAndBandits();
	}
	
	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		habPrint("InvokeOnConnect Player Connected", "Debug");
		GetHeroesAndBandits().OnPlayerConnect(identity);
		super.InvokeOnConnect(player, identity);
		
	}

	override void InvokeOnDisconnect( PlayerBase player )
	{
		habPrint("InvokeOnDisconnect Player Disconneted", "Debug");
		GetHeroesAndBandits().OnPlayerDisconnect(player);
		super.InvokeOnDisconnect(player);
	}
	
	
}


