This mod allows for the the Destory Code Lock Action in the Code Lock Mod to affect your humanity
This mod requires both the Heroes and Bandits Mod and the Code Lock Mod
https://steamcommunity.com/sharedfiles/filedetails/?id=2111160696
https://steamcommunity.com/sharedfiles/filedetails/?id=1646187754

Installation
To be safe make sure to load this after the @HeroesAndBandits and @CodLock in your -mods= command
Add the actions to the Heroes and Bandits Config add this to the config.json in the Actions Array
```json
        {
            "Name": "CodeLockFenceRaid",
            "Affinity": "bandit",
            "SecondaryAffinity": "none",
            "Points": 300,
            "NotifiyPlayer": 1
        },
        {
            "Name": "CodeLockTentRaid",
            "Affinity": "bandit",
            "SecondaryAffinity": "none",
            "Points": 250,
            "NotifiyPlayer": 1
        }
```