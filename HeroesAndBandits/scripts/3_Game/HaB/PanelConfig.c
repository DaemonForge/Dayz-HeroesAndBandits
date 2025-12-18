/**
 * File: PanelConfig.c
 * Description: Configuration for the HAB Panel info sections
 * Allows server owners to provide custom information to players based on their affinity
 */

static autoptr UDBGlobalHandler<HABPanelConfig> m_HABPanelConfigHandler = new UDBGlobalHandler<HABPanelConfig>("HAB_PANEL");
static autoptr HABPanelConfig m_HABPanelConfig;

class HABPanelSection
{
	string Title;
	ref array<string> Content;
	
	void HABPanelSection()
	{
		Content = new array<string>;
	}
}

class HABPanelAffinityConfig
{
	ref array<autoptr HABPanelSection> Sections;
	
	void HABPanelAffinityConfig()
	{
		Sections = new array<autoptr HABPanelSection>;
	}
}

class HABPanelConfig
{
	float AccentColorR;
	float AccentColorG;
	float AccentColorB;
	
	string DiscordLink;
	
	ref HABPanelAffinityConfig Bambi;
	ref HABPanelAffinityConfig Hero;
	ref HABPanelAffinityConfig Bandit;
	
	void HABPanelConfig()
	{
		// Default gold accent color
		AccentColorR = 0.85;
		AccentColorG = 0.65;
		AccentColorB = 0.3;
		
		// Empty by default - set a Discord invite URL to show button
		DiscordLink = "";
		
		Bambi = new HABPanelAffinityConfig();
		Hero = new HABPanelAffinityConfig();
		Bandit = new HABPanelAffinityConfig();
	}
	
	int GetAccentColor()
	{
		int r = (int)(AccentColorR * 255);
		int g = (int)(AccentColorG * 255);
		int b = (int)(AccentColorB * 255);
		return ARGB(255, r, g, b);
	}
	
	HABPanelAffinityConfig GetAffinityConfig(int affinity)
	{
		if (affinity == HAB_HERO)
			return Hero;
		if (affinity == HAB_BANDIT)
			return Bandit;
		return Bambi;
	}
	
	static HABPanelConfig Get()
	{
		return m_HABPanelConfig;
	}
}
