static autoptr UApiDBHandler<HeroesAndBanditsPlayerBase> HABPlayerDataHandler = new UApiDBHandler<HeroesAndBanditsPlayerBase>("Humanity", OBJECT_DB);
static autoptr UApiDBHandler<HeroesAndBanditsDaily> HABDailyDataHandler = new UApiDBHandler<HeroesAndBanditsDaily>("HABDailyCount", OBJECT_DB);
class HeroesAndBanditsPlayerBase extends Managed
{
	protected string GUID = "";
	protected float Humanity = 0;
	protected int MinLevel = 0;
	protected int MaxLevel = 0;
	
	protected autoptr array<autoptr HeroesAndBanditsStats> m_Stats;
	//Daily Gain saved in its own objects
	[NonSerialized()]
	protected autoptr map<string, autoptr HeroesAndBanditsDaily> m_DailyGain;
	[NonSerialized()]
	protected int m_LastDailyCall = 0;
	
	void HeroesAndBanditsPlayerBase(string guid = "") {
        GUID = guid;
		m_Stats = new array<autoptr HeroesAndBanditsStats>;
		m_DailyGain = new map<string, autoptr HeroesAndBanditsDaily>;
		InitDailyGains();
    }
	
	void InitDailyGains(){
		int Date = UUtil.GetDateInt();
		m_LastDailyCall = HABDailyDataHandler.Query(new UApiQueryObject("{ \"GUID\": \""+ GUID +"\", \"DateStamp\": \""+ Date +"\" }"), this,"CBLoadDailyArray");
	}
	
	void CBLoadDailyArray(int cid, int status, string oid, autoptr UApiQueryResult<HeroesAndBanditsDaily> data){
		if (status == UAPI_SUCCESS){
			if (!m_DailyGain){
				m_DailyGain = new map<string, autoptr HeroesAndBanditsDaily>;
			}
			foreach (HeroesAndBanditsDaily daily : data.GetResults()){
				m_DailyGain.Set(daily.GetAction(),daily);
			}
		} else if (status == UAPI_EMPTY){
			m_DailyGain = new map<string, autoptr HeroesAndBanditsDaily>;
		}
	}
	void CBLoadDaily(int cid, int status, string oid, HeroesAndBanditsDaily data){
		if (status == UAPI_SUCCESS){
			if (!m_DailyGain){
				m_DailyGain = new map<string, autoptr HeroesAndBanditsDaily>;
			}
			m_DailyGain.Set(data.GetAction(),data);
		}
	}
	
	void UpdateHumanity(float humanity){
		Humanity = humanity;
		HABPlayerDataHandler.Update(GUID,"Humanity", humanity.ToString());
	}
		
	float GetHumanity(){
		return Humanity;
	}
	
	int UpdateLevel(int newLevel){
		if (newLevel > MaxLevel){
			MaxLevel = newLevel;
			HABPlayerDataHandler.Update(GUID,"MaxLevel",MaxLevel.ToString());
			return 1;
		}
		if (newLevel < MinLevel){
			MinLevel = newLevel;
			HABPlayerDataHandler.Update(GUID,"MinLevel",MinLevel.ToString());
			return -1;
		}
		return 0;
	}
	
	int Max(){
		return MaxLevel;
	}
	int Min(){
		return MinLevel;
	}
	
	bool IncermentAction(string action, int max){
		if (!m_DailyGain){
			m_DailyGain = new map<string, autoptr HeroesAndBanditsDaily>;
		}
		int date = UUtil.GetDateInt();
		HeroesAndBanditsDaily daily;
		int value = 1;
		if (!m_DailyGain.Find(action,daily) || daily.GetDate() != date){
			daily = new HeroesAndBanditsDaily(GUID, action, date, value);
			HABDailyDataHandler.Save(daily.OID(), daily, this, "CBLoadDaily");
			return (value <= max || max == -1);
		}
		value = daily.Increment();
		HABDailyDataHandler.Transaction(daily.OID(), "Value", 1);
		Print("[HAB] Incerment Action: " + action + " Max: " + max + " Value: " + value);
		return (value <= max || max == -1);
	}
}

class HeroesAndBanditsStats extends Managed {
	string m_Stat;
	int m_Value;
	void HeroesAndBanditsStats(string stat, int value){
		m_Stat = stat;
		m_Value = value;
	}
}

class HeroesAndBanditsDaily extends Managed {
	protected string ObjectId = "NewObject";
	protected string GUID;
	protected string Action;
	protected int DateStamp;
	protected int Value;
	
	void HeroesAndBanditsDaily(string guid, string action, int date, int value){
		GUID = guid;
		Action = action;
		DateStamp = date;
		Value = value;
	}
	
	string OID(){
		return ObjectId;
	}
	
	int Increment(){
		return ++Value;
	}
	
	int GetValue(){
		return Value;
	}
	
	int GetDate(){
		return DateStamp;
	}
	
	string GetAction(){
		return Action;
	}
}