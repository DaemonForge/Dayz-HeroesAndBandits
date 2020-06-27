modded class TrapBase
{
	protected string hab_ActivatedByID;		// ID for player who placed Trap

	void habSetActivatedBy(string playerID)
	{
		if (playerID)
		{
			hab_ActivatedByID = playerID;
		}
	}
	
	string habGetActivatedBy()
	{
		if (hab_ActivatedByID){
			return hab_ActivatedByID;
		} else {
			return "null";
		}
	}
	
}