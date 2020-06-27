modded class ActionUnpin
{
    override void OnStartServer(ActionData action_data)
    {
        PlayerBase sourcePlayer = PlayerBase.Cast(action_data.m_Player);
        Grenade_Base grenade = Grenade_Base.Cast(action_data.m_MainItem);
        if (sourcePlayer && grenade)
        {
			habPrint("Player with ID " + sourcePlayer.GetIdentity().GetPlainId() + " Activated " + grenade.GetType() ,"Debug");
            grenade.habSetActivatedBy(sourcePlayer.GetIdentity().GetPlainId());
        }
        super.OnStartServer(action_data);
    }
}