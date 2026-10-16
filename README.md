# Ticking Bomb

A loud, physical party game for festivals. A ball-shaped bomb ticks down while players race to solve quick puzzles on it: a sum, a color sequence, a dial, a hard shake. Solve it to score or survive. Stuck? Gamble on a pass and dump the bomb on someone else, but pick the wrong button and it blows up in your hands.

**Team:** Fardoush Hassan and David Olamide, REPLAY Erasmus Mundus Master's in Game Design.

**Status:** design done, hardware being gathered, PlatformIO project set up. No game code yet.

## Docs

- [REPLAY design document (Delivery A1)](docs/Design-Document-REPLAY.pdf)
- [Full working design doc](docs/Working-Design-Doc.pdf)
- [Design sketches](docs/sketches/)
- [Prototype photos](docs/prototype/)

## Stack

- ESP32 DevKit, one board inside the ball
- C++ with the Arduino framework
- PlatformIO in VS Code

## Getting started

1. Install [VS Code](https://code.visualstudio.com/) and the **PlatformIO IDE** extension. VS Code will suggest it when you open this folder
2. Install the USB driver for your ESP32's USB chip. It's printed on the small chip next to the USB port: **CP2102** (Silicon Labs driver) or **CH340** (WCH driver)
3. Clone this repo and open the folder in VS Code. PlatformIO downloads the ESP32 toolchain and libraries on first build, which takes a few minutes
4. Plug in the ESP32 with a **data** USB cable. Charge-only cables are common and won't show up
5. Click **Upload** (the arrow in the bottom bar), then **Serial Monitor** (the plug icon)

If it works, the blue onboard LED blinks and the monitor prints the chip info and a heartbeat every second.

**Upload stuck on "Connecting..."?** Hold the **BOOT** button on the board until the upload starts.

## Project layout

```
platformio.ini     board, framework and pinned library versions
include/Pins.h     every GPIO assignment in one place (provisional)
src/main.cpp       entry point. Currently a setup test only
docs/              design docs, sketches and photos
```

Planned firmware modules (Input, Game, Puzzles, Display, Audio, Lights, Haptics, Settings) are described in `CLAUDE.md`. They'll go under `src/` as they're built.
