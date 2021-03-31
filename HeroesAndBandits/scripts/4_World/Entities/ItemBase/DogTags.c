#ifdef WRDG_DOGTAGS
modded class Dogtag_Base : ItemBase
{
	
	protected float m_HeroesAndBandits_Humanity = 0;
	
	protected string m_HeroesAndBandits_Affinity = "#STR_HAB_BAMBI";
	

	void Dogtag_Base()
	{
		RegisterNetSyncVariableFloat("m_HeroesAndBandits_Humanity");
	}


	override bool DescriptionOverride(out string output)
	{
		bool modified = super.DescriptionOverride(output);
		
		
		if ( GetHeroesAndBanditsSettings().DogTags_EnableAffinityOnDogTags && m_HeroesAndBandits_Affinity )
		{
			output += "#STR_HAB_AFFINITY: " + m_HeroesAndBandits_Affinity + " \n";
			modified = true;
		}
		
		if ( GetHeroesAndBanditsSettings().DogTags_EnableHumanityOnDogTags )
		{
			output += string.Format("#STR_HAB_HUMANITY: %1 \n", m_HeroesAndBandits_Humanity);
			modified = true;
		}

		return modified;
	}


	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);

		ctx.Write(m_HeroesAndBandits_Humanity);
		ctx.Write(m_HeroesAndBandits_Affinity);
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (!super.OnStoreLoad(ctx, version))
			return false;

		if (!ctx.Read(m_HeroesAndBandits_Humanity))
			return false;
			
		if (!ctx.Read(m_HeroesAndBandits_Affinity))
			return false;

		return true;
	}



	override void OnRPCRead(PlayerIdentity sender, ParamsReadContext ctx)
	{
		super.OnRPCRead(sender, ctx);
		ctx.Read(m_HeroesAndBandits_Affinity);
	}

	override void OnRPCWrite(ScriptRPC rpc)
	{
		super.OnRPCWrite( rpc );
		rpc.Write(m_HeroesAndBandits_Affinity);
	}



	void SetHaBHumanity(float humanity)
	{
		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			m_HeroesAndBandits_Humanity = humanity;
			SetSynchDirty();
		}
	}

	void SetHaBAffinity(string affinity)
	{
		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
			m_HeroesAndBandits_Affinity = affinity;
	}


	string GetHaBAffinity()
	{
		return m_HeroesAndBandits_Affinity;
	}

	float GetHaBHumanity()
	{
		return m_HeroesAndBandits_Humanity;
	}

}
#endif