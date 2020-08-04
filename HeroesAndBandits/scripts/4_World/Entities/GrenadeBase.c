modded class Grenade_Base extends InventoryItemSuper
{
    protected string hab_ActivatedByID;

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