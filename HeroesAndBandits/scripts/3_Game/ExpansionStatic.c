#ifdef EXPANSIONMOD
	modded class ExpansionStatic
	{
		override void LoadFlagTextures()
		{
			super.LoadFlagTextures();
			AddFlagTexture("HeroesAndBandits\\data\\texture\\flag_hero_co.paa", "Hero Flag");
			AddFlagTexture("HeroesAndBandits\\data\\texture\\flag_bandit_co.paa", "Bandit Flag");
		};
	};
#endif