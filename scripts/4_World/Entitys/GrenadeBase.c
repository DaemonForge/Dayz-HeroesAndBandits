modded class Grenade_Base
{
    protected string hab_ThrowerId;

    void habSetThrower(string playerId)
    {
		if ( hab_ThrowerId )
		{
        	hab_ThrowerId = playerId;
		} else {
			hab_ThrowerId = "";
		}
    }

    string habGetThrowerID() 
	{ 
		if ( hab_ThrowerId )
		{
			return hab_ThrowerId; 
		} else {
			return "null";
		}
	}
}