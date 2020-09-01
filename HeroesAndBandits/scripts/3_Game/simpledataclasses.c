class HABSimpleLevel {
	string Name;
	float Humanity;
	void HABSimpleLevel(string name, float humanity){
		Name = name;
		Humanity = humanity;
	}
};

class HABSimpleAction{
	string Name;
	float Humanity;
	void HABSimpleAction(string name, float humanity){
		Name = name;
		Humanity = humanity;
	}
};

class HABSimpleZone{
    string Name;
	float X;
	float Z;
	float Radius;
	float MinHumanity = -1000;
	float MaxHumanity = 1000;
	string WelcomeMessage = "Welcome to the Default Zone";
	string WarningMessage = "!!Warning!! you are about to enter Default Zone if you continue you will be shot!";
	bool OverrideSafeZone = false;
	bool GodModPlayers = false;
	bool PreventWeaponRaise = false;
	bool PreventActions = false;
	bool PreventTrade = false;
	bool KillAggressors = false;
	float RespawnTimer = 600;
	int GuardDifficulty = 5; 
	ref array<ref HABSimpleGuard> Guards = new ref array<ref HABSimpleGuard>;
	//5 OP can't be killed Can Shot Throw Objects, and 100% HitChance, 
	//4 Require Line Of Sight 92% HitChance can't be killed, medium high fire rate 
	//3 Require Line Of Sight 90% HitChance can be killed, high fire rate,
	//2 Require Line Of Sight 85% HitChance can be killed, medium high fire rate  
	//1 Require Line Of Sight 80% HitChance can be killed, low high fire rate 
	
	void HABSimpleZone(string name, float x, float y, float radius){
		Name = name;
		X = x;
		Z = y;
		Radius = radius;
	}
};

//Class for holding guard values
class HABSimpleGuard
{
	float X;
	float Y;
	float Z;
	float Orientation;
	string Skin;
	string WeaponInHands = "M4A1";
	ref TStringArray WeaponInHandsAttachments = {"M4_RISHndgrd", "M4_OEBttstck", "M68Optic"};
	ref TStringArray GuardGear =  { "PlateCarrierVest", "JungleBoots_Black", "CargoPants_Black", "M65Jacket_Black"};

	void HABSimpleGuard(float x, float y, float z, float orientation = 0.0, string skin = "SurvivorM_Hassan", string weaponInHands = "M4A1") 
	{
		X = x;
		Y = y;
		Z = z;
		Orientation = orientation;
		if ( skin == ""){
		 	Skin = "SurvivorM_Hassan";
		}else {
			Skin = skin;
		}
	}
};
