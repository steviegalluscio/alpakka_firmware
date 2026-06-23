# Alpakka Lite Firmware

*Fork of Alpakka controller reference firmware (for Alpakka Lite with Raspberry Pi Pico)*
## Project links
- [Alpakka Lite Docs](https://github.com/steviegalluscio/alpakka-lite).
- [Alpakka Lite Firmware](https://github.com/steviegalluscio/alpakka_firmware/tree/alpakka-lite). _(you are here)_

## Supported developer operative systems
- GNU/Linux (and MacOS) - See [Development in Linux](https://inputlabs.io/devices/alpakka/manual/dev_unix).
- Windows - See [Development in Windows](https://inputlabs.io/devices/alpakka/manual/dev_windows).

## System dependencies
With `apt`, `rpm`, `pacman`, `brew`, or the equivalent package manager of your system, install:
- **gcc**
- **git**
- **cmake**

## Project dependencies
- `DEVICE=<device> make install`: Download and configure dependencies automatically.

## Compilation targets
- `alpakka_lite`: Alias for `alpakka_lite_dsv`.
- `alpakka_lite_dsv`: [Alpakka Lite](https://github.com/steviegalluscio/alpakka-lite) with LSM6DSV and Raspberry Pico.
- `alpakka_lite_ds3`: [Alpakka Lite](https://github.com/steviegalluscio/alpakka-lite) with LSM6DS3 and Raspberry Pico.

Example usage:
```
DEVICE=alpakka_lite make
```
```
DEVICE=alpakka_lite CFG_TICK_FREQUENCY=500 CFG_IMU_TICK_SAMPLES=64 make
```

## Development commands
- `DEVICE=<device> make`: Build compilation environment and build executables.
- `make rebuild`: Build executables again using cache (faster).
- `make load`: Load built .uf2 file into the Pico (requires bootsel mode or active session).
- `make reload`: Do both `rebuild` and `load` commands (for dev convenience).
- `make clean`: Delete previous build files.
- `make session`: Connect to UART serial stdio, and display controller log.

While having an active session:
- `make restart`: Restart the controller.
- `make bootsel`: Put the controller in bootsel mode.
- `make calibrate`: Calibrate thumbstick and IMUs.
- `make format`: Format NVM sector and reset to initial values.
- `make test`: Start a semi-manual testing procedure for the buttons and axis.

## Devkit button
- Single press: Restart the controller.
- Double press: Put the controller in bootsel mode.

Read the [Alpakka developer manual](https://inputlabs.io/devices/alpakka/manual/dev) for details about the **devkit** and more.
