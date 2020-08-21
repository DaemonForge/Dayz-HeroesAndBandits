class habDeathType 
{
	static int ByUnknown = -1;
	
	static int BySucide = 0;
	
	static int ByBambi = 1;
	static int ByHero = 2;
	static int ByBandit = 3;
	static int ByOtherAffinity = 4;
	
	static int ByZombie = 5;
	
	static int ByOther = 6;
	
	static int ByBleeding = 7;
	
	static int ByGuardAI = 8;
	
	static int ByCarCrash = 9;
	
	static int ByFalling = 10;
	
	static int ByZombieBleeding = 11;
	
}

class habConstant
{
	static string Directory = "$profile:HeroesAndBandits";
	
	static string PlayerDB =  Directory + "\\PlayerDB";
	
	static string SettingsPATH = Directory + "\\settings.json";
	static string ActionsPATH = Directory + "\\actions.json";
	static string ZonesPATH = Directory + "\\zones.json";
	static string ConfigPATH = Directory + "\\simple.json";
	static string LevelsPATH = Directory + "\\levels.json";
}