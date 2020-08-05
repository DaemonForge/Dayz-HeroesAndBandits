modded class DayZPlayerImplement extends DayZPlayer
{
	override void SetSuicide(bool state)
	{
		super.SetSuicide(state);

		if (state && GetGame().IsServer())
			SetAllowDamage(true);
	}
}