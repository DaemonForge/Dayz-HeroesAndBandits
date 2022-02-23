modded class MissionBaseWorld extends Mission
{
	
	void OnHaBInit(){
		
	}
	
	void NewHABAction(PlayerBase player, inout string action, inout string ActionName, EntityAI other, inout float gain, inout bool notify, inout bool ignoreLimit){
		
		
	}
	
	void NewHABKillAction(PlayerBase player, EntityAI other,inout float gain, inout bool notify, inout bool ignoreLimit){
	
	}
	
	
	void OnHABAffinityChange(PlayerBase player,int oldAffinity,int newAffinity, bool isFirst, inout bool notify){
	
	}
	
	void OnLevelChange(PlayerBase player, int oldLevel, int newLevel, bool isFirst, inout bool notify){
	
	}
	
}