modded class ZombieBase
{
	override void EEKilled(Object killer)
    {

        super.EEKilled(killer);
		if (GetGame().IsServer()){
			PlayerBase sourcePlayer;
			if (killer.IsMan())
			{
				if (killer.IsInherited(SurvivorBase))
				{
					sourcePlayer = PlayerBase.Cast(killer);
					
				}
			}else if (killer.IsWeapon())
			{
				sourcePlayer = PlayerBase.Cast(EntityAI.Cast(killer).GetHierarchyParent());
			}else if (killer.IsMeleeWeapon())
			{
				sourcePlayer = PlayerBase.Cast(EntityAI.Cast(killer).GetHierarchyParent());
			}
			
			if (!sourcePlayer){
			}else{
				string sourcePlayerID = sourcePlayer.GetIdentity().GetPlainId();
				GetHeroesAndBandits().NewPlayerAction(sourcePlayerID, "ZombieKill");
			}
		}
    }
}