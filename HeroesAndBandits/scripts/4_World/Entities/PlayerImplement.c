modded class DayZPlayerImplement
{
	override void SetSuicide(bool state)
	{
		super.SetSuicide(state);

		if (state && GetGame().IsServer())
			SetAllowDamage(true);
	}
}