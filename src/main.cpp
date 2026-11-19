#include <Arduino.h>
#include <tinysnore.h>

#define PRODUCTION

#define HOURS_TO_MILLIS(h) (h * 3600 * 1000)
// The WDT oscillator is imprecise (~10%), so actual on/off durations will drift.
constexpr uint8_t OUT_PIN = 0;
// Calibrate: 
// When using bench supply, original 1000 was adding 5 seconds per cycle.  1100 was losing about .5 second per cycle.
// When using CR2032 coin cell, original 1000 was losing about 1 second per cycle.  1100 was closer to accurate.
constexpr uint32_t ADJUSTED_ONE_MS = 1100;
#ifdef PRODUCTION
// Use 64-bit math: millis * 1000 overflows uint32_t for multi-hour durations.
constexpr uint32_t ON_TICKS = (uint64_t)HOURS_TO_MILLIS(6UL) * 1000 / ADJUSTED_ONE_MS;  // production: on for 6 hours
constexpr uint32_t OFF_TICKS = (uint64_t)HOURS_TO_MILLIS(18UL) * 1000 / ADJUSTED_ONE_MS;  // production: off for 18 hours
#else
constexpr uint32_t ON_TICKS = 20000UL * 1000 / ADJUSTED_ONE_MS;    // testing, on for 20 seconds
constexpr uint32_t OFF_TICKS = 40000UL * 1000 / ADJUSTED_ONE_MS;   // off for 40 seconds
#endif

void setup() {
    pinMode(OUT_PIN, OUTPUT);
    for (int i = 1; i < 5; i++) {
        pinMode(i, INPUT_PULLUP);
    }
}

void loop() {
    digitalWrite(OUT_PIN, HIGH);
    snore(ON_TICKS);
    digitalWrite(OUT_PIN, LOW);
    snore(OFF_TICKS);
}
