modded class PlayerBase
{
	float m_HeroesAndBandits_WarningSent = -1; //For Zones
	
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
		if (GetGame().IsServer()){
			PlayerBase sourcePlayer;
			PlayerBase targetPlayer = this;
			string weaponName = "";
			if (killer.IsMan())	{
				sourcePlayer = PlayerBase.Cast(killer);
				weaponName = "their fists";
			} else if (killer.IsWeapon()) {
				sourcePlayer = PlayerBase.Cast(EntityAI.Cast(killer).GetHierarchyParent());
				weaponName = "a "+ killer.GetDisplayName();
			} else if (killer.IsMeleeWeapon()) {
				sourcePlayer = PlayerBase.Cast(EntityAI.Cast(killer).GetHierarchyParent());
				weaponName = "a "+ killer.GetDisplayName();
			} else {
				return;
			}
			
			if (!sourcePlayer || !targetPlayer){ //Make sure Players are valid
			}else{
				string sourcePlayerID = sourcePlayer.GetIdentity().GetPlainId();
				string targetPlayerID = targetPlayer.GetIdentity().GetPlainId();
				if (sourcePlayerID == targetPlayerID){ //Sucide
					GetHeroesAndBandits().NewPlayerAction(sourcePlayerID, GetHeroesAndBandits().GetPlayerAffinity(sourcePlayerID)+"Sucide");
					GetHeroesAndBandits().TriggerSucideFeed(sourcePlayerID);
					
				}else {
					GetHeroesAndBandits().NewPlayerAction(sourcePlayerID, GetHeroesAndBandits().GetPlayerAffinity(sourcePlayerID)+"Vs"+GetHeroesAndBandits().GetPlayerAffinity(targetPlayerID));
					GetHeroesAndBandits().TriggerKillFeed(sourcePlayerID, targetPlayerID, weaponName);
				}
			}
		}
	}
}