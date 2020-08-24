modded class AnimalBase
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
			} else if (killer.IsTransport()){
				CarScript vehicle;
				if (Class.CastTo(vehicle, killer))
				{
					if ( vehicle.CrewSize() > 0 )
					{
						sourcePlayer = PlayerBase.Cast(vehicle.CrewMember( 0 ));
					}
				}
			} else {
				return;
			}
			
			if (!sourcePlayer){
			}else{
				string actionName = "Hunt" + GetType();
				habPrint("Player Killed Animal of type" + GetType(), "Debug");
				string sourcePlayerID = sourcePlayer.GetIdentity().GetPlainId();
				GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLaterByName(GetHeroesAndBandits(), "NewPlayerAction", 1, false, new Param2<string, string>(sourcePlayerID, actionName));
			}
		}
    }
};