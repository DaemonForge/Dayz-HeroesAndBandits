#ifdef WRDG_DOGTAGS
modded class Dogtag_Base : ItemBase
{
	
	protected float m_HeroesAndBandits_Humanity;
	
	protected string m_HeroesAndBandits_Affinity = "Bambi";
	

	void Dogtag_Base()
	{
		RegisterNetSyncVariableFloat("m_HeroesAndBandits_Humanity");
	}


	override bool DescriptionOverride(out string output)
	{
		bool modified = super.DescriptionOverride(output);
		
		
		if (m_HeroesAndBandits_Humanity != NULL)
		{
			output += "#HAB_AFFINITY: " + m_HeroesAndBandits_Level + " \n", );
			modified = true;
		}
		
		if (m_HeroesAndBandits_Humanity != NULL)
		{
			output += string.Format("#HAB_HUMANITY: %1 \n", m_HeroesAndBandits_Humanity);
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



	override void OnRPCRead(PlayerIdentity sender, ref ParamsReadContext ctx)
	{
		super.OnRPCRead(sender, ctx);
		ctx.Read(m_HeroesAndBandits_Humanity);
		ctx.Read(m_HeroesAndBandits_Affinity);
	}

	override void OnRPCWrite(ScriptRPC rpc)
	{
		super.OnRPCWrite( rpc );
		rpc.Write(m_HeroesAndBandits_Humanity);
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