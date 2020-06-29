class HeroesAndBanditsPlayer
{
	//Default Values
    string PlayerID = "";
	float Humanity = 0; //Legacy Will remove
	ref array< ref habStat > Stats = new ref array< ref habStat >;
	ref array< ref habPlayerAffinity > Affinities = new ref array< ref habPlayerAffinity >;
	
	void HeroesAndBanditsPlayer(string pID = "") 
	{
        PlayerID = pID;
		if (FileExist(HeroesAndBanditsPlayerDB+"\\" + pID + ".json")) //If config file exsit load the file
        {
            JsonFileLoader<HeroesAndBanditsPlayer>.JsonLoadFile(HeroesAndBanditsPlayerDB+"\\" + pID + ".json", this);
        }
        else //If config file doesn't exsit create the file
        {
            if (GetGame().IsServer() || !GetGame().IsMultiplayer())
            {
                JsonFileLoader<HeroesAndBanditsPlayer>.JsonSaveFile(HeroesAndBanditsPlayerDB+"\\" + pID + ".json", this);
            }
        }
    }
    
	habLevel getLevel(){
		habLevel tempLevel = GetHeroesAndBanditsLevels().DefaultLevel;
		float points = 0;
		if (GetHeroesAndBanditsSettings().Mode == 0){
			float humanity = getHumanity();
			if ( humanity > 0 ){
				tempLevel = getLevel("hero", humanity);	
			} else if ( humanity < 0 ) {
				tempLevel = getLevel("bandit", -humanity);	
			} 
		} else if (GetHeroesAndBanditsSettings().Mode == 1){
			for (int i = 0; i < Affinities.Count(); i++)
			{
				if (getLevel(Affinities.Get(i).Name, Affinities.Get(i).Points) != GetHeroesAndBanditsLevels().DefaultLevel){
					if (Affinities.Get(i).Points > points){
						points = Affinities.Get(i).Points;
						tempLevel = getLevel(Affinities.Get(i).Name, Affinities.Get(i).Points);
					}
				}
			}
		}
		return tempLevel;
	}
	
	
	habAffinity getAffinity(){
		habAffinity tempAffinity = GetHeroesAndBanditsLevels().DefaultAffinity;
		habLevel tempLevel;
		float points = 0;
		if (getLevel() != GetHeroesAndBanditsLevels().DefaultLevel){
			tempAffinity = GetHeroesAndBanditsLevels().getAffinity(getLevel().Affinity);
		}
		return tempAffinity;
	}
	
	float getAffinityPoints( string name ){
		for (int i = 0; i < Affinities.Count(); i++)
		{
			if (Affinities.Get(i).Name == name) {
				return Affinities.Get(i).getPoints();
			}
		}
		return 0;
	}
	
	void addAffinityPoints( string name, float points ){
		bool found = false;
		for (int i = 0; i < Affinities.Count(); i++)
		{
			if (Affinities.Get(i).Name == name) {
				Affinities.Get(i).updatePoints(points);
				found = true;
			}
		}
			
		if (!found && doesAffinityExsit(name)){
			 Affinities.Insert(new ref habPlayerAffinity(name, points));
		}
	}
	
	string getAffinityName(){
		return getAffinity().Name;
	}
		
	void saveData(){
		if (GetGame().IsServer())
		{
			JsonFileLoader<HeroesAndBanditsPlayer>.JsonSaveFile(HeroesAndBanditsPlayerDB + "\\" + PlayerID + ".json", this);
	    }
	}
	
	float getHumanity(){
		return getAffinityPoints("hero") - getAffinityPoints("bandit");
	}

	float getStat(string statName){
		//Print("[HeroesAndBandits][DebugClient] Looking for Stat: " + statName );
		float statTotal = 0;
		string prefix;
		int nameLength;
		if ( statName == "Kill" ){
			for ( int h =0; h < Stats.Count(); h++ )
			{
				if (Stats.Get(h).Name == "heroVshero" || Stats.Get(h).Name == "heroVsbambi" || Stats.Get(h).Name == "heroVsbandit" || Stats.Get(h).Name == "banditVshero" || Stats.Get(h).Name == "banditVsbambi" || Stats.Get(h).Name == "bambiVsbandit" || Stats.Get(h).Name == "bambiVshero" || Stats.Get(h).Name == "bambiVsbambi" || Stats.Get(h).Name == "bambiVshero"){
					statTotal = statTotal + Stats.Get(h).Stat;
				}
			}
			return statTotal;
		} else if ( statName == "Hunt" ) {
			for ( int j =0; j < Stats.Count(); j++ )
			{
				prefix = Stats.Get(j).Name.Substring(0,4);
				//Print("[HeroesAndBandits][DebugClient] Looking for Stat: " + statName + " comparing to " + Stats.Get(j).Name + " Prefix is " + prefix );
				if (prefix == "Hunt"){
					statTotal = statTotal + Stats.Get(j).Stat;
				}
			}
			return statTotal;
		} else if ( statName == "Medic" ){
			for ( int k =0; k < Stats.Count(); k++ )
			{
				if ( Stats.Get(k).Name == "BandagePlayer" || Stats.Get(k).Name == "GiveBloodPlayer" || Stats.Get(k).Name == "GiveSalinePlayer" || Stats.Get(k).Name == "GivePlayerCPR" ){
					statTotal = statTotal + Stats.Get(k).Stat;
				}
			}
			return statTotal;
		} else if ( statName == "Raid" ) {
			for ( int l =0; l < Stats.Count(); l++ )
			{
				string tempStatName = Stats.Get(l).Name;
				nameLength = tempStatName.Length();
				nameLength = nameLength - 4;
				prefix = Stats.Get(l).Name.Substring(nameLength, 4);
				//Print("[HeroesAndBandits][DebugClient] Looking for Stat: " + statName + " comparing to " + Stats.Get(j).Name + " Prefix is " + prefix );
				if ( prefix == "Raid" ){
					statTotal = statTotal + Stats.Get(l).Stat;
				}
			}
			return statTotal;
		} else if ( statName == "Mission" ) {
			for ( int m =0; m < Stats.Count(); m++ )
			{
				prefix = Stats.Get(m).Name.Substring(0, 3);;
				
				//Print("[HeroesAndBandits][DebugClient] Looking for Stat: " + statName + " comparing to " + Stats.Get(j).Name + " Prefix is " + prefix );
				if ( prefix == "SMM" ){
					statTotal = statTotal + Stats.Get(m).Stat;
				}
			}
			return statTotal;
		}  else if ( statName == "Sucide" ) {
			for ( int n =0; n < Stats.Count(); n++ )
			{
				if ( Stats.Get(n).Name == "heroSucide" || Stats.Get(n).Name == "banditSucide" || Stats.Get(n).Name == "bambiSucide" ){
					statTotal = statTotal + Stats.Get(n).Stat;
				}
			}
			return statTotal;
		} 
		
		for ( int i =0; i < Stats.Count(); i++ )
		{
			if (Stats.Get(i).Name == statName){
				return Stats.Get(i).Stat;
			}
		}
		habPrint("Stat: " + statName + " for player " + PlayerID + " not found", "Verbose");
		return 0;
	}
	
	// Will return true if player leveled up from the action
	bool NewAction(string actionName){
		int statID = -1;
		habPrint("Player: " + PlayerID + " performed Action: " + actionName, "Verbose");
		for ( int i =0; i < Stats.Count(); i++ )
		{
			if (Stats.Get(i).Name == actionName){
				statID = i;
			}
		}
		if (statID == -1){//Stat Not found so adding it
			Stats.Insert(new ref habStat(actionName, 1));
			habPrint("Stat: " + actionName + " for player " + PlayerID + " Created", "Debug");
		}else
		{
			Stats.Get(statID).updateStat();
			habPrint("Stat: " + actionName + " for player " + PlayerID + " Updated to " + Stats.Get(statID).Stat, "Debug");
		}
		string oldLevel = getLevel().Name;
		habAction tempAction GetHeroesAndBanditsActions().getAction(actionName);
		if ( tempAction.Affinity != "none" ){
			addAffinityPoints(tempAction.Affinity, tempAction.Points);
		}
		if (( tempAction.SecondaryAffinity != "none" )){
			addAffinityPoints(tempAction.SecondaryAffinity, tempAction.Points);
		}
		if (oldLevel == getLevel().Name){ //Check if Level Changed
			return false;
		} else
		{
			habPrint( "Player: " + PlayerID + " Leveled Changed from " + oldLevel + " to " + getLevel().Name, "Verbose");
			return true;
		}
		
	}
	
	void recalculateTotals()
	{
		if (Affinities){
			for (int i = 0; i < Affinities.Count(); i++)
			{
				Affinities.Get(i).setPoints(0);
			}
		}
		float subTotal = 0;
		for ( int i =0; i < Stats.Count(); i++ )
		{
			habAction tempAction GetHeroesAndBanditsActions().getAction(Stats.Get(i).Name);
			subTotal = Stats.Get(i).Stat * tempAction.Points;
			if ( tempAction.Affinity != "none" ){
				addAffinityPoints(tempAction.Affinity, subTotal);
			}
			if (( tempAction.SecondaryAffinity != "none" )){
				addAffinityPoints(tempAction.SecondaryAffinity, subTotal);
			}
		}
	}
	
	
}

class habStat
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
}


class habPlayerAffinity
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
}