//Return an array of file names for all the files in the specified directory
TStringArray habFindFilesInDirectory(string directory) 
{ 
	
	TStringArray fileList = new TStringArray;
	
	string		fileName;
	int		fileAttr;
	int		flags;
	//Add \ to directory path and add search parameter (*)
	string pathpattern = directory + "\\*";
	
	//Search for files in file directory
	FindFileHandle fileHandler = FindFile(pathpattern, fileName, fileAttr, flags);
	
	bool found = true;
        while ( found ) //while there are files loop through looking for more
        {                
	        if ( fileAttr )
	        {
	        	//If file exsit add to array
	        	fileList.Insert(fileName);
	        }
		found = FindNextFile(fileHandler, fileName, fileAttr);
        }

	return fileList; 
}


//Always Exception Verbose Debug
static void habPrint(string message, string msgType){
	if (!GetHeroesAndBanditsSettings()){return;} //Preventing Null pointers on the client before the server has restarted
	if (msgType == "Always"){
		Print("[HeroesAndBandits]  " + message);
	}else if (msgType == "Exception" && GetHeroesAndBanditsSettings().ExceptionLogs){
		Print("[HeroesAndBandits] [Exception]  " + message);
	}else if (msgType == "Verbose" && GetHeroesAndBanditsSettings().VerboseLogs){
		Print("[HeroesAndBandits] [Verbose]  " + message);
	}else if (msgType == "Debug" && GetHeroesAndBanditsSettings().DebugLogs){
		Print("[HeroesAndBandits] [Debug]  " + message);
	}
}


PlayerBase habGetPlayerBaseByID( string pID )
{
	PlayerBase p;
	ref array<Man> players = new array<Man>;
	GetGame().GetPlayers(players);
	for ( int i = 0; i < players.Count(); i++ )
	{
		p = PlayerBase.Cast(players.Get(i));
		if ( p.GetIdentity().GetPlainId() ==  pID )
		{
			habPrint("Found Player " + p.GetIdentity().GetName() + " with id " + pID, "Debug");	
			return p;
		}
	}
	habPrint("Failed to GetPlayerBaseByID for Player " + pID , "Exception");
	return null;
}

PlayerBase habGetPlayerBaseByName( string pName )
{
	PlayerBase p;
	ref array<Man> players = new array<Man>;
	GetGame().GetPlayers(players);
	for ( int i = 0; i < players.Count(); i++ )
	{
		p = PlayerBase.Cast(players.Get(i));
		habPrint("Looking for Player " + pName + " Checking against Name " + p.GetIdentity().GetName() + " Full Name: " + p.GetIdentity().GetFullName(), "Debug");	
		if ( p.GetIdentity().GetName() ==  pName)
		{
			habPrint("Found Player " + p.GetIdentity().GetName() + " with Name " + pName, "Debug");	
			return p;
		}
	}
	habPrint("Failed to GetPlayerBaseByName for Player " + pName , "Verbose");
	return null;
}


string habUpdateStat(string statName){
	if ( statName == "BandagePlayer") {
		statName = "MedicBandagePlayer";
	}
	if (statName == "GiveBloodPlayer" ) {
		statName = "MedicGiveBlood";	
	}
	if (statName == "GiveSalinePlayer" ) {
		statName = "MedicGiveSaline";
				
	}
	if (statName == "GivePlayerCPR" ) {
		statName = "MedicGiveCPR";
	}
	return statName;
}