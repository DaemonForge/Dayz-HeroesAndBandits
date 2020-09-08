ref HeroesAndBanditsSettings m_HeroesAndBanditsSettings;

ref HeroesAndBanditsConfigZones m_HeroesAndBanditsConfigZones;
ref HeroesAndBanditsConfigActions m_HeroesAndBanditsConfigActions;
ref HeroesAndBanditsConfigLevels m_HeroesAndBanditsConfigLevels;

ref HeroesAndBanditsSettings 		g_HeroesAndBanditsSettings;
ref HeroesAndBanditsConfigLevels 	g_HeroesAndBanditsConfigLevels;
ref HeroesAndBanditsConfigActions 	g_HeroesAndBanditsConfigActions;

//Helper function to return Config
static ref HeroesAndBanditsSettings GetHeroesAndBanditsSettings()
{
	if ( GetGame().IsServer()){
		if (!m_HeroesAndBanditsSettings)
		{
			m_HeroesAndBanditsSettings = new HeroesAndBanditsSettings;
			m_HeroesAndBanditsSettings.Load();
		}
		return m_HeroesAndBanditsSettings;
	} else {
		
		return g_HeroesAndBanditsSettings;
	}
};


//Helper function to return Config
static ref HeroesAndBanditsConfigLevels GetHeroesAndBanditsLevels()
{
	if ( GetGame().IsServer()){
		if (!m_HeroesAndBanditsConfigLevels)
		{
			m_HeroesAndBanditsConfigLevels = new HeroesAndBanditsConfigLevels;
			m_HeroesAndBanditsConfigLevels.Load();
		}
		return m_HeroesAndBanditsConfigLevels;
	} else {
		return g_HeroesAndBanditsConfigLevels;
	}
};



//Helper function to return Config
static ref HeroesAndBanditsConfigActions GetHeroesAndBanditsActions()
{
	if ( GetGame().IsServer()){
		if (!m_HeroesAndBanditsConfigActions)
		{
			m_HeroesAndBanditsConfigActions = new HeroesAndBanditsConfigActions;
			m_HeroesAndBanditsConfigActions.Load();
		}
		return m_HeroesAndBanditsConfigActions;
	} else {
		return g_HeroesAndBanditsConfigActions;
	}
};


//Helper function to return Config
static ref HeroesAndBanditsConfigZones GetHeroesAndBanditsZones()
{
	if (!m_HeroesAndBanditsConfigZones && GetGame().IsServer())
	{
		m_HeroesAndBanditsConfigZones = new HeroesAndBanditsConfigZones;
		m_HeroesAndBanditsConfigZones.Load();
	}
	return m_HeroesAndBanditsConfigZones;
};

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
};


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
};