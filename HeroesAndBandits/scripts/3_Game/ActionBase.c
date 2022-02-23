class HaBActionType {
	static const int EXACT = 0;
	static const int MULTIPLE = 1;
	static const int ADD = 2;
	static const int SUB = 3;
	static const int MULTIPLEADD = 4;
	static const int MULTIPLESUB = 5;
}

class HaBActionBase extends Managed {
	protected string name;
	protected string displayName;
	protected float value;
	protected bool notify
	protected int adjustmentType = HaBActionType.EXACT;
	protected int dailyLimit = -1;
	
	void HaBActionBase(string Name, string displayname, float Value, bool Notify = true, int type = HaBActionType.EXACT, int limit = -1){
		name = Name;
		displayName = displayname;
		value = Value;
		notify = Notify;
		adjustmentType = type;
		dailyLimit = limit;
	}
	
	string Name(){
		return name;
	}
	
	float Value(){
		return value;
	}
	
	bool Notify(){
		return notify;
	}
	
	string DisplayName(){
		return displayName;
	}
	
	int AdjustmentType(){
		return adjustmentType;
	}
	int DailyLimit(){
		return dailyLimit;
	}
	
	float GetGain(float curHumanity){
		switch (AdjustmentType()){
			case HaBActionType.EXACT:
				return Value();
			break;
			case HaBActionType.MULTIPLE:
				return Math.Round(Value() * curHumanity);
			break;
			case HaBActionType.ADD:
				if (curHumanity < 0){
					return Value() * -1;
				}else if (curHumanity > 0){
					return Value();
				} else {
					return 0;
				}
			break;
			case HaBActionType.SUB:
				if (curHumanity > 0){
					return Math.Round(Math.Max(Value() * -1, curHumanity * -1));
				}else if (curHumanity < 0){
					return Math.Round(Math.Min(Value(), curHumanity * -1));
				} else {
					return 0;
				}
			break;
			case HaBActionType.MULTIPLEADD:
				if (curHumanity < 0){
					return Math.Round(curHumanity * Value() * -1);
				}else if (curHumanity > 0){
					return Math.Round(curHumanity * Value());
				} else {
					return 0;
				}
			break;
			case HaBActionType.MULTIPLESUB:
				if (curHumanity > 0){
					return Math.Round(Math.Max(curHumanity * Value() * -1, curHumanity * -1));
				}else if (curHumanity < 0){
					return Math.Round(Math.Min(curHumanity * Value(), curHumanity * -1));
				} else {
					return 0;
				}
			break;
		}
		return 0;
	}
	
}