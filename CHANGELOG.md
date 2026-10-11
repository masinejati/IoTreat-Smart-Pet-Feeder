# Change log

## Repository maintenance edition (2026)

- Preserved the recovered Arduino code verbatim in `original/IoTreatFinal.ino`.
- Added `firmware/arduino/IoTreatController.ino` as a *separately labeled maintenance copy*.
- Fixed a local variable shadowing problem in `SET_TARGET` handling: the parsed weight is now assigned to the global target value.
- Updated the display of the parsed weight to use the new local variable.
- No hardware integration or compilation was performed.
- **Unresolved:** RTC time reset at startup, blocking stepper controls, long motor movement duration, serial command parsing and calibration behavior.

These edits should not be described as part of the original 2024 submission.
