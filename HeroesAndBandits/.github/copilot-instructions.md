# HeroesAndBandits Mod - AI Assistant Guidelines

This document provides both **project-specific patterns** for the HeroesAndBandits mod and **general DayZ modding toolkit** references.

---

# 📁 PROJECT: HEROESANDBANDITS

## Overview

HeroesAndBandits is a DayZ mod that tracks player "Humanity" - a score that increases with heroic actions (helping players, killing zombies) and decreases with bandit actions (killing innocents). Players are classified into three affinities:
- **Bambi** (neutral) - Starting state, Humanity near 0
- **Hero** - Positive Humanity above threshold
- **Bandit** - Negative Humanity below threshold

## Key Systems

### 1. Humanity & Affinity System
| Constant | Value | Description |
|----------|-------|-------------|
| `HAB_BAMBI` | 0 | Neutral player (fresh spawn or near-zero humanity) |
| `HAB_HERO` | 1 | Positive humanity above first level threshold |
| `HAB_BANDIT` | 2 | Negative humanity below first level threshold |

### 2. Config System (UFramework Integration)
The mod uses **UFramework** (`UDBGlobalHandler`) for persistent configuration storage:

```enforce
// Loading config from UFramework
static autoptr UDBGlobalHandler<HaBGeneralConfig> m_HaBGeneralConfigHandler = new UDBGlobalHandler<HaBGeneralConfig>("HAB_GENERAL");
static autoptr HaBGeneralConfig m_HaBGeneralConfig;

// Accessing loaded config
HABPanelConfig config = HABPanelConfig.Get();
HaBGeneralConfig generalConfig = m_HaBGeneralConfig;
```

**Key Config Classes:**
- `HaBGeneralConfig` - Discord roles, levels, blocked items/recipes
- `HABPanelConfig` - UI accent colors, Discord link, info sections per affinity

### 3. GUI System Architecture

**Layout Files:** `.layout` files in `gui/layouts/` define widget hierarchy

**Key Layout Properties:**
| Property | Value 0 | Value 1 |
|----------|---------|---------|
| `hexactpos` / `vexactpos` | Relative (0.0-1.0) | Absolute pixels |
| `hexactsize` / `vexactsize` | Relative (0.0-1.0) | Absolute pixels |
| `ignorepointer` | Receives mouse events | Ignores mouse events |
| `noclip` | Clips children to bounds | Allows overflow |

**Panel Classes (in `MainPanel.c`):**
- `HAB_MainPanel` - Main menu container with tab management
- `HAB_PageBase` - Base class for all tab pages
- `HAB_StatsPage` - Player stats, humanity display, progress bars
- `HAB_LeaderboardsPage` - Hero/Bandit leaderboards
- `HAB_InfoPage` - Server info with collapsible sections

### 4. Player Data System

**Key Player Methods (in `PlayerImplement.c` / `PlayerBase.c`):**
```enforce
float Humanity()          // Current humanity score
int HABLevel()            // Current level (negative for bandit)
int HABAffinity()         // HAB_BAMBI, HAB_HERO, or HAB_BANDIT
HeroesAndBanditsPlayerBase HABData()  // Full player data object
void RefreshHABData()     // Refresh data from server
void NewHABAction(string action)      // Trigger an action
```

### 5. Localization
Use `languagecore/stringtable.csv` for translations:
```
"HAB_KEY","original","english","french",...
```
Reference in layouts/code: `#HAB_KEY`

---

## Project File Structure

```
HeroesAndBandits/
├── config.cpp              # CfgPatches, CfgVehicles (flags), CfgMods
├── data/Inputs.xml         # Keybinding definitions
├── gui/
│   ├── images/             # .paa textures, .imageset files
│   └── layouts/            # .layout UI definitions
├── languagecore/
│   └── stringtable.csv     # Localization strings
└── scripts/
    ├── 3_Game/HaB/         # Core systems, configs, constants
    │   ├── generalConfig.c # HaBGeneralConfig (levels, roles, blocked items)
    │   ├── PanelConfig.c   # HABPanelConfig (UI, accent colors)
    │   ├── habContants.c   # Death types enum
    │   ├── ActionsConfigs.c # Action definitions per affinity
    │   └── HeroesAndBanditsPlayer.c # Player data class
    ├── 4_World/HaB/        # World entities, actions, GUI logic
    │   ├── Entities/       # modded ItemBase, PlayerBase, etc.
    │   ├── Classes/        # Actions (Continuous, Interact, SingleUse)
    │   ├── GUI/            # MainPanel.c, IngameHud.c, OG_Panel.c
    │   └── HeroesAndBandits/ # Core singleton, controllers
    └── 5_Mission/HaB/      # Mission logic, menu creation
        ├── GUI/            # Expansion integrations
        └── mission/        # MissionBase.c, MissionServer.c, MissionGameplay.c
```

---

## Coding Patterns for This Mod

### Creating New GUI Elements
```enforce
// 1. Define layout in .layout file
// 2. Create handler class extending ScriptedWidgetEventHandler
class MyWidget extends ScriptedWidgetEventHandler {
    protected Widget layoutRoot;
    
    void MyWidget(Widget parent) {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets("path/to/layout.layout", parent);
        layoutRoot.SetHandler(this);
    }
    
    override bool OnClick(Widget w, int x, int y, int button) {
        // Handle clicks
        return false;
    }
}
```

### Accessing Config Values
```enforce
// Panel config (accent colors, Discord link, info sections)
HABPanelConfig config = HABPanelConfig.Get();
if (config) {
    int accentColor = config.GetAccentColor();
    string discordLink = config.DiscordLink;
    HABPanelAffinityConfig affinityConfig = config.GetAffinityConfig(playerAffinity);
}

// General config (levels, blocked items)
if (m_HaBGeneralConfig) {
    TIntArray levels = m_HaBGeneralConfig.Levels;
    string pathHeroRole = m_HaBGeneralConfig.PathHeroRole;
}
```

### Discord Integration
```enforce
// Check if player has Discord role (requires UFramework Discord integration)
#ifdef UFRAMEWORK_DISCORD
UDiscordUser discordUser = UDiscordUser.Get(player);
if (discordUser && discordUser.HasRole("ROLE_ID")) {
    // Player has the role
}
#endif

// Open Discord link
GetGame().OpenURL("https://discord.gg/yourlink");
```

### Color System
```enforce
// Affinity-based colors
int banditColor = ARGB(255, 204, 64, 51);   // Soft red
int heroColor = ARGB(255, 51, 115, 204);     // Soft blue
int bambiColor = ARGB(255, 64, 128, 64);     // Soft green

// Accent color from config (default gold)
int accentColor = ARGB(255, 217, 166, 77);   // Gold
// Or use config: config.GetAccentColor()
```

---

## Important Notes for This Project

1. **UFramework Dependency** - This mod requires UFramework for database/config handling
2. **Expansion Compatibility** - Has optional integration with DayZ Expansion mod
3. **Discord Integration** - Optional Discord role-based features via UFramework
4. **Layout Responsiveness** - Use relative sizes (hexactsize 0) for flexible layouts
5. **Accent Colors** - All accent widgets should use `config.GetAccentColor()` for consistency

---

# 🚨 CRITICAL: ENFORCE SCRIPT IS NOT C++ - YOU WILL FAIL WITHOUT TOOLS

## ⛔ STOP! READ THIS FIRST!

**Enforce Script LOOKS like C++ but it is NOT.** It has dozens of quirks, missing features, and unique syntax that WILL break your code if you guess.

**FACTS:**
- ❌ No ternary operator (`a ? b : c` does NOT work)
- ❌ No `static_cast`, `dynamic_cast` - must use `Class.CastTo()`
- ❌ No templates, no lambdas, no auto keyword
- ❌ `extends` does NOT modify vanilla classes - you need `modded class`
- ❌ Missing `override` keyword = silent failure, not an error
- ❌ Forgetting `super.Method()` = broken inheritance chain

**Your code WILL NOT WORK if you write from memory.** You MUST use the tools below.

---

## 🔥 YOU HAVE FULL TOOL ACCESS - USE IT!

You have access to the complete DayZ modding toolkit. **USE THESE TOOLS FOR EVERY TASK.**

### 📚 KNOWLEDGE BASE TOOLS (Start Here!)

#### `search_dayz_kb` - YOUR FIRST STOP FOR EVERY TASK
**What it does:** Searches a curated knowledge base of DayZ modding tutorials, best practices, and expert guides.

**Why it's critical:** The KB contains solutions to problems the community has already solved. It knows about quirks, pitfalls, and the RIGHT way to implement features.

**Parameters:**
- `query` (required): What you're trying to do or learn about
- `limit` (optional): Number of results (default: 5)

**Example usage:**
```
search_dayz_kb query:"how to create custom item"
search_dayz_kb query:"modded class override method"
search_dayz_kb query:"RPC server client communication"
search_dayz_kb query:"player inventory manipulation"
search_dayz_kb query:"config.cpp CfgVehicles syntax"
```

**USE THIS BEFORE WRITING ANY CODE!**

---

#### `list_dayz_kb` - See All Available KB Articles
**What it does:** Returns a list of all available knowledge base articles.

**When to use:** When you want to browse what documentation is available, or find the exact name of an article.

**Parameters:** None

---

#### `get_dayz_kb_article` - Get a Specific KB Article
**What it does:** Retrieves the full content of a specific KB article by name.

**Parameters:**
- `name` (required): The exact article name from `list_dayz_kb`

**Example:**
```
get_dayz_kb_article name:"modded_class_overview"
get_dayz_kb_article name:"custom_actions"
```

---

### 🔍 SOURCE CODE SEARCH TOOLS (Find Exact Implementations!)

#### `search_dayz_files` - FIND THE ACTUAL VANILLA CODE
**What it does:** Vector search across ALL DayZ source files to find relevant code, classes, and implementations.

**Why it's critical:** You MUST see the actual vanilla implementation before overriding methods. The method signature, parameters, and return type must match EXACTLY.

**Parameters:**
- `query` (required): What to search for (class names, method names, concepts)
- `limit` (optional): Number of results (default: 5)
- `includeMods` (optional): Include community mod code (default: false)

**Example usage:**
```
search_dayz_files query:"PlayerBase OnConnect"
search_dayz_files query:"ItemBase EEKilled"
search_dayz_files query:"override void Init"
search_dayz_files query:"GetGame().CreateObject"
search_dayz_files query:"RPC SendRPC"
```

**ALWAYS use this to find method signatures before overriding!**

---

#### `get_dayz_file_by_class` - Get File Content by Class Name
**What it does:** Finds and returns the file(s) containing a specific class, including the first 3 parts of content.

**Parameters:**
- `className` (required): The class name to find
- `includeMods` (optional): Include community mods

**Example:**
```
get_dayz_file_by_class className:"PlayerBase"
get_dayz_file_by_class className:"ItemBase"
get_dayz_file_by_class className:"CarScript"
```

---

#### `get_dayz_file_parts` - Get Parts List for Large Files
**What it does:** Large files are split into parts. This returns the list of available parts for a file.

**Parameters:**
- `filePath` (required): The file path

**Example:**
```
get_dayz_file_parts filePath:"scripts/4_world/entities/manbase/playerbase.c"
```

---

#### `get_dayz_file_part` - Get Specific Part of a File
**What it does:** Retrieves a specific part of a large file.

**Parameters:**
- `filePath` (required): The file path
- `partNumber` (required): Part number (1-based)

**Example:**
```
get_dayz_file_part filePath:"scripts/4_world/entities/manbase/playerbase.c" partNumber:2
```

---

#### `search_in_dayz_file` - Search Within a Specific File
**What it does:** Text search within a specific file to find methods, variables, or patterns.

**Parameters:**
- `filePath` (required): The file path
- `searchTerm` (required): Text to search for (case-insensitive)

**Example:**
```
search_in_dayz_file filePath:"scripts/4_world/entities/itembase.c" searchTerm:"OnInventoryEnter"
```

---

#### `get_dayz_credits` - Check API Credits
**What it does:** Returns your remaining DaemonForge API credits.

---

## 📋 MANDATORY WORKFLOW - FOLLOW THIS EVERY TIME!

### Step 1: Research the Feature (BEFORE writing ANY code)
```
search_dayz_kb query:"[what you want to build]"
```
Read the KB articles. They contain the RIGHT way to do things.

### Step 2: Find the Vanilla Implementation
```
search_dayz_files query:"[class or method you need]"
get_dayz_file_by_class className:"[TargetClass]"
```
You MUST see the original code to override correctly.

### Step 3: Understand the Class Structure
```
lsp_find_class className:"[TargetClass]"
lsp_get_class_hierarchy className:"[TargetClass]"
lsp_get_class_methods className:"[TargetClass]"
```

### Step 4: Check for Known Quirks
```
list_enforce_quirks search:"[topic]"
get_enforce_context section:"[topic]"
```

### Step 5: Write Code
NOW you can write code, using the information you gathered.

### Step 6: VALIDATE (Required!)
```
lsp_get_diagnostics filePath:"[your file]"
check_class_references classNames:["Class1", "Class2"]
```
**DO NOT present code with compile errors!**

---

## 🛠️ COMPLETE TOOL REFERENCE

### 📚 DayZ Knowledge Base Tools
| Tool | Description |
|------|-------------|
| `search_dayz_kb` | Vector search the knowledge base for tutorials, guides, best practices |
| `list_dayz_kb` | List all available KB articles |
| `get_dayz_kb_article` | Get full content of a specific KB article |

### 🔍 DayZ Source Code Tools
| Tool | Description |
|------|-------------|
| `search_dayz_files` | Vector search across all DayZ source files |
| `get_dayz_file_by_class` | Get file content by class name |
| `get_dayz_file_parts` | Get list of parts for a large file |
| `get_dayz_file_part` | Get specific part of a file |
| `search_in_dayz_file` | Search within a specific file |
| `get_dayz_credits` | Check remaining API credits |

### 🏗️ Mod Scaffolding Tools
| Tool | Description |
|------|-------------|
| `scaffold_mod` | Generate complete mod folder structure with all required files |
| `generate_item_config` | Generate config.cpp CfgVehicles entry for new items |
| `list_base_classes` | List common base classes with their properties |
| `find_parent_class` | Find the parent class of a given class |
| `check_class_references` | Verify that class names exist in vanilla |
| `list_inventory_slots` | List valid inventory slot names |

### 🔬 Code Intelligence (LSP) Tools
| Tool | Description |
|------|-------------|
| `lsp_find_class` | Find a class definition by exact name |
| `lsp_search_classes` | Search for classes matching a pattern |
| `lsp_get_class_hierarchy` | Get complete inheritance hierarchy |
| `lsp_find_child_classes` | Find all classes extending a base class |
| `lsp_get_class_methods` | Get all methods defined in a class |
| `lsp_find_method_overrides` | Find all overrides of a method |
| `lsp_search_symbols` | Search for any symbol type |
| `lsp_get_file_outline` | Get structure/outline of a file |
| `lsp_go_to_definition` | Navigate to symbol definition |
| `lsp_find_references` | Find all usages of a symbol |
| `lsp_get_symbol_info` | Get detailed info about a symbol |
| `lsp_get_completions` | Get code completion suggestions |
| `lsp_get_diagnostics` | **CRITICAL** - Get compile errors/warnings |

### 📂 Project Drive Tools (P:\ Access)
| Tool | Description |
|------|-------------|
| `project_read_file` | Read vanilla source file content |
| `project_list_files` | List files in vanilla directories |
| `project_grep` | Search vanilla code with text/regex |

### 🧠 Quirks & Context Tools
| Tool | Description |
|------|-------------|
| `get_enforce_context` | Get syntax reference for a topic |
| `list_context_sections` | List all available context sections |
| `get_enforce_warnings` | Get common Enforce Script pitfalls to avoid |
| `refresh_context` | Reload context after adding new .insc files |
| `list_enforce_quirks` | List known Enforce Script quirks |
| `get_enforce_quirk` | Get details on a specific quirk |
| `save_enforce_quirk` | Save a new quirk you discovered |

---

## ⚠️ ENFORCE SCRIPT TRAPS - MEMORIZE THESE!

### ❌ Things That DO NOT Work (Even Though They Look Like C++)

| What You Might Try | Why It Fails | What To Do Instead |
|-------------------|--------------|---------------------|
| `x = a ? b : c;` | No ternary operator | `if (a) x = b; else x = c;` |
| `PlayerBase.Cast(entity)` | Wrong cast syntax | `Class.CastTo(player, entity);` |
| `class MyPlayer extends PlayerBase` | Creates NEW class, doesn't modify vanilla | `modded class PlayerBase` |
| `void OnInit() { ... }` | Missing override keyword | `override void OnInit() { ... }` |
| `override void OnInit() { MyCode(); }` | Missing super call | `super.OnInit(); MyCode();` |
| `static MyClass instance;` | Static member syntax differs | Use singleton pattern with function |
| `auto x = GetSomething();` | No auto keyword | Explicitly declare type |
| `[](int x) { return x * 2; }` | No lambdas | Use named functions |
| `template<T>` | No templates | Use specific types or Managed |

### ✅ Correct Patterns

**Casting:**
```cpp
PlayerBase player;
if (Class.CastTo(player, entity)) {
    // player is now valid
}
```

**Modifying Vanilla Classes:**
```cpp
modded class PlayerBase {
    override void Init() {
        super.Init();  // ALWAYS call super FIRST!
        // Your modifications here
    }
}
```

**Null Checks:**
```cpp
if (object && object.IsValid()) {
    // Safe to use object
}
```

---

## 📁 MOD STRUCTURE

```
YourMod/
├── mod.cpp              # Mod registration (name, author, version)
├── config.cpp           # CfgPatches, CfgVehicles, CfgMods
└── Scripts/
    ├── 3_Game/          # Core game systems (rare to modify)
    ├── 4_World/         # Items, players, entities, vehicles (most common)
    │   └── YourMod/     # Your script files go here
    └── 5_Mission/       # Mission logic, UI, menus
```

Use `scaffold_mod modName:"YourMod"` to generate this automatically!

---

## ✅ FINAL CHECKLIST - DO NOT SKIP!

Before presenting ANY code to the user, verify:

- [ ] ✅ Used `search_dayz_kb` to find the right approach
- [ ] ✅ Used `search_dayz_files` to find vanilla implementation
- [ ] ✅ Method signatures match vanilla EXACTLY
- [ ] ✅ Used `lsp_get_diagnostics` - **ZERO compile errors**
- [ ] ✅ All `override` methods call `super.MethodName()` first
- [ ] ✅ Used `modded class` (NOT `extends`) for vanilla modifications
- [ ] ✅ Checked `list_enforce_quirks` for known issues
- [ ] ✅ No ternary operators, no auto, no lambdas
- [ ] ✅ Using `Class.CastTo()` for all casts

---

## 📚 CONTEXT SECTIONS

Use `get_enforce_context section:"name"` for detailed syntax reference:

| Category | Available Sections |
|----------|-------------------|
| **Core Language** | `variables`, `functions`, `operators`, `control_flow`, `arrays`, `enums` |
| **OOP** | `oop`, `inheritance`, `overrides`, `modded_class_overview`, `modded_class_advanced` |
| **Patterns** | `patterns`, `base_classes`, `modules`, `lifecycle` |
| **Networking** | `networking_overview`, `networking_rpc`, `networking_netsync` |
| **UI** | `ui_widget_types`, `ui_widget_code`, `ui_quick_reference` |
| **Systems** | `actions`, `inventory`, `config`, `file_structure`, `persistence` |

Use `list_context_sections` to see all available sections with descriptions.

