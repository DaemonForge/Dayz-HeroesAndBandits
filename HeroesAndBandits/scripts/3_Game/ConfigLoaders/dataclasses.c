class habStat extends Managed
{
    string Name;
	int Stat;

    void habStat(string statName, int stat) 
	{
        Name = statName;
		Stat = stat;
    }
	
	void updateStat(){
		Stat++;
	}
};


class habPlayerAffinity extends Managed
{
    string Name;
	float Points;

    void habPlayerAffinity(string affinityName, float points = 0) 
	{
        Name = affinityName;
		Points = points;
    }
	
	float getPoints(){
		return Points;
	}
	
	void updatePoints(float amount){
		Points = Points + amount;
	}
	
	void setPoints(float amount){
		Points = amount;
	}
};


//Class for holding levels
class habLevel extends Managed
{
	string Name;
	string Affinity; //bandit / hero / bambi
	string LevelImage;
	float MinPoints;
	float MaxPoints;

	void habLevel(string name, string affinity, string levelImage, float minPoints, float maxPoints) 
	{
		Name = name;
		Affinity = affinity;
		LevelImage = levelImage;
		MinPoints = minPoints;
		MaxPoints = maxPoints;
	}
};

//Class for holding Actions
class habAffinity extends Managed
{
	string Name;
	string DisplayName;
	string Image;
	autoptr array< autoptr habItemList > ItemBlackList = new array< autoptr habItemList >;

	void habAffinity(string name, string displayName, string image) 
	{
		Name = name;
		DisplayName = displayName;
		Image = image;
	}
	
	bool checkItem(float points, string itemType, string location){
		bool allowed = true;
		if (!ItemBlackList){ return true; }
		if (ItemBlackList.Count() == 0){ return true; }
		for (int i = 0; i < ItemBlackList.Count(); i++){
			float minPoints = ItemBlackList.Get(i).MinPoints;
			float maxPoints = ItemBlackList.Get(i).MaxPoints;
			if ( minPoints != -1 && maxPoints != -1 && points >= minPoints && points <= maxPoints){
				if ( !ItemBlackList.Get(i).checkItem(itemType, location) ){
					return false;
				}
			}else if (minPoints == -1 && maxPoints != -1 && points <= maxPoints){
				if ( !ItemBlackList.Get(i).checkItem(itemType, location) ){
					return false;
				}
			}else if (minPoints != -1 && maxPoints == -1 && points >= minPoints){
				if ( !ItemBlackList.Get(i).checkItem(itemType, location) ){
					return false;
				}
			}else if ( minPoints == -1 && maxPoints == -1 ){
				if ( !ItemBlackList.Get(i).checkItem(itemType, location) ){
					return false;
				}
			}
		}
		return true;
	}
	
	void addItemBlackList( float minPoints, float maxPoints, string type, TStringArray items)
	{
		ItemBlackList.Insert(new habItemList( minPoints, maxPoints, type));
		int index = ItemBlackList.Count() - 1;
		ItemBlackList.Get(index).Items = items;
	}
};

class habItemList extends Managed
{
	float MinPoints;
	float MaxPoints;
	string Location; // attach / inventory / inhands / all 
	autoptr TStringArray Items = new TStringArray;
	
	void habItemList( float minPoints, float maxPoints, string location )
	{
		MinPoints = minPoints;
		MaxPoints = maxPoints;
		Location = location;
	}
	
	bool checkItem( string itemType , string location){
		bool check = true;
		bool doCheck = false;
		if ( Location == "all" ){
			doCheck = true;
		} else if ( Location == location){
			doCheck = true;
		} else if (location == "inventory" && Location == "attach"){
			doCheck = true;
		}
		
		if (doCheck){
			int index = Items.Find(itemType);
				if (index != -1){
						check = !check;
				}
		}
		return check;
	}	
};

//Class for holding Actions
class habAction extends Managed
{
	string Name;
	string Affinity; //bandit / hero / none
	string SecondaryAffinity = "none";
	float Points;
	bool NotifiyPlayer;

	void habAction(string actionName, string affinity, string secondaryAffinity, float points, bool notifiyPlayer = true) 
	{
		Name = actionName;
		Affinity = affinity;
		SecondaryAffinity =  secondaryAffinity;
		Points = points;
		NotifiyPlayer = notifiyPlayer;
	}
};


//Class for holding Actions
class habAggressorAction extends Managed
{
	string Name;
	float Amount;

	void habAggressorAction(string actionName, float amount) 
	{
		Name= actionName;
		Amount = amount;
	}
};