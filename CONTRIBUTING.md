# Contributing

## Contribution Guidelines

### Do not make EWS require any unnecessary dependencies, including mod loaders
EWS is built with portability between games in mind, all game mods must not require any mod loaders
and be able to run under a different mod loader just by changing whatever config is used to declare mods (`supermod.xml`, `mod.txt`).

## Building EWS
To build EWS, you will first need to manually build wxWidgets. Then you will be able to generate the mod's Visual Studio project and build it.

### Dependencies/Requirements
 - 5-10GB of available disk space. The EWS repository requires 5-10GB of available space once built, it should stay around 5GB but in worst case it can be up to 10GB.
 - [XMake](https://xmake.io/#/getting_started). For generating EWS's project files.
 - Visual Studio 2022. The version shouldn't matter, but the version originally used for development of the mod was `17.11.4`. The GitHub Actions runner uses `17.12.35707.178`.
 - [Python](https://www.python.org/). For build scripts.

### Building Steps
In the following steps, substitute `[arch]` with the architecture you are building EWS for (`x64` or `x86`)

#### Building wxWidgets
 1. Open a `[arch] Native Tools Command Prompt for VS 2022` window.
 2. Navigate to the folder you have cloned the EWS repository to.
 3. Run `python "./scripts/buildwxwidgets.py [arch]"`

#### Generating the EWS project and building
 1. Open a command prompt and navigate to the folder you have cloned the EWS repository to.
 2. Run `genproject.bat`
 3. Open the generated Visual Studio project (`vsxmake2022/EWS.sln`) and switch the build mode to to `release` mode.
 4. Right click the project for the game you wish to build for, and click "Build".
 5. After building, binaries will be located in `build/windows/[arch]/release`
