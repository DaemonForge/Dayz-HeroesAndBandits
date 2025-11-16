#ifdef EXPANSIONMOD
	modded class ExpansionBaseBuilding extends BaseBuildingBase
	{
		override void EEKilled( Object killer )
		{
			if ( g_Game.IsServer() ){
				if ( habLastHitBy ){
					if ( habLastHitBy != "null")
					{
						habPrint("Player with ID " + habLastHitBy + " Destoryed " + GetType(), "Debug");
						GetHeroesAndBandits().NewPlayerAction(habLastHitBy, GetType()+"Raid");
					}
				} else {
					habPrint("Destroyed Item " + GetType() , "Debug");
				}
			}
			super.EEKilled( killer );
		}
	};
#endif