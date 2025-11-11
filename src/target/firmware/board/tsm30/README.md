# Vitelcom TSM30 Board Support

This directory contains board support for the Vitelcom TSM30 GSM phone, which is based on the TI Calypso chipset with TWL3025 analog baseband.

## Hardware Overview

- **CPU**: TI Calypso ARM7TDMI
- **ABB**: TI TWL3025 Analog Baseband
- **RF**: LMX2326 RF Transceiver
- **Display**: Renesas R61505 128x160 RGB565 LCD
- **Keypad**: 5x5 matrix
- **Flash**: External NOR Flash (typically 8MB Spansion S71PL064J)
- **Clock**: 13MHz ARM clock (different from standard 104MHz)

## Building for TSM30

To build firmware for the TSM30:

```bash
cd src/target/firmware
make BOARD=tsm30 APP=layer1
```

Or build all applications:

```bash
cd src/target/firmware
make BOARD=tsm30
```

## Key Features

### 13MHz Clock Support
The TSM30 uses a 13MHz ARM clock configuration instead of the standard 104MHz used by other Calypso boards. This is configured in `board_init()`.

### Memory-Mapped LCD
The R61505 display uses a memory-mapped interface rather than I2C/UWire like other boards:
- Command address: `0x400000`
- Data address: `0x400001`

### LMX2326 RF Driver
The TSM30 uses LMX2326 RF transceiver instead of the standard TRF6151. The driver supports:
- Frequency synthesis for GSM900/DCS1800
- Gain control
- Power management

### RF Calibration
The RF calibration tables (`rf_tables.c`) contain placeholder values that need to be extracted from actual TSM30 hardware for proper operation.

## Hardware Connections

### Keypad Matrix (5x5)
The TSM30 uses a 5x5 keypad matrix. The mapping is defined in `keymap.h`.

### GPIO Configuration
- GPIO 0: DSP_WAKEUP (output, high)
- GPIO 6: KEYPAD_DET (input)
- GPIO 9: CMOS_PWDN (output, high)
- GPIO 11: CMOS_RESET (output, high)

### Flash Configuration
The board automatically detects flash type and configures TIFFS accordingly. Currently supports Spansion S71PL064J (8MB).

## Current Status

- ✅ Board initialization
- ✅ LCD driver (R61505)
- ✅ RF driver (LMX2326)
- ✅ Keypad support
- ✅ Flash detection
- ⚠️ RF calibration (placeholder values)
- ⚠️ TSP strobe assignments need verification

## Known Issues

1. **RF Calibration**: The RF power levels and ramps in `rf_tables.c` are placeholder values and need to be extracted from actual TSM30 hardware.
2. **TSP Configuration**: The LMX2326 TSP strobe assignments may need adjustment based on actual hardware.
3. **Clock Stability**: The 13MHz clock configuration may need fine-tuning for optimal performance.

## Testing

For initial testing, use the `hello_world` application:

```bash
make BOARD=tsm30 APP=hello_world
```

This will verify basic board functionality including display output and serial communication.

## Contributing

If you have access to TSM30 hardware and can extract the actual RF calibration values, please contribute them to improve the implementation.