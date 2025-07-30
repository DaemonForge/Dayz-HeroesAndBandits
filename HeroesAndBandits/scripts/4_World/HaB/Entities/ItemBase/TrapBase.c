modded class TrapBase
{
	protected string hab_ActivatedByID;		// ID for player who placed Trap

	void habSetActivatedBy(string guid)
	{
		if (guid)
		{
			hab_ActivatedByID = guid;
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
	
};