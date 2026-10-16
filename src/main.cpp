// Setup test. Proves the toolchain, upload and serial monitor work.
// If the blue onboard LED blinks and the monitor prints a heartbeat,
// the project is set up correctly. No game code yet.

#include <Arduino.h>
#include "Pins.h"

namespace
{
    constexpr uint32_t BlinkIntervalMs = 500;
    uint32_t LastToggleMs = 0;
    bool LedOn = false;
}

void setup()
{
    Serial.begin(115200);
    pinMode(Pins::StatusLed, OUTPUT);

    Serial.println();
    Serial.println("Ticking Bomb: setup test");
    Serial.printf("Chip: %s rev %d, %d cores, %lu MHz\n",
                  ESP.getChipModel(), ESP.getChipRevision(),
                  ESP.getChipCores(), (unsigned long)ESP.getCpuFreqMHz());
}

void loop()
{
    const uint32_t Now = millis();
    if (Now - LastToggleMs >= BlinkIntervalMs)
    {
        LastToggleMs = Now;
        LedOn = !LedOn;
        digitalWrite(Pins::StatusLed, LedOn ? HIGH : LOW);
        if (LedOn)
        {
            Serial.printf("Heartbeat: %lu s\n", (unsigned long)(Now / 1000));
        }
    }
}
