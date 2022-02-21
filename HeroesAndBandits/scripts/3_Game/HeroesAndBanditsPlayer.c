class HeroesAndBanditsPlayerBase extends Managed
{
	protected string GUID = "";
	protected float Humanity = 0;
	protected int MinLevel = 0;
	protected int MaxLevel = 0;
	
	protected autoptr array<autoptr HeroesAndBanditsStats> m_Stats;
	//Daily Gain saved in its own objects
	[NonSerialized()]
	protected autoptr array<autoptr HeroesAndBanditsDaily> m_DailyGain;
	
	void HeroesAndBanditsPlayerBase(string guid = "") {
        GUID = guid;
		m_Stats = new array<autoptr HeroesAndBanditsStats>;
		m_DailyGain = new array<autoptr HeroesAndBanditsDaily>;
    }
	
	void UpdateHumanity(float humanity){
		Humanity = humanity;
	}
	
	void UpdateDaily(float humanity){
		
	}
	
	float GetHumanity(){
		return Humanity;
	}
	
	int UpdateLevel(int newLevel){
		if (newLevel > MaxLevel){
			MaxLevel = newLevel;
			return 1;
		}
		if (newLevel < MinLevel){
			MinLevel = newLevel;
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
	protected string m_guid;
	protected int m_DateStamp;
	protected int m_Value;
	protected autoptr array<autoptr HeroesAndBanditsStats> m_Stats;
	
	void HeroesAndBanditsDaily(string guid, int date, int value){
		m_guid = guid;
		m_DateStamp = date;
		m_Value = value;
		m_Stats = new array<autoptr HeroesAndBanditsStats>;
	}
}