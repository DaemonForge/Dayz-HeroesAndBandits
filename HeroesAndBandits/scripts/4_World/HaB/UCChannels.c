/**
 * File: UCChannels.c
 * Description: UniversalComms integration for Heroes and Bandits chat channels
 * Heroes get access to Hero Channel, Bandits get access to Bandit Channel
 */

#ifdef UniversalComms

class HAB_HeroChannel extends UCChannel
{
    void HAB_HeroChannel()
    {
        m_Id = "hero";
        m_DisplayName = "Hero";
        m_DefaultColor = "blue";
    }
    
    override bool IsAvailableFor(PlayerBase player)
    {
        if (!player) return false;
        
        // Must be a Hero
        if (player.HABAffinity() != HAB_HERO) return false;
        
        // Check if radio is required
        if (m_HaBGeneralConfig && m_HaBGeneralConfig.UCRequireRadio)
        {
            return HasWorkingRadio(player);
        }
        
        return true;
    }
    
    override bool CanSend(PlayerBase player)
    {
        return IsAvailableFor(player);
    }
    
    override string GetShortName()
    {
        return "Hero";
    }
    
    protected bool HasWorkingRadio(PlayerBase player)
    {
        if (!player) return false;
        if (!player.GetInventory()) return false;
        
        array<EntityAI> items = new array<EntityAI>();
        player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
        
        for (int i = 0; i < items.Count(); i++)
        {
            EntityAI item = items.Get(i);
            if (item && item.IsTransmitter() && item.HasEnergyManager() && item.GetCompEM().IsWorking())
            {
                return true;
            }
        }
        return false;
    }
}

class HAB_BanditChannel extends UCChannel
{
    void HAB_BanditChannel()
    {
        m_Id = "bandit";
        m_DisplayName = "Bandit";
        m_DefaultColor = "red";
    }
    
    override bool IsAvailableFor(PlayerBase player)
    {
        if (!player) return false;
        
        // Must be a Bandit
        if (player.HABAffinity() != HAB_BANDIT) return false;
        
        // Check if radio is required
        if (m_HaBGeneralConfig && m_HaBGeneralConfig.UCRequireRadio)
        {
            return HasWorkingRadio(player);
        }
        
        return true;
    }
    
    override bool CanSend(PlayerBase player)
    {
        return IsAvailableFor(player);
    }
    
    override string GetShortName()
    {
        return "Bandit";
    }
    
    protected bool HasWorkingRadio(PlayerBase player)
    {
        if (!player) return false;
        if (!player.GetInventory()) return false;
        
        array<EntityAI> items = new array<EntityAI>();
        player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
        
        for (int i = 0; i < items.Count(); i++)
        {
            EntityAI item = items.Get(i);
            if (item && item.IsTransmitter() && item.HasEnergyManager() && item.GetCompEM().IsWorking())
            {
                return true;
            }
        }
        return false;
    }
}

#endif
