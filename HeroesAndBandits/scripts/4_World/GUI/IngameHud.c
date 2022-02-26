autoptr HABStatusBarIconWidget m_HABStatusBarIconWidget;
class HABStatusNotification extends ScriptedWidgetEventHandler {
	
	static string m_LayoutPath = "HeroesAndBandits/gui/layouts/HumanityNotificationText.layout";
	

	protected Widget m_LayoutRoot;
	protected TextWidget m_Text;
	protected float m_y;
	protected float m_a;
	protected float m_time;
	protected static float y_translation = 69;
	protected static float a_translation = 0.36;
	
	void HABStatusNotification(string text, Widget parent){
		m_LayoutRoot = GetGame().GetWorkspace().CreateWidgets(m_LayoutPath, parent, true);
		m_Text = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("Text"));
		m_a = 1;
		m_y = 0;
		m_Text.SetText(text);
	}
	
	bool HABOnUpdate(float timeslice){
		m_time+= timeslice;
		if (m_time > 0.035){		
			m_y = Math.Min(255, m_y + (y_translation * m_time));
			m_a = Math.Max(0, m_a - (a_translation * m_time));
			m_time = 0;
			m_LayoutRoot.SetPos(0,m_y);
			m_Text.SetAlpha(m_a);
		}
		return (m_a <= 0);
	}
}
class HABStatusBarIconWidget extends ScriptedWidgetEventHandler {
	
	static string 				m_LayoutPath = "HeroesAndBandits/gui/layouts/StatusBar.layout";
	
	protected Widget m_LayoutRoot;
	protected ImageWidget m_Icon;
	protected Widget m_Notifications;
	
	protected string m_LastIcon = "";
	
	protected autoptr array<autoptr HABStatusNotification> m_StatusNotifications;
	
	void HABStatusBarIconWidget(){
		Init();
	}
	void Init(){
		m_LayoutRoot = GetGame().GetWorkspace().CreateWidgets(m_LayoutPath, NULL, true);
		m_Icon = ImageWidget.Cast(m_LayoutRoot.FindAnyWidget("Icon"));
		m_Notifications = Widget.Cast(m_LayoutRoot.FindAnyWidget("Notifications"));
		m_StatusNotifications = new array<autoptr HABStatusNotification>;
	}
	
	bool IsInit(){
		return (m_LayoutRoot && m_Icon);
	}
	void ~HABStatusBarIconWidget(){
		Destroy();
	}
	
	void Destroy(){
		if (!m_LayoutRoot) return;
		m_LayoutRoot.Show(false);
		delete m_LayoutRoot;
	}
	
	void HABOnUpdate(float timeslice, bool hide){
		if (!GetGame().IsClient() || !IsInit()){
			return;
		}
		if (!GetGame().GetPlayer()){
			m_LayoutRoot.Show(false);
			return;
		}
		TIntArray toremove = new TIntArray;
		int i = 0;
		for (i = 0; i < m_StatusNotifications.Count();i++){
			if (m_StatusNotifications.Get(i).HABOnUpdate(timeslice)){
				toremove.Insert(i);
			}
		}
		if (toremove.Count() > 0){
			int max = toremove.Count() - 1;
			for (i = max; i >= 0; i--){
				m_StatusNotifications.RemoveOrdered(i);
			}
		}
		HideImage((hide || GetGame().GetPlayer().IsUnconscious() || !GetGame().GetPlayer().IsAlive()));
		string newIcon = PlayerBase.Cast(GetGame().GetPlayer()).GetClientIcon();
		if (m_LastIcon  != newIcon){
			UpdateImage(newIcon);
		}
	}
	
	void HideImage(bool hide){
		if (hide){
			m_LayoutRoot.Show(false);
			return;
		}
		m_LayoutRoot.Show(true);
	}
	
	void UpdateImage(string icon){
		m_LastIcon = icon;
		if ( icon == "" ){
			HideImage(true);
			return;
		}
		m_Icon.LoadImageFile(0, icon);
	}
	
	void CreateNotification(string text){
		m_StatusNotifications.Insert(new HABStatusNotification(text,m_Notifications));
	}
}