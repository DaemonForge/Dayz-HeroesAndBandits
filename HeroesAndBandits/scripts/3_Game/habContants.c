class habDeathType 
{
	static int Unknown = -1;
	
	static int Sucide = 0;
	
	static int Bambi = 1;
	static int Hero = 2;
	static int Bandit = 3;
	
	static int AI = 4;
	
	static int Zombie = 5;
	
	static int Animal = 6;
	
	static int Other = 7;
	
	static int Bleeding = 8;
	
	static int LegacyAI = 9;
	
	static int CarCrash = 10;
	
	static int Falling = 11;
	
	static int ZombieBleeding = 12;
	
};

class habConstant
{
	static string Directory = "$profile:HeroesAndBandits";
	
	static string PlayerDB =  Directory + "\\PlayerDB";
	static string ZoneDB =  Directory + "\\ZoneDB";
	
	static string SettingsPATH = Directory + "\\settings.json";
	static string ActionsPATH = Directory + "\\actions.json";
	static string ZonesPATH = Directory + "\\zones.json";
	static string ConfigPATH = Directory + "\\simple.json";
	static string LevelsPATH = Directory + "\\levels.json";
};