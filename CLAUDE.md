# Ticking Bomb: project brief for Claude

Read this first. It carries over everything decided in the original planning chat (Oct 14 to 16, 2026), so a new session can pick up where that one left off.

## Who and what

- **Team:** Fardoush Hassan and David Olamide, REPLAY Erasmus Mundus Master's in Game Design (Lusófona University, Lisbon). Group project.
- **The game:** a loud, physical party game for events and festivals like Amaze and GDC. A ball-shaped bomb ticks down while players solve quick puzzles on it. Solve to score or survive, or gamble on a pass to dump the bomb on someone else. Pick the wrong button and it explodes in your hands.
- **"Ticking Bomb" is a working title.** The real name is still open.
- **REPLAY Delivery A1 is due 19 October 2026:** short description (250 to 400 characters), player interaction, playful approach, reflection on the object and play, and a mockup of the game space. The finished submission is `docs/Design-Document-REPLAY.pdf`.

## How David wants to work

- **Ask before writing game code.** On Oct 16 David asked for the PlatformIO project skeleton only: `platformio.ini`, `include/Pins.h` and a setup-test `src/main.cpp`. No game logic exists yet. Build module by module, only when asked.
- Natural, conversational tone. Contractions, short sentences. No em-dashes, no AI-sounding phrasing.
- Targeted edits, not full rewrites.
- Be honest and critical. Don't mince words or invent facts. If something from a photo or memory is unconfirmed, say so.
- David is a strong C++ developer (Unreal Engine 5: gameplay, UI, networking, AI). Explain hardware and electronics clearly; don't over-explain C++.

## Design pillars

1. **Panic, not paralysis.** Every decision is fast. Puzzles are short; the pressure makes them hard.
2. **The ball is the star.** All input happens on the ball, and the ball is the whole game.
3. **Loud and readable.** Every event shows on at least two feedback channels.
4. **Tweakable.** Timers, counts, odds and targets are settings, not code changes.
5. **Self-contained and portable.** No laptop, no base station, no Wi-Fi. Switch on and play.

## Game modes

### Solo (Rapid Fire). Build first.
Everyone plays together on one ball. No headbands.
- Countdown starts (starting point: 5 minutes, solve 10 puzzles to defuse; both tunable).
- Correct answer: bonus time, counter goes up. Wrong answer: timer is cut, new puzzle.
- **Skip:** pick one of the 4 pass buttons instead of solving. Safe = next puzzle, no time lost. Dark = bomb explodes, group loses.
- Hit the target: defused, group wins. Timer hits zero: group loses.

### Teams. Build second.
2 to 8 players in two teams, each wearing a fixed-color headband.
- Teams take turns. Each round the bomb tags a player on the team whose turn it is. A fresh timer starts, a little shorter each round.
- The tagged player can solve or pass at any time, even without trying first.
- **Correct solve:** +1 point. Next round goes to the other team.
- **Wrong answer:** timer is cut. Try again or pass.
- **Pass:** pick one of the 4 physical buttons. Safe = other team takes the bomb with the time left (no reset; the cut carries over). Dark = explodes on the passer.
- **Explosion** (timer out or dark button): that team loses 1 point, floored at 0. Next round goes to the other team.
- **First team to the points target wins the set.** One set per match.
- Puzzle difficulty is balanced across teams with some randomness. Exact sets to be worked out in playtesting.

### Individual. Proposed.
Every player for themselves. Elimination, no sets, so it runs longer.
- Bomb tags a random player. Correct solve: bomb goes to another player with a fresh, shorter timer. Wrong: timer cut. Pass: safe = another player gets the time left; dark = explodes on passer.
- Explode and you're out. Last player standing wins.
- **Open:** after a solve, random player or solver's choice? How many players does it need?

### Imposter. Parked, design later.
One hidden imposter. Ball passed around for voting on the imposter's color. Vote out an innocent: timer halves and they leave. Find the imposter: civilians win.

## Core mechanics

- **Tagging:** the bomb calls a color on its screen, LED strip and speaker at once. That player grabs the ball. Headbands are static in v1, so the voice and lights must carry the callout.
- **Solving:** answer on the ball, confirm with Enter. Wrong always cuts time.
- **Pass gamble:** always available, 4 physical buttons (chosen over an on-screen grid for feedback and pressure), 1 or 2 secretly dark (a setting).
- **Explosion:** full show on every channel: explosion audio, white strobe, long hard buzz, mockery line on screen.

## Puzzles (v1)

| Puzzle | Input | Notes |
| --- | --- | --- |
| Maths | Buttons | Short sum, 4 answers shown in a row matching the button order (the screens can't show color) |
| Number sequence | Buttons | Missing number, same 4-in-a-row layout |
| Color sequence | Buttons | LED strip flashes a sequence, repeat it on the colored buttons |
| Knob to position | Rotary encoder | Needle and target on screen, needle starts in a random spot |
| Shake the ball | Accelerometer | Fill a meter. Threshold must ignore normal passing |
| Shout at it | Microphone | **Cut for v1.** Too unreliable on a loud floor |

## Hardware

**Architecture:** one ESP32 inside the ball runs everything. Headbands are plain battery LEDs in fixed colors, no electronics to pair. Upgrade path if needed later: ESP32-C3 per headband, ESP-NOW broadcast straight from the ball (still no base station).

**Have:**
- ESP32 DevKit x2 (one brain, one spare), micro-USB
- 16x2 LCD with HW-061 I2C backpack (PCF8574) for puzzles and menus
- Grove quad alphanumeric display x2 (likely HT16K33, check the label) for the countdown
- Speaker + PAM8403 amp (about 3W per channel at 5V), fed from the ESP32 DAC
- Addressable RGB LED strip pieces
- Vibration motors (about 4)
- Arcade buttons (black, yellow, blue, red) plus small tactile buttons
- Rotary encoders x2 (KY-040 style, with push button)
- Round rocker switch
- Power bank

**Spare, not in the main build:** BBC micro:bit V2 (bench-test shake and drops, later the shout puzzle), PIR motion sensor (possible attract mode, can't sense falls), 12-key keypad, 12-channel capacitive touch board (I2C, possible touch pads on the shell), 8x8 LED matrix, Arduino Nano-style board on a screw shield.

**Need to buy:**
- Accelerometer: ADXL345 (has built-in free-fall detection) or MPU-6050
- NPN transistor (2N2222) or logic-level MOSFET + 1N4007 diode for the motor
- 330 ohm resistor (LED data line) and 1000 uF capacitor (across strip power)
- Optional: DFPlayer Mini + microSD, only for lots of voice and mockery lines
- Shell and headbands
- Possibly a logic level shifter if 5V parts glitch on 3.3V signals

**Gotchas:**
1. Power the LED strip and motor from the power bank, never from ESP32 pins. Common ground everywhere.
2. ESP32 is 3.3V logic; several modules are 5V. Usually fine; a level shifter fixes glitches.
3. Some power banks auto-shut-off at low current. Test for 30 minutes; get an always-on bank if it cuts out.
4. A speaker inside a padded ball loses volume. Test in a noisy room early.

## Software

- **Language:** C++ with the Arduino framework on the ESP32 (not an Arduino board; the API runs on the ESP32).
- **Tooling:** PlatformIO in VS Code. `platformio.ini` pins the board and library versions so both team members get the same setup. Arduino IDE only for quick part tests.
- **Skip:** ESP-IDF directly (overkill) and MicroPython (slower, gives up David's C++ strength).
- **Likely libraries:** LiquidCrystal_I2C, Adafruit LED Backpack or Seeed Grove (depends on display chip), FastLED, ESP32Encoder or AiEsp32RotaryEncoder, Adafruit ADXL345 or MPU6050, Preferences (built in, for saved settings), DFPlayer Mini library if that module is bought.
- **Inputs:** buttons via digitalRead with internal pull-ups and debounce; encoder via library using the ESP32 hardware counter; accelerometer over I2C (shares the bus with LCD and timer display).

**Planned firmware modules** (the game module never touches hardware directly; it sends requests like "play explosion"):

| Module | Job |
| --- | --- |
| Input | Buttons, encoder, accelerometer turned into events (pressed, turned, shake, picked up, set down) |
| Game | State machine, timer, puzzle picking, answer checks |
| Puzzles | One file per puzzle type, same interface: start, handle input, report solved or failed |
| Display | LCD menus and puzzles, alphanumeric countdown |
| Audio | Sound playback and volume |
| Lights | LED strip patterns: glow, drain, flash, strobe |
| Haptics | Vibration patterns: tick, buzz, long |
| Settings | Save settings and calibration to flash |

**Project setup (done Oct 16):**
- `platformio.ini`: env `ball`, `espressif32@7.1.3`, board `esp32dev`, Arduino framework, monitor at 115200. Platform ships Arduino core 2.0.17 and GCC 8.4. Libraries pinned exactly: FastLED 3.7.8 (3.10+ fails to compile on GCC 8.4, confirmed on David's first build), ESP32Encoder 0.12.0, LiquidCrystal_I2C from GitHub tag 1.1.3. Accelerometer, timer display and DFPlayer libraries are listed as comments until the parts are confirmed.
- `include/Pins.h`: all GPIO assignments in `namespace Pins`. **Provisional**: check against the real board before wiring. Avoids strapping pins (0, 2, 12, 15) except the onboard LED on 2, and flash pins (6 to 11).
- `src/main.cpp`: setup test only (blink + serial heartbeat).
- **Build status (Oct 16):** builds, uploads and runs on David's PC. The setup test's blue LED blinks. FastLED 3.10.7 had failed to compile (looser throw specifier errors), fixed by pinning 3.7.8. Claude's sandbox can't reach the PlatformIO registry, so builds are only verified on David's PC.
- **David's board:** 30-pin ESP32 DevKit, DOIT DevKit V1 layout, ESP32-WROOM-32 module, CP2102 USB chip (Silicon Labs driver installed), shows up as COM5. Labels: GPIO16 = RX2, GPIO17 = TX2, others printed as D2, D4 and so on. Every pin in `Pins.h` exists on it. Red LED = power; blue LED on GPIO2 sits beside it.
- **Arduino IDE works too:** board "ESP32 Dev Module", ESP32 core 2.0.17 (matches PlatformIO). Serial Monitor must be set to 115200. A button test on GPIO16 (RX2) printed correctly on Oct 16. His PC also has Bluetooth serial ports on COM3 and COM4, which can confuse auto-detect. Never commit `upload_port` or `monitor_port` to `platformio.ini`; they differ per machine.
- Code style (proposed, not yet confirmed by David): Unreal-like naming (PascalCase for types, constants and locals) since that's what he writes daily. Arduino's own functions stay as they are.

**State machine:** Idle, Setup, Tagging (not in Solo), Puzzle, Pass select, Exploded, Defused (Solo only), Results. Countdown runs during Tagging, Puzzle and Pass select.

## Setup and calibration (all on the ball: knob to scroll, Enter to select)

- **Setup:** switch on, pick mode, pick colors in play (Teams/Individual), run mode check, set the ball down and **hold** Enter to arm.
- **Host menu:** long-press Enter from the main menu. Settings persist across power-off.
- **Daily hardware check:** buttons, encoder direction and press, shake threshold (still / normal pass / hard shake), pickup and set-down, screens, LEDs, vibration, volume, battery.
- **Solo check:** difficulty preset, puzzle mix, one practice puzzle of each type.
- **Teams check:** assign colors to teams, callout check (each player confirms they hear their color), pass risk, timer, points target.
- **Individual check:** as Teams without the team split.

## Tunable settings

Countdown length and puzzles to defuse (Solo), bonus time per solve, starting timer, shrink per round, minimum timer, wrong-answer cut, pass/skip risk (1 or 2 of 4), points target, explosion penalty (-1, floor 0), puzzle mix, difficulty, shake threshold, knob tolerance, volume.

## Open and parked

- **Open:** Individual mode details (above). Balance values, all set in playtesting.
- **Parked until after prototyping:** game name (must read clearly as a toy for event listings); screen setup (LCD + alphanumeric, or something else) and confirming part counts, which were read from photos; where the screen sits on the ball and how it's protected; whether the voice callout is loud enough to replace headbands that light up.

## Files in this repo

- `docs/Design-Document-REPLAY.pdf`: the REPLAY Delivery A1 submission
- `docs/Working-Design-Doc.pdf`: the full working doc, including the one-week build plan and parts inventory
- `docs/sketches/`: the original notebook design pages
- `docs/prototype/`: bench prototype photos
- `docs/parts/`: photos of individual parts identified so far

The live, editable versions of both docs are Claude Docs artifacts:
- Working doc: https://claude.ai/code/artifact/8c65c04c-2bd5-4b40-bc18-978db05509d2
- REPLAY doc: https://claude.ai/code/artifact/7e1a7c04-6e51-4ea1-8618-c74b8a072d17
