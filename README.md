# IoTreat — IoT Smart Pet Feeder

**Academic Internet of Things project | 2024**  
**Project team:** Masoumeh Nejati Moharrami and Parsa Khazraei

IoTreat is a university project in which we built a prototype automated pet feeder with scheduled feeding, weight sensing, motorized dispensing and a sliding food tray. Our original documentation also describes an ESP8266-connected web interface for setting feeding preferences and controlling the tray.

This repository combines **recovered original materials** and a clearly separated **later maintenance copy** of the Arduino program. It is an academic archive, not a validated production-ready device.

![IoTreat prototype](docs/photos/prototype-front.png)

## Key features

- **Scheduled portions:** food dispensing controlled by a real-time clock and a configured target weight.
- **Weight sensing:** load cell and HX711 amplifier.
- **Motorized mechanisms:** servo actuation and stepper-driven tray movement.
- **Display:** I²C LCD for time, weight and status information.
- **Serial controls:** `SET_TARGET`, `EJECT_CONTAINER`, `INSERT_CONTAINER`, `TARE_SCALE`.
- **Remote control concept and implementation:** ESP8266 Wi-Fi, NTP and web-server behavior described in the original presentation; complete ESP8266 firmware is not included here.

## Prototype photographs

| Device | Device and web UI |
|---|---|
| ![Built feeder](docs/photos/prototype-front.png) | ![Feeder and computer](docs/photos/prototype-and-laptop.png) |

![Construction process](docs/photos/construction-process.png)

## Hardware

The 2024 presentation lists an Arduino/ATmega328P controller, an ESP8266-compatible board, SG90 servo, stepper motor, load cell, HX711, LCD, sliding rail, belt/pulley and enclosure materials. The archived Arduino sketch also imports `DS1307RTC` and uses a `Stepper` library. See the original presentation for the full component inventory.

![Original circuit schematic](docs/circuit-diagram.png)

## System overview

1. Feeding schedule and desired portion are configured by serial command (the original presentation also depicts a browser interface through ESP8266).
2. The Arduino reads the RTC and weight sensor.
3. At the requested time, the servo operates until the target portion condition is reached.
4. The stepper drives the food tray outward, and subsequent logic can return it.

This describes the recorded project design and control flow, **not** an independently verified contemporary test.

## Repository layout

```text
original/IoTreatFinal.ino                  Original recovered 2024 Arduino source (unchanged)
firmware/arduino/IoTreatController.ino     Later maintenance copy: small bug fix, untested
firmware/esp8266/README.md                Recovery notes; source unavailable in package
docs/photos/                               Original prototype and build photos
docs/circuit-diagram.png                   Circuit schematic extracted from presentation
docs/presentations/                        Original university presentation and brochure
CHANGELOG.md                               Exact modifications since original source
PUBLISHING-CHECKLIST.md                    Security and accuracy checks
```

## Reproducing and limitations

This repository does not contain the original ESP8266 source, hardware test fixture, calibrated load-cell settings, board definition or full wiring/setup instructions. The Arduino sketch depends on Servo, HX711, LiquidCrystal_I2C, DS1307RTC and Stepper libraries. Library compatibility, board wiring, serial wiring, calibration, timekeeping and motion safety need verification before hardware use. **Do not assume that simply uploading the sketch to a feeder will operate it safely.**

Known original-code issues include a shadowed target-weight variable (fixed only in the later maintenance copy), RTC time being set from compilation time during startup, and blocking stepper movement.

The ESP8266 file on the historical Gist reportedly exposes a Wi-Fi password; avoid republishing that credential.

## Source material and attribution

The project was completed collaboratively by **Masoumeh Nejati Moharrami and Parsa Khazraei** for an undergraduate IoT course in 2024. The presentation includes prototype photos, a circuit diagram, components, and excerpts of Arduino and ESP8266 implementation. Individual contributions should not be inferred from authorship of this repository alone.

Original historical code reference: [IoTreat GitHub Gist](https://gist.github.com/masinejati/910b18335b36653a8483f609a42ca6c1).

See [`CHANGELOG.md`](CHANGELOG.md) to distinguish contemporary edits from the university submission.
