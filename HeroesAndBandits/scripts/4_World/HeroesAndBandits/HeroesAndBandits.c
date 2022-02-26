//Server Variables
static autoptr HeroesAndBandits m_HeroesAndBandits;
class HeroesAndBandits extends Managed
{	
	static autoptr TIntArray Levels;
	void HeroesAndBandits()
	{
		
	}
	
	static int GetLevel(float humanity){
		int level = GetABSLevel(humanity);
		if (humanity < 0)
			return level * -1;
		return level;
	}
	
	static int GetABSLevel(float humanity){
		int i = 0;
		for (i = 0; i < Levels.Count(); i++){
			if (Math.AbsFloat(humanity) < Levels.Get(i)){
				return i;
			}
		}
		return i;
	}
	
	static int GetAffinity(float humanity){
		if (humanity >= 1000){
			return HAB_HERO;
		}
		if (humanity <= -1000){
			return HAB_BANDIT;
		}
		return HAB_BAMBI;
	}
	
	static HeroesAndBanditsControllerBase Controller(float humanity, PlayerBase player){
		return GetHeroesAndBandits().GetController(humanity,player);
	}
	
	HeroesAndBanditsControllerBase GetController(float humanity, PlayerBase player){
		switch (GetAffinity(humanity)){
			case HAB_HERO:
				return new HeroController(player);
			break;
			case HAB_BANDIT:
				return new BanditController(player);
			break;
			default:
				return new BambiController(player);
			break;
		}
		return new BambiController(player);
	}
	
	
	void Init(){
		UpdateLevels({1000,3000,6000,15000,30000,80000,200000,500000,1000000});
	}
	
	static void UpdateLevels(TIntArray levels){
		Levels = levels;
	}
		
	void OnPlayerInit(PlayerBase player)
	{
		
	}
	
	void OnPlayerDisconnect(PlayerBase player)
	{
		
	}
	
	void NewPlayerAction(PlayerBase player, string action)
	{
		
	}
	
	void NewPlayerAction(string player, string action)
	{
		
	}
	
	static void NotifyPlayer(PlayerBase player, string image ,string message, string heading = "#HAB_HUMANITY_CHANGEHEADING")
	{
		if (player && player.GetIdentity()){
			NotificationSystem.CreateNotification(new StringLocaliser(heading), new StringLocaliser(message), image, ARGB(255,255,255,255), 4, player.GetIdentity());
		}
	}
	
	static void NotifyKillFeed(string image ,string message, string heading = "#HAB_KILLFEED_HEADING")
	{
	
	}
	
	static void WarnPlayer( string header, string message, PlayerBase player)
	{
		if (player && player.GetIdentity()){
			
		}
	}
		
	static void WelcomePlayer( string zoneName, string message, string welcomeImage, PlayerBase player, int welcomeColor)
	{
		if (player && player.GetIdentity()){
		}
	}
	
	static void TriggerKillFeed(PlayerBase sourcePlayer, PlayerBase targetPlayer, string weaponName, int deathType = -1, string zoneImage = ""){
		

	}
	
	static void TriggerSucideFeed(PlayerBase sourcePlayer){
		
	}
	
	HeroesAndBanditsPlayerBase GetPlayer( string pID )
	{
		return null;
	}
	
	
	float GetPlayerHumanity( string pID )
	{
		return 0;
	}
	
	int GetPlayerStat( string pID, string stat )
	{
		return 0;
	}
	
	string GetPlayerAffinity( string pID )
	{
		return "";
	}
	
	string GetPlayerHeroOrBandit( string pID )
	{
		return "";
	}
	
	/*habLevel GetPlayerLevel( string pID )
	{
		return NULL;
	}*/
	
	string GetPlayerLevelName( string pID )
	{
		return "";
	}
	
	string getPlayerSkin(string playerID, int skinIndex = -1){
		return "";
	}
	
	
	void CheckPlayersEnterZones(){
		
	}
	
	void ReduceAggressors(){
		
	}
	
	void updatePlayerTotals()
	{
	}
	
	void NewAggressorAction(PlayerBase player, string action, EntityAI entity = NULL) {
	}
	
	void SaveAggressorData(bool lazy = true){
	}
	
};


static HeroesAndBandits GetHeroesAndBandits()
{
	return m_HeroesAndBandits;
};

