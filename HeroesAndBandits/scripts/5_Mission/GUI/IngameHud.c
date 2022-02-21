class HABStatusBarIconWidget extends ScriptedWidgetEventHandler {
	
	static string 				m_LayoutPath = "HeroesAndBandits/gui/layouts/StatusBar.layout";
	
	protected Widget m_LayoutRoot;
	protected ImageWidget m_Icon;
	
	void Init(){
		m_LayoutRoot = GetGame().GetWorkspace().CreateWidgets(m_LayoutPath, NULL, true);
		m_Icon = ImageWidget.Cast(m_LayoutRoot.FindAnyWidget("Icon"));
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
	
	void OnUpdate(bool hide){
		if (!GetGame().IsClient() || !IsInit()){
			return;
		}
		if (!GetGame().GetPlayer())
			m_LayoutRoot.Show(false);
		
		HideImage(hide);
		UpdateImage(PlayerBase.Cast(GetGame().GetPlayer()).GetClientIcon());
	}
	
	void HideImage(bool hide){
		if (hide){
			m_LayoutRoot.Show(false);
			return;
		}
		m_LayoutRoot.Show(true);
	}
	
	void UpdateImage(string icon){
		if ( icon == "" ){
			HideImage(true);
			return;
		}
		m_Icon.LoadImageFile(0, icon);
	}
}