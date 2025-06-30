Piglet's Big Game
=================
[![Build Status]][actions] [![Progress]][Progress Site] [![Game Progress]][Progress Site] [![Engine Progress]][Progress Site]
<!--
[![Discord Badge]][discord]
-->

[Build Status]: https://github.com/tgsm/pbg/actions/workflows/build.yml/badge.svg
[actions]: https://github.com/tgsm/pbg/actions/workflows/build.yml
[Progress]: https://decomp.dev/tgsm/pbg.svg?mode=shield&measure=code&label=Overall
[Game Progress]: https://decomp.dev/tgsm/pbg.svg?mode=shield&measure=code&label=Game&category=game
[Engine Progress]: https://decomp.dev/tgsm/pbg.svg?mode=shield&measure=code&label=Engine&category=engine
[Progress Site]: https://decomp.dev/tgsm/pbg
<!--
Replace with your Discord server's ID and invite URL.
[Discord Badge]: https://img.shields.io/discord/727908905392275526?color=%237289DA&logo=discord&logoColor=%23FFFFFF
[discord]: https://discord.gg/hKx3FJJgrV
-->

A work-in-progress decompilation of Piglet's Big Game.

This repository does **not** contain any game assets or assembly whatsoever. An existing copy of the game is required.

Supported versions:

- `GPLE9G`: USA

Dependencies
============

Windows
--------

On Windows, it's **highly recommended** to use native tooling. WSL or msys2 are **not** required.
When running under WSL, [objdiff](#diffing) is unable to get filesystem notifications for automatic rebuilds.

- Install [Python](https://www.python.org/downloads/) and add it to `%PATH%`.
  - Also available from the [Windows Store](https://apps.microsoft.com/store/detail/python-311/9NRWMJP3717K).
- Download [ninja](https://github.com/ninja-build/ninja/releases) and add it to `%PATH%`.
  - Quick install via pip: `pip install ninja`

macOS
------

- Install [ninja](https://github.com/ninja-build/ninja/wiki/Pre-built-Ninja-packages):

  ```sh
  brew install ninja
  ```

- Install [wine-crossover](https://github.com/Gcenx/homebrew-wine):

  ```sh
  brew install --cask --no-quarantine gcenx/wine/wine-crossover
  ```

After OS upgrades, if macOS complains about `Wine Crossover.app` being unverified, you can unquarantine it using:

```sh
sudo xattr -rd com.apple.quarantine '/Applications/Wine Crossover.app'
```

Linux
------

- Install [ninja](https://github.com/ninja-build/ninja/wiki/Pre-built-Ninja-packages).
- For non-x86(_64) platforms: Install wine from your package manager.
  - For x86(_64), [wibo](https://github.com/decompals/wibo), a minimal 32-bit Windows binary wrapper, will be automatically downloaded and used.

Building
========

- Clone the repository:

  ```sh
  git clone https://github.com/tgsm/pbg.git
  ```

- Copy your game's disc image to `orig/GPLE9G`.
  - Supported formats: ISO (GCM), RVZ, WIA, WBFS, CISO, NFS, GCZ, TGC
  - After the initial build, the disc image can be deleted to save space.

- Configure:

  ```sh
  python configure.py
  ```

  To use a version other than `GPLE9G` (USA), specify it with `--version`.

- Build:

  ```sh
  ninja
  ```

Diffing
=======

Once the initial build succeeds, an `objdiff.json` should exist in the project root.

Download the latest release from [encounter/objdiff](https://github.com/encounter/objdiff). Under project settings, set `Project directory`. The configuration should be loaded automatically.

Select an object from the left sidebar to begin diffing. Changes to the project will rebuild automatically: changes to source files, headers, `configure.py`, `splits.txt` or `symbols.txt`.

![](assets/objdiff.png)

Credits
=======

Some parts of this repository were taken from other decomp projects:
* Dolphin OS: [dolsdk2004](https://github.com/doldecomp/dolsdk2004), [dolsdk2001](https://github.com/doldecomp/dolsdk2001), [pikmin2](https://github.com/projectPiki/pikmin2)
* Dolphin AI, PAD: [dolsdk2004](https://github.com/doldecomp/dolsdk2004)
* THP: [pikmin2](https://github.com/projectPiki/pikmin2), [prime](https://github.com/PrimeDecomp/prime)
* RenderWare (DWARF symbols): [bfbb](https://github.com/bfbbdecomp/bfbb/tree/main/dwarf)
