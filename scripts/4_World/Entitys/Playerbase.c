modded class PlayerBase
{
	float m_HeroesAndBandits_WarningSent = -1; //For Zones
	
	
	override void Init()
	{
		super.Init();
		
		RegisterNetSyncVariableFloat("m_HeroesAndBandits_WarningSent");
		
	}
	
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
		if (GetGame().IsServer()){
			PlayerBase sourcePlayer;
			PlayerBase targetPlayer = this;
			string weaponName = "";
			if (killer.IsMan())	{
				sourcePlayer = PlayerBase.Cast(killer);
				weaponName = "#HAB_KILLFEED_FISTS";
			} else if (killer.IsWeapon()) {
				sourcePlayer = PlayerBase.Cast(EntityAI.Cast(killer).GetHierarchyParent());
				weaponName = "#HAB_KILLFEED_PRE "+ killer.GetDisplayName();
			} else if (killer.IsMeleeWeapon()) {
				sourcePlayer = PlayerBase.Cast(EntityAI.Cast(killer).GetHierarchyParent());
				weaponName = "#HAB_KILLFEED_PRE "+ killer.GetDisplayName();
			} else if (killer.IsTransport()){
				CarScript vehicle;
				if (Class.CastTo(vehicle, killer))
				{
					weaponName = "#HAB_KILLFEED_PRE "+ vehicle.GetDisplayName();
					if ( vehicle.CrewSize() > 0 )
					{
						PlayerBase driver = PlayerBase.Cast(vehicle.CrewMember( 0 ));
						if ( driver ){
							sourcePlayer = PlayerBase.Cast(driver);
						}
					}
				}
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
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		
		PlayerBase targetPlayer = this;
		if ( targetPlayer && source && damageType)		
		{
			switch ( damageType )
			{
				case DT_EXPLOSION:		
                    if (!targetPlayer.IsAlive())
                    {
						habPrint("Player " + targetPlayer.GetIdentity().GetPlainId()+ " Killed by Explosion Damage " + source.GetType(), "Debug");	
                    }
					break;
				case DT_CUSTOM:			
								
					if ( ammo == "BearTrap" || source.GetType() == "BearTrap" )			
					{
                        if (!targetPlayer.IsAlive())
                        {    
							habPrint("Player " + targetPlayer.GetIdentity().GetPlainId()+ " Killed by BearTrap Damage " + source.GetType(), "Debug");	
						}
					}
					break;
											
				default:
				
					break;
			}
		}
	}
}