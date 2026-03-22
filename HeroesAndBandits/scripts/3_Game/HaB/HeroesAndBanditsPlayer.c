
static autoptr UDBHandler<HeroesAndBanditsPlayerBase> HABPlayerDataHandler = new UDBHandler<HeroesAndBanditsPlayerBase>("Humanity", OBJECT_DB);
static autoptr UDBHandler<HeroesAndBanditsDaily> HABDailyDataHandler = new UDBHandler<HeroesAndBanditsDaily>("HABDailyCount", OBJECT_DB);
class HeroesAndBanditsPlayerBase extends Managed
{
	protected string GUID = "";
	protected string Name = "";
	protected float Humanity = 0;
	protected int MinLevel = 0;
	protected int MaxLevel = 0;
	
	protected autoptr array<autoptr HeroesAndBanditsStats> Stats;
	//Daily Gain saved in its own objects
	[NonSerialized()]
	protected autoptr map<string, int> m_Stats;
	
	[NonSerialized()]
	protected autoptr map<string, autoptr HeroesAndBanditsDaily> m_DailyGain;
	[NonSerialized()]
	protected int m_LastDailyCall = 0;
	
	void HeroesAndBanditsPlayerBase(string guid = "") {
        GUID = guid;
		m_Stats = new map<string, int>;
		m_DailyGain = new map<string, autoptr HeroesAndBanditsDaily>;
		// Don't call InitDailyGains here - it will be called after data is loaded from DB
    }
	
	void ~HeroesAndBanditsPlayerBase(){
		if(Stats) delete Stats;
		if(m_Stats) delete m_Stats; 
		if(m_DailyGain) delete m_DailyGain;
	}
		
	void InitDailyGains(){
		// Guard against client-side calls
		if (g_Game.IsClient()){
			return;
		}
		
		// Guard against offline UFramework - check both UF() and IsOnline()
		if (!UF() || !UF().IsOnline()){
			Print("[HaB] [Warn] InitDailyGains skipped - Universal Framework is offline");
			return;
		}
		
		// Guard against empty GUID
		if (GUID == ""){
			Print("[HaB] [Warn] InitDailyGains skipped - GUID is empty");
			return;
		}
		
		int Date = UUtil.GetDateInt();
		m_LastDailyCall = HABDailyDataHandler.Query(new UDBQuery("{ \"GUID\": \""+ GUID +"\", \"DateStamp\": "+ Date +" }"), this,"CBLoadDailyArray");
		
		// Initialize stats map from stored Stats array
		if (!m_Stats){
			m_Stats = new map<string, int>;
		}
		if (!Stats) return; //Stats not set yet
		for (int i = 0; i< Stats.Count(); i++){
			HeroesAndBanditsStats stat = HeroesAndBanditsStats.Cast(Stats.Get(i));
			if (stat){
				m_Stats.Set(stat.m_Stat, stat.m_Value);
			}
		}
	}
	
	void CBLoadDailyArray(int cid, int status, string oid, UDBQueryResult<HeroesAndBanditsDaily> data){
		if (status == UF_SUCCESS){
			if (!m_DailyGain){
				m_DailyGain = new map<string, autoptr HeroesAndBanditsDaily>;
			}
			array<autoptr HeroesAndBanditsDaily> dataarray;
			Class.CastTo(dataarray,data.GetResults());
			Print("[UF] LoadDailyArray found " + data.Count() + " Daily count objects");
			for (int i = 0; i< dataarray.Count(); i++){
				HeroesAndBanditsDaily daily = HeroesAndBanditsDaily.Cast(dataarray.Get(i));
				m_DailyGain.Set(daily.GetAction(), daily);
			}
		} else if (status == UF_EMPTY){
			Print("[UF] LoadDailyArray found zero Daily count objects");
			m_DailyGain = new map<string, autoptr HeroesAndBanditsDaily>;
		}
		Print("[UF] CBLoadDailyArray Completed");
	}
	void CBLoadDaily(int cid, int status, string oid, HeroesAndBanditsDaily data){
		if (status == UF_SUCCESS){
			if (!m_DailyGain){
				m_DailyGain = new map<string, autoptr HeroesAndBanditsDaily>;
			}
			m_DailyGain.Set(data.GetAction(),data);
		}
	}
	
	void UpdateHumanity(float humanity){
		Humanity = humanity;
		// Guard against offline UFramework
		if (!UF() || !UF().IsOnline()){
			Print("[HaB] [Warn] UpdateHumanity skipped DB write - UFramework offline");
			return;
		}
		HABPlayerDataHandler.Update(GUID,"Humanity", humanity.ToString());
	}
		
	float GetHumanity(){
		return Humanity;
	}
	
	string PlayerName(){
		return Name;
	}
	
	string GetGUID(){
		return GUID;
	}
	
	void UpdateName(string name){
		if (Name != name){
			// Guard against offline UFramework
			if (UF() && UF().IsOnline()){
				HABPlayerDataHandler.Update(GUID,"Name",name);
			} else {
				Print("[HaB] [Warn] UpdateName skipped DB write - UFramework offline");
			}
			Name = name;
		}
	}
	
	int UpdateLevel(int newLevel){
		// Guard against offline UFramework
		bool uOnline = (UF()) && UF().IsOnline();
		if (newLevel > MaxLevel){
			MaxLevel = newLevel;
			if (uOnline){
				HABPlayerDataHandler.Update(GUID,"MaxLevel",MaxLevel.ToString());
			} else {
				Print("[HaB] [Warn] UpdateLevel MaxLevel skipped DB write - UFramework offline");
			}
			return 1;
		}
		if (newLevel < MinLevel){
			MinLevel = newLevel;
			if (uOnline){
				HABPlayerDataHandler.Update(GUID,"MinLevel",MinLevel.ToString());
			} else {
				Print("[HaB] [Warn] UpdateLevel MinLevel skipped DB write - UFramework offline");
			}
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
	
	bool IncermentAction(string action, int max, bool doStat = true){
		// Guard against offline UFramework
		bool uOnline = (UF()) && UF().IsOnline();
		if (!uOnline){
			Print("[HaB] [Warn] IncermentAction skipped - UFramework offline");
			return true; // Allow action when offline (fail-open)
		}
		
		if (!m_DailyGain){
			m_DailyGain = new map<string, autoptr HeroesAndBanditsDaily>;
		}
		if (!m_Stats){
			m_Stats = new map<string, int>;
		}
		if (!Stats){
			Stats = new array<autoptr HeroesAndBanditsStats>;
		}
		
		if (doStat){
			int stat = 1;
			if (!m_Stats.Find(action,stat)){
				HeroesAndBanditsStats tstat = new HeroesAndBanditsStats(action,1);
				Stats.Insert(tstat);
				HABPlayerDataHandler.Update(GUID, "Stats", tstat.ToJson(),UpdateOpts.PUSH);
			} else {
				stat++;
				HABPlayerDataHandler.QueryUpdate(new UDBQuery("{\"GUID\": \"" + GUID + "\", \"Stats.m_Stat\": \"" + action + "\"}"),"Stats.$.m_Value", stat.ToString());
			}
			m_Stats.Set(action,stat);
		}
		int date = UUtil.GetDateInt();
		HeroesAndBanditsDaily daily;
		int value = 1;
		if (!m_DailyGain.Find(action,daily) || daily.GetDate() != date){
			daily = new HeroesAndBanditsDaily(GUID, action, date, value);
			HABDailyDataHandler.Save(daily.OID(), daily, this, "CBLoadDaily");
			Print("[HAB] Incerment Action: " + action + " Max: " + max + " Value: " + value + " New Daily Limit Created");
			m_DailyGain.Set(action,daily);
			return (value <= max || max == -1);
		}
		value = daily.Increment();
		if (daily.OID() != "NewObject")
			HABDailyDataHandler.Transaction(daily.OID(), "Value", 1);
		Print("[HAB] Incerment Action: " + action + " Max: " + max + " Value: " + value + " Daily Limit OID: " + daily.OID());
		return (value <= max || max == -1);
	}
	
	bool GetStat(string action, out int stat){
		return m_Stats.Find(action,stat);
	}
	
	array<autoptr HeroesAndBanditsStats> GetAllStats(){
		return Stats;
	}
}

class HeroesAndBanditsStats extends Managed {
	string m_Stat;
	int m_Value; 
	
	void HeroesAndBanditsStats(string stat, int value){
		m_Stat = stat;
		m_Value = value;
	}
	
	
	
	string ToJson(){
		string jsonString = UJSONHandler<HeroesAndBanditsStats>.ToString(this);
		return jsonString;
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