modded class PlayerBase
{
	float m_HeroesAndBandits_WarningSent = -1;
	
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
		if (GetGame().IsServer()){
			PlayerBase sourcePlayer;
			PlayerBase targetPlayer = this;
			if (killer.IsMan())	{
				sourcePlayer = PlayerBase.Cast(killer);
			} else if (killer.IsWeapon()) {
				sourcePlayer = PlayerBase.Cast(EntityAI.Cast(killer).GetHierarchyParent());
			} else if (killer.IsMeleeWeapon()) {
				sourcePlayer = PlayerBase.Cast(EntityAI.Cast(killer).GetHierarchyParent());
			}
			
			if (!sourcePlayer || !targetPlayer){ //Make sure Players are valid
			}else{
				string sourcePlayerID = sourcePlayer.GetIdentity().GetPlainId();
				string targetPlayerID = targetPlayer.GetIdentity().GetPlainId();
				if (sourcePlayerID == targetPlayerID){ //Sucide
					GetHeroesAndBandits().NewPlayerAction(sourcePlayerID, GetHeroesAndBandits().GetPlayerAffinity(sourcePlayerID)+"Sucide");
				}else {
					GetHeroesAndBandits().NewPlayerAction(sourcePlayerID, GetHeroesAndBandits().GetPlayerAffinity(sourcePlayerID)+"Vs"+GetHeroesAndBandits().GetPlayerAffinity(targetPlayerID));
				}
			}
		}
	}
}