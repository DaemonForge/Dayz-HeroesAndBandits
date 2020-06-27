modded class TrapBase
{
	protected string hab_ActivatedByID;		// ID for player who placed Trap

	void habSetActivatedBy(string PlayerID)
	{
		if (PlayerID)
		{
			hab_ActivatedByID = PlayerID;
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