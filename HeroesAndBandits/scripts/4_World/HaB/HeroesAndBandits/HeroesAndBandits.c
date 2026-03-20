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
		// Safety check - if Levels not yet initialized, return Bambi
		if (!Levels || Levels.Count() == 0)
			return HAB_BAMBI;
		
		if (humanity >= Levels[0]){
			return HAB_HERO;
		}
		float bandit = Levels[0] * -1;
		if (humanity <= bandit){
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
		UpdateLevels({1000,2500,5000,10000,20000,40000,80000,200000,500000,1000000});
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
	
	static void NewPlayerAction(string player, string action)
	{
		Print("[HAB] New Contoller Action " + action + " for " + player);
		PlayerBase ply;
		if (Class.CastTo(ply, UUtil.FindPlayer(player))){
			ply.NewHABAction(action);
			return;
		}
		if (Class.CastTo(ply, FindPlayerByPlainId(player))){
			ply.NewHABAction(action);
			return;
		}
	}
	static DayZPlayer FindPlayerByPlainId(string id){
		if (g_Game.IsServer()){
			autoptr array<Man> players = new array<Man>;
			g_Game.GetPlayers( players );
			for (int i = 0; i < players.Count(); i++){
				DayZPlayer player = DayZPlayer.Cast(players.Get(i));
				if (player.GetIdentity() && player.GetIdentity().GetPlainId() == id ){
					return player;
				}
			}
		}
		return NULL;
	} 
	
	static void NotifyPlayer(PlayerBase player, string image ,string message, string heading = "#HAB_HUMANITY_CHANGEHEADING")
	{
		if (player && player.GetIdentity()){
			UUtil.SendNotification(heading, message, player.GetIdentity(), image);
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
	
	static void TriggerSuicideFeed(PlayerBase sourcePlayer){
		
	}
	
	HeroesAndBanditsPlayerBase GetPlayer( string pID )
	{
		return null;
	}
	
	
	float GetPlayerHumanity( string pID )
	{
		PlayerBase ply;
		if (Class.CastTo(ply, UUtil.FindPlayer(pID))){
			return ply.Humanity();
		}
		if (Class.CastTo(ply, FindPlayerByPlainId(pID))){
			return ply.Humanity();
		}
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

