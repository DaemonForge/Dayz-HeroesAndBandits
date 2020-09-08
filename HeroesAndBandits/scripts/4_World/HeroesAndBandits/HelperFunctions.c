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
};

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
};