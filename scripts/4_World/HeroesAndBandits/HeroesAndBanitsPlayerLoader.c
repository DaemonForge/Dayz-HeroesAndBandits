static string HeroesAndBanditsPlayerDBDIR = "$profile:HeroesAndBandits\\PlayerDB";

class HeroesAndBanditsPlayer
{
	//Default Values
    string PlayerID = "";
	float Humanity = 0;
	ref array< ref habStat > Stats = new ref array< ref habStat >;
	
	void HeroesAndBanditsPlayer(string pID = "") 
	{
        PlayerID = pID;
		if (FileExist(HeroesAndBanditsPlayerDBDIR+"\\" + pID + ".json")) //If config file exsit load the file
        {
            JsonFileLoader<HeroesAndBanditsPlayer>.JsonLoadFile(HeroesAndBanditsPlayerDBDIR+"\\" + pID + ".json", this);
        }
        else //If config file doesn't exsit create the file
        {
            if (GetGame().IsServer() || !GetGame().IsMultiplayer())
            {
                JsonFileLoader<HeroesAndBanditsPlayer>.JsonSaveFile(HeroesAndBanditsPlayerDBDIR+"\\" + pID + ".json", this);
            }
        }
    }
    
	habLevel getLevel(){
		return GetHeroesAndBanditsConfig().getLevel(Humanity);
	}
	
	
	string getAffinity(){
		return GetHeroesAndBanditsConfig().getAffinity(Humanity);
	}
	
	void saveData(){
		if (GetGame().IsServer())
		{
			JsonFileLoader<HeroesAndBanditsPlayer>.JsonSaveFile(HeroesAndBanditsPlayerDBDIR + "\\" + PlayerID + ".json", this);
	    }
	}
	
	float getHumanity(){
		return Humanity;
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
		Humanity = Humanity + GetHeroesAndBanditsConfig().getActionHumanity(actionName);
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
		float newTotal = 0;
		float subTotal = 0;
		for ( int i =0; i < Stats.Count(); i++ )
		{
			subTotal = Stats.Get(i).Stat * GetHeroesAndBanditsConfig().getActionHumanity(Stats.Get(i).Name);
			newTotal = newTotal + subTotal;
		}
		Humanity = newTotal;
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