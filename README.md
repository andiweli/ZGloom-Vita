# ZGloom-Vita – SDL port of Amiga **Gloom** for PS Vita / PSTV

> SDL-based port of the modern **ZGloom** engine, bringing the classic Amiga FPS **Gloom** (plus Gloom Deluxe, Gloom 3 and Zombie Massacre) to **PlayStation Vita** and **PSTV** as a native homebrew game.

[![Latest release](https://img.shields.io/github/v/release/andiweli/ZGloom-Vita-SDL?label=latest%20Vita%20release)](https://github.com/andiweli/ZGloom-Vita-SDL/releases/latest)
[![Platform](https://img.shields.io/badge/platform-PS%20Vita%20%2F%20PSTV-blue)](https://github.com/andiweli/ZGloom-Vita-SDL)
[![Engine](https://img.shields.io/badge/renderer-SDL2%20software-brightgreen)](https://github.com/andiweli/ZGloom-Vita-SDL)

**ZGloom-Vita** is an SDL port of [ZGloom](https://github.com/Swizpig/ZGloom) for the **PlayStation Vita / PSTV**.  
It is a fork of [JetStreamSham/ZGloom-vita](https://github.com/JetStreamSham/ZGloom-vita) with:

- integrated **launcher** for all Gloom-engine games
- integrated **cheat menu**
- **optimized and fixed renderer**
- additional **video options**, post-processing filters and UX tweaks

The goal is to keep the original Amiga Gloom feeling while making it easy and fun to play on modern handheld hardware.

---

## ✨ Key Features (What’s new in this fork?)

ZGloom-Vita now includes:

- 🧮 **Optimized and fixed renderer (SDL2 software)**  
  Cleaned-up and tuned renderer for smoother visuals and fewer glitches compared to early Vita builds.

- ⏱️ **Stable framerate options (50 / 30 FPS)**  
  In-game menu option to choose between **50 FPS** and **30 FPS**, both tuned for stable performance on PS Vita / PSTV.

- 🩸 **Improved visibility & blood logic**  
  Blood is now correctly **covered by walls** when enemies are beside or behind geometry, improving depth perception.

- 🔫 **Correct muzzle flash behavior**  
  Fixes a bug where moving sideways made the muzzle flash appear from the side instead of the weapon.

- 🎮 **Integrated launcher for all Gloom-engine games**  
  Launch directly into:
  - Gloom (Classic)  
  - Gloom Deluxe / Ultimate Gloom  
  - Gloom 3  
  - Zombie Massacre  

- 🕹️ **Cheat menu in the game options**  
  Integrated cheat selector (for example infinite health and photon weapon at start) to make the game more approachable on handheld.

- 🎛️ **Improved menu navigation & controls**  
  - DPAD navigation now **loops** through entries  
  - Circle = go back  
  - Square = value lower  
  - Cross = value higher  

- 🎨 **Atmospheric post-processing overlays**  
  Optional, fully configurable filters:
  - **Vignette**  
  - **Film grain**  
  - **Scanlines**  

- 🔊 **Higher-quality audio defaults**  
  `SDL_mixer` and LibXMP player set to **44 kHz** (previously 22 kHz) for cleaner in-game sound.

- 🧹 **General cleanup & polish**  
  - Unified `config.txt` and new default values  
  - Menu items and descriptions revised and optimized  
  - All compilation warnings eliminated  
  - Non-used dependencies removed  
  - Slightly smaller `.VPK` thanks to toolchain finetuning

---

## 🕹️ What is Gloom?

[Gloom](https://en.wikipedia.org/wiki/Gloom_(video_game)) was a 1995 Doom-like first-person shooter from **Black Magic Software** for the Commodore Amiga.  
It featured very messy and meaty graphics, and at the time required a powerful Amiga (an A1200 with 030 CPU was still on the low end).

The engine later powered several successors and conversions, including:

- **Gloom Deluxe / Ultimate Gloom** – graphically enhanced version of the original  
- **Gloom 3**  
- **Zombie Massacre**  
- Several full-game conversions of other 90s Amiga titles

ZGloom is a modern reimplementation of this engine, and **ZGloom-Vita** is the SDL-based PS Vita / PSTV port of that reimplementation.

---

## 🖼️ Screenshot

A mockup of ZGloom-Vita running on PS Vita:

![ZGloom-Vita – PS Vita gameplay mockup](https://github.com/user-attachments/assets/98efe43e-9a7a-4fc7-87bf-7463df071cb5)

---

# Instructions

## 🎮 How to play on PS Vita / PSTV

Gloom was made freely available by its developers (see the original links in the upstream project).  
For the other official games using the Black Magic **Gloom** engine, such as **Gloom 3** or **Zombie Massacre**, you can download them for free from Aminet.  
Alternatively, you can use the original game files from your own game installation.

Additionally, there is a download of pre-packed game files provided with permission from **Gareth Murfin** (see link in the original README).

### 1. Install the VPK

1. Download the latest `.vpk` from the Releases page.  
2. Install the `.vpk` on your PS Vita / PSTV using VitaShell or your preferred homebrew method.

### 2. Prepare the game data

Download the `.zip` of the game(s) you want to play, extract them on your PC, and copy **only the folders** (depending on which games you want) to the following locations on your Vita:

- `ux0:/data/zgloom/gloom`  
- `ux0:/data/zgloom/deluxe`  
- `ux0:/data/zgloom/gloom3`  
- `ux0:/data/zgloom/massacre`

Once the folders and data files are in the right place, start **ZGloom-Vita** from LiveArea and select the game you want from the integrated launcher.

---

## 🔊 In-game music (XMP modules)

ZGloom supports in-game music using any module format that **XMP** can play (for example `.mod` and other tracker formats).

1. Put your module files (mods) into the `sfxs` folder of the game data.  
2. Add a line such as the following to the script:

   song_blitz.mod

You can add **multiple `song_` commands**, which allows **per-level music** (each level can use a different module).

---

## 🛠 Building on Linux / WSL

You can build ZGloom-Vita yourself using **Linux** or **WSL (Windows Subsystem for Linux)** together with **VitaSDK**.

I am using Ubuntu on Windows with VitaSDK.

### 1. Install required packages

    apt-get install make git-core cmake python

### 2. Install VitaSDK

Follow the instructions on the [VitaSDK](https://vitasdk.org) site (see “VitaSDK Installation”):

    export VITASDK=/usr/local/vitasdk
    export PATH=$VITASDK/bin:$PATH    # add vitasdk tools to $PATH
    git clone https://github.com/vitasdk/vdpm
    cd vdpm
    ./bootstrap-vitasdk.sh
    ./install-all.sh

### 3. Install the LibXMP Vita library

Install the **LibXMP** Vita port (see the “porting libraries” section on the VitaSDK site for detailed installation instructions).

### 4. Build the .VPK

In the repository root, simply run:

    ./build.sh

This will generate the `.VPK` package ready to be installed on your PS Vita / PSTV.

---

## 📜 License & third-party code

The licensing situation around the original **Gloom** source is a bit unusual:

The Gloom source release says only the `.s` and `.bb2` files are open source, but the original executable includes:

- maths lookup tables (generated by the `.bb2` files)  
- bullet and sparks graphics  
- the Classic Gloom title screen

This port follows the original constraints and **does not** include the Classic Gloom title screen and similar executable-bundled assets.  
Instead, it may display alternative imagery such as the Black Magic image.

### Libraries and tools used

- **LibXMP** – MED / module playback  
  - http://xmp.sourceforge.net/  

- **SDL2** and **SDL2_mixer** – cross-platform media & audio (for sound)  
  - https://www.libsdl.org/  

- **DeCrunchmania** C code by Robert Leffman (licence unknown)  
  - http://aminet.net/package/util/pack/decrunchmania_os4  

- **VitaSDK** – open SDK for PS Vita homebrew  
  - https://vitasdk.org/

---

## ℹ️ About this fork

This repository is a **fork of ZGloom for PS Vita** from **JetStreamSham** with an SDL renderer.  
It supports:

- **Gloom (Classic)**  
- **Gloom Deluxe / Ultimate Gloom**  
- **Gloom 3**  
- **Zombie Massacre**

**Keywords / topics:**  
_amiga • gloom • vita • psvita • pstv • zgloom • gloomdeluxe • zombiemassacre • homebrew • retro fps • doom-like_

If you enjoy playing classic Amiga shooters on your Vita, feel free to ⭐ star the repository so other PS Vita and Amiga fans can discover ZGloom-Vita more easily.
