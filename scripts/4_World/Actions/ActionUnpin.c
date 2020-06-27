modded class ActionUnpin
{
    override void OnStartServer(ActionData action_data)
    {
        PlayerBase sourcePlayer = PlayerBase.Cast(action_data.m_Player);
        Grenade_Base grenade = Grenade_Base.Cast(action_data.m_MainItem);
        if (sourcePlayer && grenade)
        {
			habPrint("Player with ID " + sourcePlayer.GetIdentity().GetPlainId() + " Threw " + grenade.GetType() ,"Debug");
            grenade.habSetThrower(sourcePlayer.GetIdentity().GetPlainId());
        }
        super.OnStartServer(action_data);
    }
}