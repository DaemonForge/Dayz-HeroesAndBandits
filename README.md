# Heroes and Bandits Humanity Mod

This mod add humanity to Dayz in a very expandable way, allow for server owners and mod creators to dictate almost everything including the ability to create customized Levels and Actions, the ability to create customized zones for exclusive use based on the players stats

### Player Commands
"/humanity" in local chat will tell you you're humanity
"/stat ActionName" in local chat will give you how many times you have done a certain action
"/stat Kills" in local chat will give you how many Kills you have made


## Default Config

[https://pastebin.com/0FSstxTE](https://pastebin.com/0FSstxTE)
## Main Config Options and Explanations
### Levels
The first setting is an array of the levels there are no limit to levels but there is a limit to the JSON file of 64KB
```
{
     "Name": "Bambi",   //This is the Level name, this will be the name shown to players when they change levels or in the kill feed
     "Affinity": "bambi", // This should be either bambi, hero, or bandit
     "LevelImage": "HeroesAndBandits/gui/images/BambiNotification.paa", //This is the notification icon for that level i have included 14 Icons in my mod but you can always upload custom images to your own mod
     "MinHumanity": -1000,  //This is the Minimum humanity the player has to have to be considered that level (-1 means no minimum)
     "MaxHumanity": 1000  //This is the Maximum humanity the player has to have to be considered that level (-1 means no maximum)
}
```
There is also a default level this is used when the level can't be determined in different functions, and may be used in when using load out functions and the player is new this generally should be the same as what you are using for your bambi level
#### Custom Images
If you wish to learn how to create your own mod to upload custom images [Click Here](https://github.com/daemonforge/Dayz-HeroesAndBandits/blob/master/HowToCreateCustomImages.md)


### Actions
Actions are things players can do to lose or gain humanity currently actions can be classified as either hero or bandit actions, and more actions can be added through add on mods
```
{
     "Name": "ZombieKill",  // Name of the Action should NOT ever be changed
     "Affinity": "hero",  // This should be either hero or bandit hero will add humanity, bandit subtracts humanity
     "Humanity": 5, // This is how much humanity should be adjusted (should always be a positive number) use the affinity to determine if it should subtract or add
     "NotifiyPlayer": 1  // 1 Enabled / 0 Disabled  This will determine if the player should be notified of their humanity adjustment from the action
}
```

### Zones
Zones allows you to determine zones for players to be allowed to enter, useful for creating exclusive traders a future updates will also allow you to potentially create safe areas for new players, by default there is a zone created by rog castle for bambi players only
```
{
            "Name": "Default Zone",  // Zone Name Name shown when notifications are sent to the player 
            "X": 11250,  // This is the X coordinate for the zone
            "Z": 4300,  // This is the Z coordinate for the zone
            "KillRadius": 50, // This is the radius at which the zone will kill players who enter it
            "MinHumanity": -1000, // This is the Minimum humanity the player is allowed to have to enter the zone (-1 will set to no minimum)
            "MaxHumanity": 1000, // This is the Maximum humanity the player is allowed to have to enter the zone (-1 will set to no maximum)
            "WarningRadius": 75, // This is the radius at which players will be warned they are approaching the zone
            "ShowWarningMsg": 1, // 1 / 0 This will show a warning to players who do not have valid humanity when they enter the warning radius
            "WarningMessage": "!!Warning!! you are about to enter Default Zone if you continue you will be shot!", // The warning message show to players
            "ShowWelcomeMsg": 0, // 1 Enabled / 0 Disabled This will show the welcome message to players if they have valid humanity when they enter the warning radius
            "WelcomeMessage": "Welcome to the Default Zone", // This is the message that is given to players when they enter the warning radius and they have valid humanity
            "WelcomeIcon": "HeroesAndBandits/gui/images/BambiNotification.paa",  // This is the icon for the notification for the welcome message
            "WelcomeMessageColor": [ 200, 0, 200, 200 ], // this is the color for the welcome message good for matching up to the map icon color
            "OverrideSafeZone": 0, // 1 Enabled / 2 Disabled This will allow the zone to kill players despite the area being a safe zone this also overrides god mode for admins so beware. Useful for server with Helicopters where the player may be able to get into the zone before they are killed by the check timer
            "GodModPlayers": 0, // This will god mod valid players when they enter the warning radius
            "Guards": [  // These are guards that you can have placed around the zone that eventually will shoot the players when they enter.
                {
                    "X": 11250,  // This is the X coordinate for the guard
                    "Y": 290.2,  // This is the Y coordinate for the guard
                    "Z": 4300,  // This is the Z coordinate for the guard
                    "Orientation": 0,   // This is the Orientation for the guard
                    "Skin": "SurvivorM_Francis", // The skin the guard will be
                    "WeaponInHands": "M4A1", // The gun that will be placed in the guards hands
                    "WeaponInHandsMag": "Mag_STANAGCoupled_30Rnd", //NOT CURRENTLY USED
                    "WeaponInHandsAttachments": [ // Attachments for the gun in the guards hands
                        "M4_RISHndgrd", 
                        "M4_OEBttstck",
                        "M68Optic"
                    ],
                    "GuardGear": [  // Gear the guard will be wearing
                        "PlateCarrierVest",
                        "JungleBoots_Black",
                        "CargoPants_Black",
                        "M65Jacket_Black"
                    ]
                }
            ]
        }
```

### Other Settings
```
    "ConfigVersion": "2", //Do not changed this will be used for future updates to help me ensure that your config files are updated automatically
    "NotifyLevelChange": 1, // 1 Enabled / 0 Disabled when enabled it will notify the player when their level changes
    "KillFeed": 1,  // 1 Enabled / 0 Disabled When enabled it will notify all players of Kills on the server
    "SucideFeed": 0, // 1 Enabled / 0 Disabled When enabled it will notify all players of Sucides on the server
    "ZoneCheckTimer": 3, // How often in seconds it will check to see if a player has entered a zone, (0 Will disable zone checks all together)
    "NotificationColor": [ 200, 0, 200, 200 ], // The colour for the notifications
    "NotificationMessageTime": 15, // How long in seconds notifications should stay up for
    "WarningMessageImagePath": "HeroesAndBandits/gui/images/DeathWarning.paa", // Icon for the warning messages
    "WarningMessageColor": [ 200, 255, 0, 0], // The colour for the Warning messages
    "KillFeedMessageColor": [ 200, 250, 0, 100 ], // The colour for the Kill feed messages
    "AllowStatCommand": 1,  // 1 Enabled / 0 Disabled allows the use of the /stat command
    "AllowHumanityCommand": 1, // 1 Enabled / 0 Disabled allows the use of the /humanity command
    "ShowLevelIcon": 1, // 1 Enabled / 0 Disabled This will enable or disable level icon
    "LevelIconLocation": 2 // 1 Top Right, 2 Bottom right by icons, 3 Both
    "ExceptionLogs": 1,  //  1 Enabled / 0 Disabled This show any exceptions the mod encounters in the script.log
    "VerboseLogs": 0, //  1 Enabled / 0 Disabled  This adds a bit of logging for standard use, script.log
    "DebugLogs": 0, // KEEP 0 OR YOUR LOGS WILL FILL LIKE CRAZY THIS IS FOR ME OR OTHER DEVS ONLY TO DEBUG
    "BambiSkins": [  // These skins are returned from the getPlayerSkin() Command if the player is a bambi
        "SurvivorM_Boris", 
        "SurvivorM_Hassan",
        "SurvivorF_Frida"
    ],
    "HeroSkins": [ // These skins are returned from the getPlayerSkin() Command if the player is a Hero
        "SurvivorM_Indar",
        "SurvivorM_Lewis",
        "SurvivorF_Linda"
    ],
    "BanditSkins": [ // These skins are returned from the getPlayerSkin() Command if the player is a Bandit
        "SurvivorM_Peter",
        "SurvivorM_Kaito",
        "SurvivorF_Naomi"
    ]
```

## Helper Functions for init.c
There are helper function to help you provide custom loadouts or skins to players based on their Affinity, Level, Humanity, or Stats

### Get Player Skin
```
m_HeroesAndBandits.getPlayerSkin(PlayerID)
```
This will return the players skin based on what is defined in the BambiSkins, HeroSkins, or BanditSkins in the config file


### Get Player Affinity
```
m_HeroesAndBandits.GetPlayerAffinity(PlayerID)
```
This will return the players Affinity in a string usually bambi, hero, bandit but this can be configured by the server admins will return bambi(Default Level Affinity) if the player can't be found


### Get Player Humanity
```
m_HeroesAndBandits.GetPlayerHumanity(PlayerID) 
```
This will return the players Humanity in a float value, it will return 0 if the player can't be found


### Get Player Level Name
```
m_HeroesAndBandits.GetPlayerLevelName(PlayerID) 
```
Returns the Level Name in a string, if the player doesn't exsit it will return the default level's name (usually Bambi) if the player can't be found


### Get Player Stat
```
m_HeroesAndBandits.GetPlayerStat(PlayerID, ActionName) 
```
Returns INT of the player stats for the Action specified, if player or the player's stat doesn't exist it returns 0


## Add On Mods
There are add on mods that allow players to gain and lose humanity based on their actions from other mods
[View on Steam](https://steamcommunity.com/workshop/filedetails/discussion/2111160696/2447091420211936958/)

## Supported Languages
- English
- French
- Russian - Thanks @NeonMurder
- German  - Thanks @ServerAtze
- Polish  - Thanks @Kirlen

(If you want your language supported please contact me)


## To Do List
- Get guards to shoot guns at players when the zone kills them
- Add a GUI for players to track there stats and humanity
- Add custom skins for Hero's and Bandits so they are more easily identifiable
- Add the ability for certain clothing to only be wear able by certain affinities

## Developer Helper Functions
These functions are to help developers (they will not work as they are shown below in the unit.c)

#### New Player Action
```
GetHeroesAndBandits().NewPlayerAction(PlayerID, ActionName) 
```
Adds an action to the player, adding an action to the player will update the stat, update the players humanity, action must exsit in the config or it will consider the action to give 0 humanity

#### Get Player Affinity
```
GetHeroesAndBandits().GetPlayerAffinity(PlayerID) 
```
This will return the players Affinity in a string Ussaly bambi, hero, bandit but this can be configured by the server admins will return bambi(Default Level Affinity) if the player can't be found

#### Get Player Humanity

```
GetHeroesAndBandits().GetPlayerHumanity(PlayerID) 
```
This will return the players Humanity in a float value, it will return 0 if the player can't be found

#### Get Player Level
```
GetHeroesAndBandits().GetPlayerLevel(PlayerID) 
```
Returns the habLevel object will return the default level if player can't be found

#### Get Player Level Name
```
GetHeroesAndBandits().GetPlayerLevelName(PlayerID) 
```
Returns the HabLevel Name in a string it will the default level's name usally Bambi if the player can't be found

#### Get Player Stat
```
GetHeroesAndBandits().GetPlayerStat(PlayerID, StatName) 
```
Returns INT of the player stat, if player or the player's stat doesn't exist it returns 0


For questions or help feel free to contact me on discord: DaemonForge#5454


### Repacking/Building from Source Code Rules
For anyone interested in repacking please read all this
- 1st I heavily discourage Repacking/Building from my source code, as it would greatly impact your ability to receive feature updates and bug fixes
- 2nd I will not support any repacks if its is causing conflict that I can't reproduce with the my published mod 
- 3rd YOU MUST PROVIDE CREDIT on the steam page and you must link to the Orginal Mod or the Source Code
- 4th Any repacks or builds can only be for personal use no redistributing my mod for public use, should only be for your own servers and players and you must be able to enfore this
- 5th Please ask for permission prior to repacking, almost all request will be approved as long as you have a valid reason to repack
- 6th Also note that the Source Code is the activly developing version so it may not work as it sits here right now so just be warnned.
