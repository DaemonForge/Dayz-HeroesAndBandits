# Block Rules System

The Block Rules system allows you to restrict items, recipes, weapons, and building based on player humanity.

## Configuration

Block rules are configured in `HAB_GENERAL` config under the `BlockRules` array.

### Rule Structure

```json
{
  "Type": 0,
  "Target": "ItemClassName",
  "Condition": "<1000",
  "BlockMessage": "Custom message shown to player"
}
```

### Block Types

| Type | Name | Description |
|------|------|-------------|
| 0 | Equip | Blocks equipping/wearing items |
| 1 | Recipe | Blocks crafting recipes |
| 2 | Weapon | Blocks firing weapons |
| 3 | Build | Blocks placing/building |

## Condition Format

Conditions use `<` or `>` followed by a value.

### Unsigned Values (Absolute Humanity)

Compares against `|humanity|` (absolute value) - affects both heroes AND bandits equally.

| Condition | Meaning | Blocked | Allowed |
|-----------|---------|---------|---------|
| `<1000` | Requires 1000+ humanity (either direction) | Bambis | Heroes, Bandits |
| `<5000` | Requires 5000+ humanity (either direction) | Low-level players | High-level Heroes/Bandits |

### Signed Values (Directional Humanity)

Compares against actual humanity value.

| Condition | Meaning | Blocked | Allowed |
|-----------|---------|---------|---------|
| `>-1000` | Bandit only | Bambis, Heroes | Bandits |
| `<+1000` | Hero only | Bambis, Bandits | Heroes |
| `<-999` | Not for bandits | Bandits | Bambis, Heroes |
| `>+999` | Not for heroes | Heroes | Bambis, Bandits |

## Quick Reference

**One group only:**
- Hero only: `<+1000`
- Bandit only: `>-1000`

**Block one group:**
- Block heroes: `>+999`
- Block bandits: `<-999`

**Require any affinity:**
- `<1000` (blocks bambis, allows heroes and bandits)

## Examples

```json
{
  "BlockRules": [
    { "Type": 0, "Target": "GhillieSuit_ColorBase", "Condition": "<5000", "BlockMessage": "Requires Level 3+" },
    { "Type": 2, "Target": "M4A1", "Condition": "<1000", "BlockMessage": "Military weapons require affinity" },
    { "Type": 1, "Target": "MakePipeBomb", "Condition": ">-1000", "BlockMessage": "Bandits only" },
    { "Type": 3, "Target": "Fence", "Condition": "<1000", "BlockMessage": "Building requires Hero or Bandit status" }
  ]
}
```
