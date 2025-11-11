# TSM30 Development Guide

This guide provides information for developers working on the Vitelcom TSM30 board support.

## Architecture Overview

### Board Initialization (`init.c`)

The TSM30 board initialization follows this sequence:

1. **Watchdog Disable** - Disables the TSM30-specific watchdog
2. **Memory Configuration** - Sets up memory banks for internal/external flash and RAM
3. **Clock Configuration** - Configures 13MHz ARM clock (unique to TSM30)
4. **RHEA Bridge** - Configures the bus bridge
5. **GPIO Initialization** - Sets up board-specific GPIO pins
6. **Bootrom/Exceptions** - Enables bootrom and installs exception handlers
7. **Interrupt Controller** - Initializes IRQ controller if enabled
8. **UART Configuration** - Sets up serial communication
9. **Hardware Timers** - Initializes system timers
10. **DMA Controller** - Sets up DMA
11. **RTC** - Initializes real-time clock
12. **LCD/Backlight** - Initializes display and backlight
13. **Keypad** - Sets up keypad driver
14. **ABB** - Initializes TWL3025 analog baseband
15. **Flash/TIFFS** - Detects flash and initializes filesystem
16. **RF** - TODO: Initialize LMX2326 when driver is ready

### Key Hardware Differences

#### 13MHz Clock
Unlike other Calypso boards that use 104MHz, the TSM30 uses 13MHz:
```c
calypso_clock_set(0, CALYPSO_PLL13_13_MHZ, ARM_MCLK_DIV_6);
```

#### Memory-Mapped LCD
The R61505 display uses memory mapping instead of I2C/UWire:
```c
#define LCD_CMD_ADDR  ((volatile uint16_t *)0x400000)
#define LCD_DATA_ADDR ((volatile uint16_t *)0x400001)
```

#### LMX2326 RF Transceiver
The TSM30 uses LMX2326 instead of TRF6151, requiring a different driver implementation.

## Driver Implementation Details

### LCD Driver (`fb_tsm30_r61505.c`)

The R61505 driver implements:
- Memory-mapped command/data interface
- RGB565 color format support
- 128x160 resolution
- Hardware acceleration for common operations

Key functions:
- `fb_tsm30_r61505_init()` - Display initialization
- `fb_tsm30_r61505_setup_dma()` - DMA setup for efficient transfers
- `fb_tsm30_r61505_set_pos()` - Position cursor

### LMX2326 RF Driver (`lmx2326.c`)

The LMX2326 driver provides:
- 8-bit and 24-bit register writes
- Frequency synthesis for GSM bands
- Gain control
- Power management

Key functions:
- `lmx2326_init()` - Chip initialization
- `lmx2326_set_arfcn()` - Set frequency
- `lmx2326_set_gain()` - Adjust gain

### RFFE Driver (`rffe_tsm30.c`)

The RF Frontend driver handles:
- Band switching (GSM900/DCS1800)
- Transmit/receive mode switching
- Antenna control
- Power amplifier control

## Configuration Options

### Build Configuration

Add to CFLAGS for specific features:
```makefile
# Enable transmitter support
CFLAGS += -DCONFIG_TX_ENABLE

# Enable flash write support
CFLAGS += -DCONFIG_FLASH_WRITE
```

### Runtime Configuration

Key parameters that may need adjustment:
- **Clock dividers** - In `calypso_clock_set()`
- **Memory wait states** - In `calypso_mem_cfg()`
- **TSP timing** - In `tsp_setup()`
- **Backlight level** - In `bl_level()`

## Extending Functionality

### Adding New Features

1. **Create driver files** in appropriate directories
2. **Update Makefile** with new object files
3. **Add initialization** to `board_init()` if needed
4. **Update documentation** with new features

### Example: Adding Battery Monitoring

1. Create `battery/tsm30.c`
2. Implement battery reading functions
3. Add to `BOARD_tsm30_OBJS` in Makefile
4. Call initialization from `board_init()`

### Example: Adding Audio Support

1. Create `audio/tsm30.c`
2. Implement audio codec interface
3. Add to appropriate OBJS list
4. Integrate with existing audio subsystem

## Testing and Validation

### Unit Testing

Test individual components:
```bash
# Test LCD driver
make BOARD=tsm30 APP=hello_world

# Test RF functionality
make BOARD=tsm30 APP=rssi

# Test keypad
make BOARD=tsm30 APP=menu
```

### Integration Testing

Verify all components work together:
1. Build complete firmware
2. Test on actual hardware
3. Verify serial communication
4. Check display output
5. Test RF functionality
6. Validate keypad input

### Performance Testing

Measure and optimize:
- **Clock accuracy** - Verify 13MHz timing
- **Memory performance** - Check access speeds
- **RF performance** - Measure signal quality
- **Power consumption** - Optimize for battery life

## Debugging and Profiling

### Debug Output

Add debug statements to track execution:
```c
#ifdef DEBUG
printf("TSM30: %s completed\n", __func__);
#endif
```

### Performance Profiling

Use system timers to measure execution time:
```c
uint32_t start = timer_get_counter();
// Code to measure
uint32_t end = timer_get_counter();
printf("Execution time: %lu ticks\n", end - start);
```

### Hardware Debugging

- **Logic analyzer** - For timing analysis
- **Oscilloscope** - For signal quality
- **Serial console** - For runtime debugging

## Contributing Guidelines

### Code Style

Follow existing OsmocomBB conventions:
- Linux kernel coding style
- C99 standard
- Descriptive variable names
- Comprehensive comments

### Documentation

- Update README.md with new features
- Add troubleshooting information
- Document hardware interfaces
- Provide usage examples

### Testing

- Test on actual hardware when possible
- Verify backward compatibility
- Document test procedures
- Report issues with reproduction steps

## Future Development

### Planned Enhancements

1. **Complete RF calibration** - Extract actual values from hardware
2. **Power management** - Implement sleep modes
3. **Audio support** - Add codec driver
4. **USB support** - Implement USB connectivity
5. **Bluetooth** - Add Bluetooth functionality

### Hardware Exploration

Areas for further hardware investigation:
- Camera interface
- External memory interfaces
- Additional GPIO functions
- Hardware acceleration features

## Resources

- [OsmocomBB Main Documentation](https://osmocom.org/projects/baseband)
- [TI Calypso Documentation](https://www.ti.com/product/CALYPSO)
- [LMX2326 Datasheet](https://www.ti.com/product/LMX2326)
- [R61505 Datasheet](https://www.renesas.com/us/en/document/dst/r61505-datasheet)