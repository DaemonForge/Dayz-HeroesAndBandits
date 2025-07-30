modded class ActionUnpin: ActionSingleUseBase
{
    override void OnStartServer(ActionData action_data)
    {
        PlayerBase sourcePlayer = PlayerBase.Cast(action_data.m_Player);
        Grenade_Base grenade = Grenade_Base.Cast(action_data.m_MainItem);
        if (sourcePlayer && grenade)
        {
            grenade.habSetActivatedBy(sourcePlayer.GetIdentity().GetId());
        }
        super.OnStartServer(action_data);
    }
}