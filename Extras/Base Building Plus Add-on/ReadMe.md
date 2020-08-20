This mod allows for the the Destory Part Action in the ]Base Building Plus Mod to affect your humanity
This mod requires both the Heroes and Bandits Mod and the Base Building Plus Mod
https://steamcommunity.com/sharedfiles/filedetails/?id=2111160696
https://steamcommunity.com/sharedfiles/filedetails/?id=1710977250

Installation
To be safe make sure to load this after the @HeroesAndBandits and @BaseBuildingPlus in your -mods= command
Add the actions to the Heroes and Bandits Config add this to the config.json in the Actions Array


Actions to add to HeroesAndBandits config.json
```json
        {
            "Name": "BBPFlagRaid",
            "Affinity": "bandit",
            "SecondaryAffinity": "none",
            "Points": 200,
            "NotifiyPlayer": 1
        },
        {
            "Name": "DestroyBBPTier1PartRaid",
            "Affinity": "bandit",
            "SecondaryAffinity": "none",
            "Points": 200,
            "NotifiyPlayer": 1
        },
        {
            "Name": "DestroyBBPTier2PartRaid",
            "Affinity": "bandit",
            "SecondaryAffinity": "none",
            "Points": 200,
            "NotifiyPlayer": 1
        },
        {
            "Name": "DestroyBBPTier3PartRaid",
            "Affinity": "bandit",
            "SecondaryAffinity": "none",
            "Points": 350,
            "NotifiyPlayer": 1
        }
```