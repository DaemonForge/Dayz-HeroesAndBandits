#ifdef EXPANSIONMOD
	class ExpansionExplosiveBase 
	{
		protected string hab_ActivatedByID;		// ID for player who placed Explosive
	    
		void habSetActivatedBy(string playerId)
	    {
			if ( hab_ThrowerId )
			{
	        	hab_ThrowerId = playerId;
			} else {
				hab_ThrowerId = "";
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
#endif