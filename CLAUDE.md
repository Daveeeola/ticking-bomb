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
- Motion sensor board, purple, 8 pins (chosen Oct 19 over the micro:bit for shake, pickup, set-down and drops). From a photo it looks like a GY-91 (MPU-9250 or MPU-6500 plus BMP280 pressure sensor); not confirmed yet. Plan: I2C on the LCD's bus (SDA D21, SCL D22), VIN to 5V, read registers directly rather than through a library, since clone chips often fail library ID checks.

**Spare, not in the main build:** BBC micro:bit V2 (not used for motion any more; possible shout puzzle later), PIR motion sensor (possible attract mode, can't sense falls), 12-key keypad, 12-channel capacitive touch board (I2C, possible touch pads on the shell), 8x8 LED matrix, Arduino Nano-style board on a screw shield.

**Need to buy:**
- ~~Accelerometer~~: covered by the purple motion board above, unless it turns out faulty
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
- `docs/wiring/bench-lcd-button-led.html`: bench schematic for the LCD (SDA D21, SCL D22, VCC on the 5V rail from VIN), button on RX2/GPIO16 and LED on D33 with a 330 ohm resistor. Live version: https://claude.ai/artifact/9uWQkzVuyBcbpDF7L1FDR5 (as of Oct 19 that page shows the v2 multiplexer wiring, which David has NOT built yet; he is still on the original wiring)
- Core board page (21 Oct, version 6): https://claude.ai/artifact/Y1UUqcZDJxtVCweNMdNEdo. 5x7 cm perfboard, columns 01-18 along the top, rows A-X down the RIGHT edge (as printed). **The ESP32 is now soldered to it** (13:34 photos, despite the earlier socket plan): 3V3 side column 05, VIN side column 14, pins rows F-T, USB at the top. The DevKit PCB covers columns 04-15 rows C-U, the USB socket columns 07-11 rows A-C; nothing goes there. Layout v6: LCD/MOTION/OLED/MATRIX column 02 (D-G, I-L, N-Q, S-W); MUX/KNOB1/KNOB2 column 17 (F-M, O-R, T-W); row B: KEYPAD columns 01-03, POWER IN 14-15 (VIN two holes below it), 1000 µF at 17-18; row V: AMP 07-09, MOTOR 12-13; row W: 10k (03-04), 1k (05-06), 1 µF (07-08, + at 08), 2N2222 E-B-C at 11-13; row X: STRIP 04-06, 220 Ω 09-10, base 1k at 12, 1N4007 14-15 (stripe at 14). Front, mirrored back and wired back views plus a 60-wire list. v13 of the game sketch (12:31): knob-2 glitch guard raised to 20000 edges/s and self-healing (1 s pause, re-attach) because the 3000 limit was switching knob 2 off for good mid-puzzle. 13:41-13:55 updates (page versions 7-11): MUX header reordered to the module's own order SIG S3 S2 S1 S0 EN VCC GND; David is replacing the 4x3 keypad with a 1x4 membrane keypad (Mauser 096-7973, keys on mux C0-C3, common to GND) and adding a Keyestudio KS0035 analog sound sensor (MIC header row B 01-03: S → D33 ADC, 3V3, GND; RX2 and D4 now free); knob headers are 5-pin in KY-040 order CLK DT SW + GND (KNOB1 17 O-S, KNOB2 17 T-X) with a 2-pin SW1/SW2 header at 17 C-D cabled to mux C5/C12; 62 wires. New page section "Mux module: the keypad and buttons" (wiring4/mux_plugs.py) shows every mux channel. **Code not yet updated for the 1x4 keypad (digits 1-4 only, no #) or the mic; puzzles need a redesign before that keypad is usable.** 14:04-14:30 (page versions 12-14): row letters drawn on both edges, A at top on the left, X at top on the right (as printed on his board; all row names on the page use A-at-top). **LCD dropped: the OLED replaces it** (no LCD header; sketch still writes menus/results to the LCD, to be moved to the OLED in the next code pass). Left headers now in the modules' own pin order: MOTION 8-pin at 02 D-K (VIN 3V3 GND SCL SDA SDO NCS CSB, only VIN/GND/SCL/SDA wired; the board is an HW-612), OLED 02 M-P (GND VCC SCL SDA), MATRIX 02 R-V (VCC GND DIN CS CLK, MAX7219). 15:44-16:00 (versions 15-17): STRIP header reordered to 5V DIN GND (as printed on the strip; his wires: orange +5V, red Din, brown GND). AMP header is now 7-pin in the PAM8403 module's order GND VCC MUTE SHDN INL GND INR at row V 04-10 (only GND, VCC, GND, INR wired); 1 µF moved to row W 09-10 with + at 10 straight under INR. Separate amp/speaker image: scratchpad wiring4/amp_detail.py → amp-detail.png (board corner, 7-wire cable, amp, 100-470 µF across the amp's GND/VCC pins, speaker on the R screw terminal, plus a schematic). 16:21, version 18: **row V is also under the DevKit** (PCB covers columns 04-15 rows C-V), so nothing goes on V. Already soldered by David at this point: ESP32, STRIP header + 220 Ω, OLED, MIC, MOTION, both knob headers, MATRIX. Moved: AMP header to row B columns 12-18 (7-pin, module order; GND/VCC/GND/INR wired, VCC straight up to the 5V bus); POWER IN to row X 01-02; 1000 µF to row X 07-08 (+ at 07); row W now holds 10k (02-03), 1k (04-05), 1 µF (06-07, stripe 06, + 07, long wire to INR at 18B), MOTOR header 09-10, 2N2222 E-B-C at 12-14, 1N4007 at 15-16 (stripe 15); base 1k at 13X. 59 wires. amp-detail.png regenerated by wiring4/amp_detail2.py (two board fragments + cable + amp + schematic). Generated by scratchpad wiring4/coreboard_gen.py + coreboard_back.py + mux_plugs.py, run in that order (older versions kept as *_v4.py, *_v5.py).
- Motion board wiring (original wiring, shares SDA D21 / SCL D22 with the LCD): https://claude.ai/artifact/ESPmAg6oxu4QV9qff27TDr
- `docs/wiring/Reset-Protocol-Wiring.pdf` (20 Oct): the planned multiplexer build. Page 1 = whole wired schematic, page 2 = tag view, pages 3-4 = rewire steps and every connection. Colour buttons on mux C7-C11 (black, yellow, blue, red, green), Pass on C6 for later, D25 kept for ESP32 sound.
- Wiring v3 (20 Oct, not built yet, no PDF): same mux plan plus a 1.3" I2C OLED (SH1106 assumed, 0x3C, VCC on the 3.3V row, shares SDA D21 / SCL D22) and a second KY-040 knob (CLK VN/GPIO39, DT VP/GPIO36, SW on mux C12, + on 3.3V). Keypad columns RX2/D33/D4, rows mux C0-C3. Free pins: D25, D19, D12, D2. Live page: the wiring artifact above (version 16). Test sketch: hw_test.ino (needs U8g2), simulated only. Game sketch for v3: reset_protocol.ino + WebPage.h (UseMux = true by default, OLED as a second screen drawn in U8g2 page mode, knob 2 read and printed but not used by puzzles). Simulated only, not compiled for the ESP32 yet.
- Sound (20 Oct, wiring page version 17): no laptop. D25 (DAC) → 10 kΩ → sound row; sound row → 1 kΩ → GND and → 1 µF capacitor → PAM8403 INR (David has a 1 µF 50V electrolytic: + leg to INR, striped leg to the sound row). Speaker on R+/R−. David chose 8-bit sound only, no voice, to keep it compact. Game sketch now plays 8-bit sound itself: I2S built-in DAC, right channel only (D25; never i2s_set_pin(NULL), that also enables DAC2 = D26 = mux S3), 16 kHz, audio task on core 0, 3 voices (UI / clock ticks / events with priority), synthesized recipes, no samples. SFX lines still sent to the laptop/phone pages. Compiles for real now: Claude built an arduino-cli + ESP32 core 2.0.17 toolchain in its scratchpad (865 KB, 65% of the default partition). Not yet heard on hardware. 20 Oct 18:00: David reported the sound version "not working" and everything dimming, while the no-sound version worked. The sound code was purely additive (diffed), so the likely cause is power: the + rail runs off laptop USB via VIN, and the amp's current on top of the strip probably browns out the ESP32. Fix sent: brownout detection at boot (prints a warning, starts in low-power mode), StripBrightness 60 → 30, SoundVolume 80 → 60, DAC soft start, and sound disabled on the original wiring (D25 is a keypad wire there). Real fix: power bank on the rails, VIN jumper pulled. 20 Oct 18:20: David confirmed the speaker works (sound_test.ino). Full game sent with the vibration motor added: buzzes with events via Sfx() (start 120 ms, correct 80, wrong 300, warn 150, BOOM 900), off in low-power mode. 20 Oct 18:35: David found the two sound resistors had been swapped on the bench (1 kΩ in line, 10 kΩ to GND = ten times too much signal into the amp, a big part of the dimming). Wiring page v19 now draws colour bands and a multimeter check (sound row ≈ 0.15 V when silent; ≈ 1.5 V means swapped). Independent review of the sketch found no crash or pin conflict; fixes applied: WifiOn = false by default, FastLED power cap 300 mA, strobe at grey 140, quieter BOOM, BOOM buzz 500 ms, no buzz on warn, motion check ignores the motor while it runs (+150 ms), Start/Reset sounds always get through, audio stack 4096. Sketch now 368 KB. 20 Oct 18:45: David said that revised version didn't work at all. Rebuilt from his working no-sound file (= reset_protocol_v1_nosound) with ONLY sound added the sound_test way: no audio task, no queue, no motor, no brownout/low-power or FastLED cap. The loop tops up the I2S DMA buffer with non-blocking i2s_write (8 x 256 frames at 8 kHz = 256 ms cushion), audio started last in setup. Other changes: WifiOn false, StripBrightness 60 → 40. Scratchpad copy: reset-protocol/reset_protocol_v5_simple.ino. Motor stays off until the 1N4007 is fitted. 20 Oct 18:55: David sent "the last working code that has sound" but it is byte-identical to v1_nosound: it never drives D25, so what he heard was the amp amplifying noise picked up on the floating D25 line, not 8-bit sound. Sent v6 = his exact file + the in-loop sound (same as v5) with his own settings kept (WifiOn true, StripBrightness 60), plus a one-line restart-reason print and a knob-2 interrupt glitch guard (detaches VN/VP interrupts if >3000 edges/s). Still unknown why v2-v5 "don't work at all": need his Serial Monitor output and a description of the symptom. 20 Oct 19:05: power ruled out (amp unplugged and SoundOn=false both still dead). 19:10: his upload log shows the Arduino IDE now uses ESP32 core **3.3.12** (path packages\esp32\hardware\esp32\3.3.12, sketch 1,064,183 bytes = 81%), not 2.0.17 as on Oct 16. All of Claude's verified builds are on 2.0.17 (865 KB). Core 3.x is IDF 5 and the legacy driver/i2s.h is deprecated there; main suspect for the dead board. **19:19: CONFIRMED. Downgrading the IDE board package to 2.0.17 fixed it; v6 runs with sound on the bench.** Core 3.x (IDF 5) breaks this sketch; both team members must stay on 2.0.17. Next issue: constant hum from the speaker. Sent v7 = v6 + WifiOn false + strip redrawn every 33 ms instead of every loop (fewer supply current pulses). Hardware advice: 100 µF+ capacitor across amp VCC/GND, amp on its own power, keep speaker/D25 wires away from the strip data and I2C wires, test with laptop on battery for mains hum. Capacitor wiring page (parallel caps across amp VCC/GND, schematic + breadboard view): https://claude.ai/artifact/C5D9MhEE7jKzQ7uVEXcCP9. 19:50: David wants it louder. v8 = v7 with SoundVolume 100 and voice amplitude 90 → 125 (about +7 dB, the most the code can give). Next step is hardware: swap the 1 kΩ for 2.2 kΩ (+6 dB), max 3.3 kΩ. The 8 Ω 0.5 W speaker is the real ceiling; a 3 W 8 Ω or 4 Ω speaker is the route to event volume. 21 Oct 10:45: v8 works (sound OK) but the LED strip stays solid white. All-white on WS2812 = bits read as all 1s: either the strip's data timing is being stretched (FastLED's own RMT ISR delayed by the new I2S DMA interrupts) or a supply/ground problem on the strip. Sent v9 = v8 + `#define FASTLED_RMT_BUILTIN_DRIVER 1` before FastLED.h (waveform prebuilt, immune to ISR delays). **11:03 RESOLVED: the IDE had FastLED 3.10.7, which compiles on core 2.0.17 but drives the strip as all-white. Installing FastLED 3.7.8 fixed it.** Everything works on the bench with v9: sound, strip, screens, keypad, knobs, motion. **Locked versions for both PCs: ESP32 board package 2.0.17 + FastLED 3.7.8. Decline IDE update prompts for both.** Still to do on hardware: capacitors across the amp for hum, 2.2 kΩ swap or a 3 W speaker for volume, motor diode before enabling the motor, power bank on the rails.
- 21 Oct 11:30, v10 (scratchpad reset-protocol/v10, built by puzzles_edit.py on v9): clock puzzles replaced by **KNOBS** (two target angles, KnobStepsPerTurn = 20 clicks/turn, random start, hold 300 ms; OLED draws two dials, LCD "T:072/036" / "N:342/342 TURN|HOLD"), **COMBO** = knobs then 4-digit code. New colour puzzles on the 5 colour buttons (mux C7-C11, order black/yellow/blue/red/green, letters K/Y/B/R/G): **SEQUENCE** (strip plays 4 colours, repeat on buttons, wrong = replay), **COLOURS** (pairs like "R8 G0 B7": colour button then digit), **COUNT** (strip filled with random colours, "COUNT RED?" type the number, # checks). Black excluded unless BlackInPuzzles = true. **PASS gamble** on C6: press PASS, strip shows 5 colour blocks, pick one; PassDarkCount (1) of 5 is dark = BOOM "DARK BTN!", safe = cooldown to next player, no point. All simulated on the host (every puzzle solved, pass safe + dark), ESP32 compile 370 KB. Not yet played on hardware. Wi-Fi page (WebPage.h) not updated for the new puzzle names.
- 21 Oct 11:50, **v11** (reset-protocol/v11, puzzles_v11.py on v10), per David: short instructions, the first 13 LEDs are "slots" (LED 1 = nearest the wires, setting PuzzleLeds), colours written Y / Blu / Blk / R / G on screens, PASS just passes the turn (dark-button gamble removed). LCD top = 13-char instruction + seconds ("COUNT YELLOW? 10"); OLED = big instruction + two hint lines (≤20 chars) + entry echo or slot boxes. Puzzles: CODE "TYPE 28973612", MATH "28 / 4 = ?", KNOBS "KNOBS 108 090" / bottom "YOU 180 036 TURN|HOLD", COMBO, ORDER "COPY STRIP ->" (slots show up to 13 colours, press buttons in order, solved slots turn white, wrong = progress resets), COLOUR+NUM "Blu6 G1 G9" (colour button then digit; generated to fit 13 chars), COUNT "COUNT BLUE?" (all 13 slots lit, asked colour appears ≥1), LED COLOUR "COLOUR OF #5?" (press the colour of slot 5), FIND LED "GREEN AT #?" (exactly one slot has it, type its number). Timer bar on LEDs 14-41 during slot puzzles, echoes the pressed colour for 200 ms. All simulated, ESP32 compile 371 KB, not played on hardware yet.
- 21 Oct 12:00, **v12** (reset-protocol/v12, v12_edit.py on v11): per David, less text and bigger: OLED hint lines removed; the thing to do is drawn in 10x20 (falls back to 7x14B if wider than 120 px), the entry echo in 7x14B bold; LED COLOUR text is now "SLOT 5 = ?". The first 13 LEDs are never part of the timer bar any more: off when no puzzle uses them (SlotsIdleWhite = true → dim white); the bar is always LEDs 14-41. David reported the COUNT colours "disappearing when the countdown starts": that was the v10 behaviour where the whole strip was shared; from v11 on the slots stay lit for the whole round.

- 21 Oct 12:10, **moving into the ball.** Shell = a blue/green Phlat Ball (collapsible disc-ball with a spring hub and slotted panels; panels unclip). David has a 7 x 9 cm double-sided perfboard (18 x 24 holes, bus pad strips top and bottom). Plan agreed in chat: a "core board" on that perfboard (ESP32 in two 15-pin female sockets, one labelled male pin header per peripheral, 220 Ω / sound divider / 1 µF / NPN + diode / 1000 µF on the board, 5V and GND on the pad strips), mux and amp stay as modules on their own plugs, every button plugs straight into the mux C4-C12 with chained GND, power from the bank via a USB breakout and the rocker switch into the 5V bus + VIN. Layout page: https://claude.ai/artifact/Y1UUqcZDJxtVCweNMdNEdo (positions are a plan, not measured). v3 of the page: board labels are the perfboard's own (columns 01-18, rows A-X), ESP32 sockets at columns 05 and 15 rows C-Q, mux col 02 C-J, knob 1 col 02 L-O, knob 2 col 02 Q-T, LCD/MOTION/OLED/MATRIX all in column 18 (C-F, H-K, M-P, R-V), KEYPAD/AMP/MOTOR on row T, POWER IN row X 04-05, STRIP row X 15-17 with the 220 Ω on row W. Page has front view, mirrored back view, back view with all 60 wire runs drawn, and a wire list by column+row. Generators: wiring4/coreboard_gen.py (front) and coreboard_back.py (back + wires). Test order: core board on the bench with the existing test sketches → power bank 30-min test → mount in the shell panel by panel → shake/drop test closed. No code changes requested.

The live, editable versions of both docs are Claude Docs artifacts:
- Working doc: https://claude.ai/code/artifact/8c65c04c-2bd5-4b40-bc18-978db05509d2
- REPLAY doc: https://claude.ai/code/artifact/7e1a7c04-6e51-4ea1-8618-c74b8a072d17
