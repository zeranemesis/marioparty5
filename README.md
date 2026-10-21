Mario Party 5  
[![Build Status]][actions] [![Progress]][progress site] [![DOL Progress]][progress site] [![RELs Progress]][progress site] [![Discord Badge]][discord]
=============

[Build Status]: https://github.com/mariopartyrd/marioparty5/actions/workflows/build.yml/badge.svg
[actions]: https://github.com/mariopartyrd/marioparty5/actions/workflows/build.yml
[Progress]: https://decomp.dev/mariopartyrd/marioparty5.svg?mode=shield&measure=code&label=Code&category=all
[DOL Progress]: https://decomp.dev/mariopartyrd/marioparty5.svg?mode=shield&measure=code&label=DOL&category=dol
[RELs Progress]: https://decomp.dev/mariopartyrd/marioparty5.svg?mode=shield&measure=code&label=RELs&category=modules
[progress site]: https://decomp.dev/mariopartyrd/marioparty5
[Discord Badge]: https://img.shields.io/discord/994839212618690590?color=%237289DA&logo=discord&logoColor=%23FFFFFF
[discord]: https://discord.gg/T4faGveujK

A work-in-progress decompilation of Mario Party 5, and a native port built from it
for Windows, Linux, macOS and Android.

This fork follows [mariopartyrd/marioparty5](https://github.com/mariopartyrd/marioparty5)
for the decompilation itself, and adds the port on top of it: the same sources build
either the original GameCube executable, byte for byte, or a native program that runs
on a modern machine.

This repository does **not** contain any game assets or assembly whatsoever. An existing copy of the game is required.

Supported versions:

- `GP5E01_00`: Rev 0 (USA)

`GP5P01_00` (Europe) has experimental configuration metadata. Its 135 original
file hashes were checked locally, but a matching PAL rebuild has not been
validated. The verified matching-build target remains `GP5E01_00`.

Verified on 2026-10-21: the USA DOL and all 134 RELs rebuild with their original
SHA-1 hashes (`135 files OK`). Across the full executable and modules, **30.04%
of code matches** (2,848,524 / 9,481,492 bytes), while **20.89% is linked from
compiled sources** (349 / 881 units). Unfinished units still use extracted
objects. See [the latest GPT-6 Luna batch](docs/decomp-luna-2026-10-21-lot2.md),
[the publication audit](docs/publication-2026-10-20.md), and
[the generated progress report](docs/avancement.md).

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
  git clone https://github.com/zeranemesis/marioparty5.git
  ```

- Copy your game's disc image to `orig/GP5E01_00`.
  - Supported formats: ISO (GCM), RVZ, WIA, WBFS, CISO, NFS, GCZ, TGC
  - After the initial build, the disc image can be deleted to save space.

- Configure:

  ```sh
  python configure.py
  ```

  To use a version other than `GP5E01_00` (USA), specify it with `--version`.

- Build:

  ```sh
  ninja
  ```

Native port
===========

The port compiles the decompiled sources for the host instead of the GameCube, and
replaces the console around them: [Aurora](https://github.com/encounter/aurora) for
GX, VI, PAD, DVD and CARD, SDL3 for the window and the controllers,
[MusyX](https://github.com/AxioDL/musyx) for the sound, libco for the game's
processes. The player's own disc image is read at run time; nothing from it is in
this repository.

It follows the decompilation: a unit enters the port once it is `Matching` or
`Equivalent`, and a REL module once all of its units are, which
`tools/gen_files_cmake.py` turns into `files.cmake`. Modules that are not there yet
are simply absent at run time â€” the game falls back to the previous screen instead
of loading them (see `src/game/objmain.c`).

**State:** `mp5native.exe`, `dol.dll` and the 47 enabled REL modules all
compile and link on Windows (MSVC). The game does not boot to a playable state yet:
most minigames and boards are still `NonMatching`, so their overlays are skipped.
`docs/port_patterns.md` (French) records the rules every port change follows;
`docs/decomp_workflow.md` (French) is the method for decompiling a module.

Building the port:

```sh
git submodule update --init --recursive
git -C extern/musyx apply ../../patches/musyx-mp5.patch
python tools/build_port.py            # Windows (MSVC), Linux or macOS
```

`tools/build_port.py` regenerates `files.cmake`, configures the CMake preset for the
host and builds it. The CMake presets can also be used directly, for instance
`cmake --preset linux-default-relwithdebinfo`. Android is built in CI
(`.github/workflows/port.yml`), which also builds the three desktop platforms.

Diffing
=======

Once the initial build succeeds, an `objdiff.json` should exist in the project root.

Download the latest release from [encounter/objdiff](https://github.com/encounter/objdiff). Under project settings, set `Project directory`. The configuration should be loaded automatically.

Select an object from the left sidebar to begin diffing. Changes to the project will rebuild automatically: changes to source files, headers, `configure.py`, `splits.txt` or `symbols.txt`.

![](assets/objdiff.png)
