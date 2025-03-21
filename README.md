# EWS

Mod for Diesel Engine games to reimplement the "Bringer of Worlds" level editor for release builds.

**This mod is highly experimental, and will cause a game crash on any failure.**

###### Image of the Mod working
![](/.github/ews-2025-03-21.png)
###### YouTube Video of the Mod Working
[![](https://img.youtube.com/vi/IYaGhP9ibMQ/0.jpg)](https://www.youtube.com/watch?v=IYaGhP9ibMQ)

## Compatibility
|Game              |Compatible|
|:-----------------|:---------|
|RAID: World War II|Yes       |
|PAYDAY 2          |No        |
|PAYDAY: The Heist |No        |

**At the moment this mod is incompatible with the current public version of "RAID-SuperBLT"**.

## Features
### C++ mod
 - `EWS` engine module reimplementation.
 - `Idstring:s()` reimplementation, to allow mods to obtain source strings for Idstrings from a hashlist.

### Lua mod
 - Modifications to game Lua to allow the game's level editor (Bringer of Worlds) to work properly.

## TODO / Next Steps
 - Implement the rest of the UI elements

### Support for other games?

Support for RAID: World War II must be finished before starting work on making the mod work with other games using Diesel.
This process will take a lot of time as there are a lot of things that still need to be finished and polished, some of these things include:
 - Writing a new asset bundling system for mod assets to load properly.
 - Rewriting a section of RAID's asset loading system to allow for unplanned access to files in packages (accessing files in packages without having to load the package they belong to).
**This will be necessary as the only way to get the mod working properly at the moment is to extract and repackage the entire game to have all non-streamed assets in one single package.**

## Editor Icons
The default editor icons (found in the `assets` folder of this repository) are from the game files of [Lead and Gold: Gangs of the Wild West](https://store.steampowered.com/app/42120/Lead_and_Gold_Gangs_of_the_Wild_West/).

No newer Diesel Engine game has been released with the editor icons packaged, so obtaining newer icons is difficult.

## License
### Source Code
EWS mod source code (C++ and Lua) is currently all rights reserved.

### Compiled Binaries
Compiled binaries fall under the licenses of wxWidgets, EWS and MinHook.

## Project Structure

 - `assets`: Folder containing generic assets needed for the editor to run (editor icons, etc.).
 - `ews_lib`: The core library containing implementations for all UI controls.
 - `game_lua`: Library to allow easily swapping between game-specific implementations of Lua.
 - `game_mod`: Folder containing Lua mods for each game to allow for Bringer of Worlds to run.
 - `pd2`/`pdth`/`raidww2`: Game-specific C++ mods implementing any engine version specific variations of features.
 - `scripts`: Build scripts used by GitHub Actions to automate compiling the project, `buildwxwidgets.py` can be ran to build wxWidgets for you.
 - `test_env`: Deprecated testing environment used before game implementations were written.
 - `version_proxy`: DLL proxy for games to load the EWS library before any others.

## Disclaimer
This project is not endorsed by or affiliated with Starbreeze Studios, Lion game Lion, Fatshark or Grin.
