# Heroes and Bandits - Player Guide

This guide explains how the Heroes and Bandits mod works from a player's perspective. Learn how your actions shape your character's reputation and unlock new abilities.

---

## Table of Contents

1. [What is Heroes and Bandits?](#what-is-heroes-and-bandits)
2. [Understanding Humanity](#understanding-humanity)
3. [The Three Affinities](#the-three-affinities)
4. [Leveling System](#leveling-system)
5. [Actions That Affect Humanity](#actions-that-affect-humanity)
6. [Death Penalties](#death-penalties)
7. [Daily Limits](#daily-limits)
8. [Restricted Items and Crafting](#restricted-items-and-crafting)
9. [The Heroes and Bandits Panel](#the-heroes-and-bandits-panel)
10. [Chat Channels (UniversalComms)](#chat-channels-universalcomms)

---

## What is Heroes and Bandits?

Heroes and Bandits is a reputation system that tracks your playstyle through a **Humanity** score. Your actions in the game—helping others, killing players, hunting zombies—all influence this score. Based on your Humanity, you're classified into one of three affinities: **Bambi** (neutral), **Hero** (positive), or **Bandit** (negative).

Your affinity affects:
- What gear you can equip
- What recipes you can craft
- What chat channels you can access
- How kill rewards/penalties are calculated

---

## Understanding Humanity

**Humanity** is a numerical score that represents your character's moral standing:

| Humanity Range | Affinity |
|----------------|----------|
| +1,000 or higher | **Hero** |
| Between -999 and +999 | **Bambi** (Neutral) |
| -1,000 or lower | **Bandit** |

- **Positive humanity** means you've performed more heroic actions (helping players, killing bandits)
- **Negative humanity** means you've performed more villainous actions (killing innocents, raiding bases)
- **Near-zero humanity** means you're neutral—a fresh spawn or someone who hasn't committed to a path

Your humanity persists across deaths, though dying does cause you to lose a percentage of your current humanity.

---

## The Three Affinities

### Bambi (Neutral)

You are a **Bambi** when your humanity is between -999 and +999.

- This is the starting state for all new players
- You have no special restrictions or bonuses
- Your actions will push you toward becoming a Hero or Bandit
- You cannot access Hero or Bandit exclusive gear or chat channels

### Hero

You become a **Hero** when your humanity reaches **+1,000 or higher**.

- Gain humanity by helping other players and killing Bandits
- Lose significant humanity for killing other Heroes or innocent Bambis
- Access to Hero-exclusive gear (Shemagh Scarf)
- Access to the Hero chat channel (if UniversalComms is installed)
- Cannot equip Bandit-exclusive gear
- Cannot craft certain Bandit-only recipes

**Kill Values for Heroes:**
| Target | Humanity Change |
|--------|-----------------|
| Kill a Bandit | **+400** |
| Kill a Bambi | **-200** |
| Kill a Hero | **-1,000** |

Heroes are heavily penalized for killing fellow Heroes (-1,000), encouraging protection of allies.

### Bandit

You become a **Bandit** when your humanity reaches **-1,000 or lower**.

- All player kills push your humanity further negative
- Access to Bandit-exclusive gear (Bandit Shemagh, Facemask Shemagh)
- Access to the Bandit chat channel (if UniversalComms is installed)
- Can craft explosive recipes (Pipe Bombs)
- Cannot equip Hero-exclusive gear
- Cannot craft certain medical supplies

**Kill Values for Bandits:**
| Target | Humanity Change |
|--------|-----------------|
| Kill a Hero | **-350** |
| Kill a Bambi | **-200** |
| Kill a Bandit | **-250** |

All kills make Bandits more negative, reinforcing their villainous path.

---

## Leveling System

Within each affinity, there are **10 levels** based on how much humanity you've accumulated. Levels are determined by the absolute value of your humanity.

### Level Thresholds

| Level | Humanity Required |
|-------|-------------------|
| Level 1 | 1,000 |
| Level 2 | 2,500 |
| Level 3 | 5,000 |
| Level 4 | 10,000 |
| Level 5 | 20,000 |
| Level 6 | 40,000 |
| Level 7 | 80,000 |
| Level 8 | 200,000 |
| Level 9 | 500,000 |
| Level 10 | 1,000,000 |

**Examples:**
- A player with **+5,000 humanity** is a **Hero Level 3**
- A player with **-10,000 humanity** is a **Bandit Level 4**
- A player with **+500 humanity** is a **Bambi** (not yet Level 1)

Your maximum achieved Hero level and minimum achieved Bandit level are tracked separately and persist.

---

## Actions That Affect Humanity

### Helping Other Players (Medical Actions)

Performing medical actions on other players increases your humanity:

| Action | Humanity Gained | Daily Limit |
|--------|-----------------|-------------|
| Give CPR | **+75** | 15 times per day |
| Bandage a player | **+25** | 50 times per day |
| Give Blood | **+25** | 15 times per day |
| Give Saline | **+25** | 15 times per day |
| Give Tetracycline | **+15** | 25 times per day |
| Give Painkiller | **+15** | 25 times per day |
| Give Charcoal | **+15** | 25 times per day |
| Give Vitamin | **+15** | 25 times per day |
| Apply Splint | **+15** | 25 times per day |

### Combat Actions

#### Killing Zombies

| Action | Humanity Gained | Daily Limit |
|--------|-----------------|-------------|
| Kill a Zombie | **+5** | Unlimited |

#### Hunting Animals

| Action | Humanity Gained | Daily Limit |
|--------|-----------------|-------------|
| Hunt an Animal | **+10** | 25 times per day |

**Note:** Hunting animals uses the ADD adjustment type, meaning the humanity gain moves in the direction of your current humanity. If you're a Bandit (negative humanity), hunting gives -10 instead.

#### Killing Players (Universal Values)

These values apply when you are a **Bambi** (neutral player):

| Target | Humanity Change | Daily Limit |
|--------|-----------------|-------------|
| Kill a Bandit | **+350** | 5 times per day |
| Kill a Hero | **-350** | 5 times per day |
| Kill a Bambi | **-300** | 5 times per day |

**Important:** Once you become a Hero or Bandit, the kill values change significantly (see the Hero and Bandit sections above).

### Raiding Actions

Raiding player-built structures decreases your humanity:

| Action | Humanity Lost | Daily Limit |
|--------|---------------|-------------|
| Raid a Fence | **-50** | Unlimited |
| Raid a Watchtower | **-30** | Unlimited |

---

## Death Penalties

When you die, you lose a percentage of your current humanity. The penalty varies based on how you died:

| Cause of Death | Humanity Lost |
|----------------|---------------|
| Suicide (F11) | **20%** of current humanity |
| Generic Death | **10%** of current humanity |
| Starvation | **10%** of current humanity |
| Killed by Zombies | **7%** of current humanity |
| Died in Contamination Zone | **5%** of current humanity |
| Killed by Another Player | **3%** of current humanity |

**Examples:**
- A Hero with +10,000 humanity who commits suicide loses 2,000 humanity (now +8,000)
- A Bandit with -5,000 humanity who dies to zombies loses 350 humanity (now -4,650, moving toward neutral)
- A Hero with +50,000 humanity killed by a player loses 1,500 humanity (now +48,500)

**Note:** Death penalties always move your humanity toward zero, regardless of your current affinity.

---

## Daily Limits

Many actions have **daily limits** to prevent exploitation. Once you reach the daily limit for an action, you will still perform the action, but you won't receive any humanity change.

### Actions with Daily Limits

| Action | Daily Limit |
|--------|-------------|
| Bandage Player | 50 |
| Hunt Animal | 25 |
| Give Tetracycline | 25 |
| Give Painkiller | 25 |
| Give Charcoal | 25 |
| Give Vitamin | 25 |
| Apply Splint | 25 |
| Give Blood | 15 |
| Give Saline | 15 |
| Give CPR | 15 |
| Kill Hero (as Hero) | 10 |
| Kill Bandit (as Hero) | 10 |
| Kill Bambi (as Hero) | 10 |
| Kill Hero (as Bambi) | 5 |
| Kill Bandit (as Bambi) | 5 |
| Kill Bambi (as Bambi) | 5 |

### Actions with No Daily Limit

| Action |
|--------|
| Kill Zombie |
| Raid Fence |
| Raid Watchtower |
| All player kills (as Bandit) |

Daily limits reset each day.

---

## Restricted Items and Crafting

Certain items and recipes are restricted based on your affinity and humanity level.

### Equipment Restrictions

#### Bandit-Only Gear
*Requires Bandit Level 1 or higher (humanity ≤ -1,000)*

| Item | Restriction |
|------|-------------|
| Bandit Shemagh (all colors) | Bandits only |
| Facemask Shemagh (all colors) | Bandits only |

If you are a Hero or Bambi and try to equip these items, you will be blocked with the message: **"Bandit gear only"**

#### Hero-Only Gear
*Requires Hero Level 1 or higher (humanity ≥ +1,000)*

| Item | Restriction |
|------|-------------|
| Scarf Shemagh (all colors) | Heroes only |

If you are a Bandit or Bambi and try to equip this item, you will be blocked with the message: **"Hero gear only"**

### Crafting Restrictions

#### Recipes Requiring Any Affinity
*Requires Hero Level 1 OR Bandit Level 1 (|humanity| ≥ 1,000)*

| Recipe | Restriction |
|--------|-------------|
| Craft Wooden Crate | Requires Hero or Bandit status |
| Craft Watchtower Kit | Requires Hero or Bandit status |

Bambis (neutral players) cannot craft these items until they commit to a path.

#### Bandit-Only Recipes
*Requires Bandit Level 1 or higher (humanity ≤ -1,000)*

| Recipe | Restriction |
|--------|-------------|
| Make Pipe Bomb | Bandits only |
| Make Pipe Bomb Shell | Bandits only |
| Make BB Fuse | Bandits only |

Only Bandits can craft explosive devices.

#### Recipes Blocked for Bandits
*Blocked for Bandit Level 1 or higher (humanity ≤ -1,000)*

| Recipe | Restriction |
|--------|-------------|
| Craft Blood Bag IV | Bandits cannot craft medical supplies |

Heroes and Bambis can craft this item, but Bandits cannot.

### Block Notifications

When you attempt to use a restricted item or craft a restricted recipe, you'll receive a notification explaining why you're blocked. These notifications have an **18-second cooldown** to prevent spam.

---

## The Heroes and Bandits Panel

Press **J** (default keybind) to open the Heroes and Bandits Panel.

The panel displays:
- Your current humanity score
- Your current affinity (Bambi, Hero, or Bandit)
- Your current level
- Progress toward the next level
- Your action statistics

### Rebinding the Key

You can change the panel keybind in DayZ's settings:
1. Go to **Settings** → **Controls**
2. Find **Heroes And Bandits Panel** in the mod keybindings section
3. Assign your preferred key

---

## Chat Channels (UniversalComms)

If the server has **UniversalComms** installed, Heroes and Bandits adds two exclusive chat channels:

### Hero Channel

| Property | Value |
|----------|-------|
| Channel Name | Hero |
| Text Color | Blue |
| Who Can Use | Heroes only (humanity ≥ +1,000) |

### Bandit Channel

| Property | Value |
|----------|-------|
| Channel Name | Bandit |
| Text Color | Red |
| Who Can Use | Bandits only (humanity ≤ -1,000) |

### Radio Requirement

Some servers may require you to have a **working radio** in your inventory to use these channels. The radio must:
- Be a transmitter-type device
- Have power (batteries with charge)
- Be turned on and functional

If the server has this requirement enabled and you don't have a working radio, you won't be able to send messages in your affinity's chat channel even if you meet the humanity requirement.

---

## Quick Reference Card

### Affinity Thresholds

| Affinity | Humanity Range |
|----------|----------------|
| Hero | +1,000 or higher |
| Bambi | -999 to +999 |
| Bandit | -1,000 or lower |

### Top Humanity-Gaining Actions

| Action | Humanity | Notes |
|--------|----------|-------|
| Give CPR | +75 | 15/day limit |
| Kill Bandit (as Hero) | +400 | 10/day limit |
| Kill Bandit (as Bambi) | +350 | 5/day limit |
| Bandage Player | +25 | 50/day limit |
| Give Blood/Saline | +25 | 15/day limit each |

### Top Humanity-Losing Actions

| Action | Humanity | Notes |
|--------|----------|-------|
| Kill Hero (as Hero) | -1,000 | 10/day limit |
| Kill Hero (as Bambi) | -350 | 5/day limit |
| Kill Bambi | -200 to -300 | Varies by affinity |
| Raid Fence | -50 | Unlimited |
| Suicide | -20% | Percentage of current |

### Keybinds

| Action | Default Key |
|--------|-------------|
| Open Heroes and Bandits Panel | **J** |

---

*This guide reflects the default values. Server administrators may have customized these settings.*
