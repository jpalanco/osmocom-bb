# TSM30 Troubleshooting Guide

This guide helps resolve common issues when working with the Vitelcom TSM30 board support.

## Common Issues

### 1. Build Failures

**Issue**: Build fails with "undefined reference" errors
**Solution**: Ensure you're building from the correct directory:
```bash
cd src/target/firmware
make BOARD=tsm30 APP=hello_world
```

**Issue**: Missing LMX2326 driver
**Solution**: The LMX2326 driver is included in the RF_OBJS list. Make sure `rf/lmx2326.c` compiles successfully.

### 2. Serial Communication Issues

**Issue**: No output on serial console
**Solution**:
- Verify the TSM30 uses UART_MODEM for sercomm (configured in `board_init()`)
- Check baud rate is 115200
- Ensure proper connection to the phone's modem port

**Issue**: Garbled serial output
**Solution**: The TSM30 uses 13MHz clock instead of 104MHz, which affects UART timing. Verify the clock configuration in `calypso_clock_set()`.

### 3. Display Issues

**Issue**: No display output
**Solution**:
- Verify the R61505 driver is compiled (`fb_tsm30_r61505.o`)
- Check memory mapping addresses (0x400000 for commands, 0x400001 for data)
- Ensure the display is properly initialized in `fb_init()`

**Issue**: Incorrect colors or artifacts
**Solution**: The R61505 uses RGB565 format. Verify the color conversion in the framebuffer driver.

### 4. RF Issues

**Issue**: No GSM signal detection
**Solution**:
- Check LMX2326 initialization in `rffe_init()`
- Verify TSP strobe assignments (currently set to 1,2)
- Ensure proper power sequencing

**Issue**: Poor signal quality
**Solution**: The RF calibration tables contain placeholder values. These need to be extracted from actual TSM30 hardware for proper operation.

### 5. Keypad Issues

**Issue**: Keys not responding
**Solution**:
- Verify the 5x5 keypad matrix mapping in `keymap.h`
- Check GPIO 6 (KEYPAD_DET) is configured as input
- Ensure keypad interrupt is enabled if using IRQ mode

### 6. Flash Issues

**Issue**: TIFFS not initializing
**Solution**:
- Verify flash detection in `board_flash_init()`
- Check the flash ID matches supported devices
- Ensure proper memory configuration for CS2

**Issue**: Flash write failures
**Solution**: Flash write support is disabled by default. Enable with `-DCONFIG_FLASH_WRITE` in CFLAGS.

## Debugging Techniques

### 1. Enable Debug Output
Add debug prints to critical initialization functions:
```c
printf("TSM30: Initializing %s\n", component);
```

### 2. Check Clock Configuration
Use `calypso_clk_dump()` to verify the 13MHz clock is properly configured.

### 3. Verify GPIO States
Check GPIO configuration in `board_io_init()` and ensure all pins are in correct states.

### 4. Test Individual Components
Build and test individual applications:
- `hello_world` - Basic functionality
- `rssi` - RF signal measurement
- `menu` - Interactive interface

## Hardware Verification

### Required Equipment
- TSM30 phone with test points accessible
- Serial cable for UART communication
- Power supply (3.6-4.2V)
- Logic analyzer (optional, for debugging)

### Test Points
- **UART**: Modem port pins
- **Power**: Battery connector
- **Display**: LCD connector
- **RF**: Antenna connector

## Performance Optimization

### Clock Configuration
The 13MHz clock may need adjustment for optimal performance. The current configuration:
- PLL multiplier: 6
- PLL divider: 0
- ARM divider: 6

### Memory Timing
Adjust memory wait states in `calypso_mem_cfg()` if experiencing memory access issues.

## Known Limitations

1. **RF Calibration**: Placeholder values in RF tables
2. **TSP Timing**: May need adjustment for LMX2326
3. **Power Management**: Basic implementation, may need optimization
4. **Display Brightness**: Fixed at 50%, may need PWM control

## Getting Help

- Check the OsmocomBB mailing list: baseband-devel@lists.osmocom.org
- Review existing board implementations for reference
- Consult the main OsmocomBB documentation
- Test with known working hardware if available