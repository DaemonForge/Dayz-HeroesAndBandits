#ifdef EXPANSIONMOD
	modded class ExpansionExplosive extends Inventory_Base
	{
		protected string hab_ActivatedByID;		// ID for player who placed Explosive
	    
		void habSetActivatedBy(string playerId)
	    {
			if ( playerId )
			{
	        	hab_ActivatedByID = playerId;
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
	
		override void TriggerExplosion()
		{
			super.Trigger();
			if (hab_ActivatedByID)
			{
				ref array<Object> objects = new array<Object>;
				ref array<CargoBase> proxy = new array<CargoBase>;
				GetGame().GetObjectsAtPosition(this.GetPosition(), 1, objects, proxy);
				for ( int i = 0; i < objects.Count(); i++ ) 
				{
					Object obj = objects.Get(i);
					if (obj.IsInherited(Expansion_C4_Explosion))
					{
						habPrint( "Found C4 Explosion setting ID to " + hab_ActivatedByID , "Debug");
						Expansion_C4_Explosion explosive = Expansion_C4_Explosion.Cast(obj);
						if ( explosive ){
							explosive.habSetActivatedBy(hab_ActivatedByID);
						}
						return;
					}
				}
			}
		}
	};


	modded class Expansion_C4_Explosion extends BuildingBase
	{
		protected string hab_ActivatedByID;		// ID for player who placed Explosive
	    
		void habSetActivatedBy(string playerId)
	    {
			if ( playerId )
			{
	        	hab_ActivatedByID = playerId;
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
#endif