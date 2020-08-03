modded class Object {
	protected bool habAIShootThrough = false;
	
	void habSetAIShootThrough(bool setTo){
		habAIShootThrough = setTo;
	}
	
	bool habCanAIShootThrough(){
		if ( IsBush() ){
			return true;
		}
		return habAIShootThrough;
	}
}