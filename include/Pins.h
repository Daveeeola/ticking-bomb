#pragma once

// Provisional pin map for an ESP32 DevKit (ESP32-WROOM-32, 30 or 38 pin).
// Check every number against your board's printed labels before wiring.
//
// Pins kept free on purpose:
//   0, 2, 12, 15  strapping pins. They affect boot if pulled the wrong way
//   6 to 11       wired to the flash chip. Never use
//   34 to 39      input only, no internal pull-ups

#include <Arduino.h>

namespace Pins
{
    // Onboard blue LED on most DevKits. Used by the setup test only
    constexpr uint8_t StatusLed = 2;

    // I2C bus, shared by the LCD, the alphanumeric timer and the accelerometer
    constexpr uint8_t I2cSda = 21;
    constexpr uint8_t I2cScl = 22;

    // Colored buttons and Enter. Wired between the pin and GND,
    // read with INPUT_PULLUP, so a press reads LOW
    constexpr uint8_t ButtonBlack  = 16;
    constexpr uint8_t ButtonYellow = 17;
    constexpr uint8_t ButtonBlue   = 18;
    constexpr uint8_t ButtonRed    = 19;
    constexpr uint8_t ButtonEnter  = 23;

    // Rotary encoder (KY-040). The module has its own pull-ups on CLK and DT
    constexpr uint8_t EncoderClk = 4;
    constexpr uint8_t EncoderDt  = 13;
    constexpr uint8_t EncoderSw  = 14;

    // Addressable LED strip data line, through a 330 ohm resistor
    constexpr uint8_t LedStripData = 27;

    // Vibration motor, through a transistor or MOSFET with a flyback diode.
    // Never drive the motor straight from this pin
    constexpr uint8_t Motor = 32;

    // Audio out to the PAM8403 amp. GPIO25 is DAC1 on the original ESP32
    constexpr uint8_t AudioDac = 25;

    // Accelerometer interrupt (free-fall or shake alert on the ADXL345)
    constexpr uint8_t AccelInterrupt = 34;
}
