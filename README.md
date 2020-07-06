# Heroes and Bandits Humanity Mod

This mod add humanity to Dayz in a very expandable way, allow for server owners and mod creators to dictate almost everything including the ability to create customized Levels and Actions, the ability to create customized zones for exclusive use based on the players stats

### Player Commands
- "/humanity" in local chat will tell you you're humanity
- "/stat ActionName" in local chat will give you how many times you have done a certain action
- "/stat Kills" in local chat will give you how many Kills you have made
- "J" Opens a very simple GUI to see your Humanity, Level and some Stats

Each command and the GUI can be disabled in the server configuration

For Information on how to configure the mod check out our wiki
[Wiki](https://github.com/daemonforge/Dayz-HeroesAndBandits/wiki)

## Supported Languages
- English
- French
- Russian - Thanks @NeonMurder
- German  - Thanks @ServerAtze
- Polish  - Thanks @Kirlen
- Portuguese - Thanks @Wwillers

(If you want your language supported please contact me)


## To Do List
- Get guards to shoot guns at players when the zone kills them
- Add custom skins for Hero's and Bandits so they are more easily identifiable
- Add the ability for certain clothing to only be wear able by certain affinities
- Create a export function for server leaderboards and a basic web app to view
- Change Icon location settings to be client side so that way players can choose where to show it
- Add secondary affinities, eg Hunter, and Medic and make it expandable

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
- 7th Must include this [Notice File](https://raw.githubusercontent.com/daemonforge/Dayz-HeroesAndBandits/master/NOTICE) in your code (If you are just copying the PBOs then it will already be in there)
