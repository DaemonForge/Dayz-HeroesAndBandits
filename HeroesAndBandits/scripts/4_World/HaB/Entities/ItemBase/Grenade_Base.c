modded class Grenade_Base
{
    protected string hab_ActivatedByID;

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