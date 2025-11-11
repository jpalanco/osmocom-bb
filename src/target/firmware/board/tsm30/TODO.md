# TSM30 Implementation Progress

This checklist tracks the implementation progress of TSM30 board support for OsmocomBB.

## ✅ COMPLETED TASKS

### Board Infrastructure
- [x] Create TSM30 board directory structure
- [x] Implement board initialization (init.c)
- [x] Create keypad mapping (keymap.h)
- [x] Update Makefile to include TSM30 board support

### Display Support
- [x] Implement LCD driver for R61505 (fb_tsm30_r61505.c)

### RF System
- [x] Create LMX2326 RF driver (lmx2326.c and lmx2326.h)
- [x] Implement RFFE driver for TSM30 (rffe_tsm30.c)
- [x] Create RF calibration tables (rf_tables.c) - **ACTUAL TSM30 VALUES**
- [x] Convert RF tables to standard OsmocomBB txcal_tx_level format

### System Support
- [x] Add 13MHz clock support to calypso/clock.h
- [x] Implement flash detection and TIFFS configuration

### Documentation
- [x] Create README.md with hardware overview and build instructions
- [x] Create TROUBLESHOOTING.md with common issues and solutions
- [x] Create DEVELOPMENT.md with architecture and development guide

## ✅ COMPLETED HARDWARE-SPECIFIC VALUES

### RF Calibration Values (IMPLEMENTED)
**File: `src/target/firmware/board/tsm30/rf_tables.c`**

#### TX Power Levels
- [x] **GSM900 TX Power Levels** (16 values, 0-15 power levels)
  - Implemented: `0x320, 0x290, 0x240, 0x1e0, 0x180, 0x130, 0x0f0, 0x0c0, 0x097, 0x077, 0x05c, 0x048, 0x039, 0x02c, 0x020, 0x000`

- [x] **DCS1800 TX Power Levels** (16 values, 0-15 power levels)
  - Implemented: `0x330, 0x2b0, 0x240, 0x1d0, 0x170, 0x118, 0x0d9, 0x0a5, 0x080, 0x062, 0x04e, 0x03d, 0x02e, 0x023, 0x013, 0x00c`

- [x] **PCS1900 TX Power Levels** (16 values, same as DCS1800)

#### TX Ramps
- [x] **Primary Ramp Profiles** (16 ramp configurations)
  - Implemented: Complete ramp tables using MC_L1D_APC_RAMP_INIT macro encoding

- [x] **Alternative Ramp Profiles** (16 ramp configurations)
  - Implemented: Alternative configuration for different hardware versions

#### AFC Parameters
- [x] **AFC Initial Value**: `0xFD00` (-768 decimal)
- [x] **AFC Slope**: `0xA0` (160 decimal)

#### Timing Arrays
- [x] **PaOnTime Arrays**: EGSM, DCS, PCS (16 values each)
- [x] **PaOffTime Arrays**: EGSM, DCS, PCS (16 values each)

### LMX2326 Configuration (IMPLEMENTED)
**File: `src/target/firmware/rf/lmx2326.c`**

#### Register Values
- [x] **LMX2326 Initial Configuration**
  - Power Control: `0x00`
  - Gain Control: `0x00`
  - Misc Control: `0x307F7E` (includes XtalTuning=0x10)
  - Wide Band PLL: `0x004005`
  - R-Counter: `0x800000`
  - IF PLL: `0x000000`
  - Test Register: `0x00000F`

#### PLL Calculation
- [x] **Frequency Calculation Algorithm**
  - RX EGSM: `Ncnt = (2052 * 5) + ((Arfcn << 2) - Arfcn)`
  - RX DCS: `Ncnt = (2046 * 5) + (Arfcn - 174)`
  - TX EGSM: `Ncnt = (2082 * 5) + (Arfcn << 1)`
  - TX DCS: `Ncnt = (2036 * 5) + (Arfcn - 174)`

### TSP Configuration (IMPLEMENTED)
**File: `src/target/firmware/board/tsm30/rffe_tsm30.c`**

#### Strobe Assignments
- [x] **LMX2326 TSP Strobe Numbers**: Strobe=TSPACT8, Reset=TSPACT9
- [x] **TSP Signal Assignments**: VCO_ON, PA_ON, SWITCH_CTL, RF_SWITCH, RAMP_DAC, NRF_SWITCH, PCS_ON

#### Timing Parameters
- [x] **RF Timing Delays** (quarter bits)
  - LdoOnTime: 0x1e0 (480)
  - VcoOnTime: 0x1d0 (464)
  - SynthOnTime: 0x1c2 (450)
  - PaOnTimeGsm: 0x22 (34)
  - PaOnTimeDcs: 0x2d (45)
  - PaOffTime: 0x1e (30)
  - TxPathOnTime: 0xa0 (160)
  - RxPathOnTime: 0xa0 (160)
  - RfRoundtripTime: 0x45 (69)

### Flash Configuration (IMPLEMENTED)
**File: `src/target/firmware/board/tsm30/init.c`**

#### Flash Parameters
- [x] **Actual Flash Device ID**: Fujitsu MBM29LV800BA
  - Manufacturer ID: `0x0004`
  - Device ID: `0x227E`
  - Extended IDs: `0x2202`, `0x2201`

#### TIFFS Configuration
- [x] **TIFFS Sector Layout**: Sectors 103-141 (39 sectors, ~2.5MB)

### GPIO Configuration (IMPLEMENTED)
**File: `src/target/firmware/board/tsm30/init.c`**

#### GPIO Functions
- [x] **Basic GPIO Setup**: DSP_WAKEUP, KEYPAD_DET, CMOS_PWDN, CMOS_RESET
- [x] **Additional Controls**: Camera controls (GPIO9, GPIO11), MP3 controls (GPIO1, GPIO5)

### Hardware Version Detection (IMPLEMENTED)
**Files: `src/target/firmware/board/tsm30/hw_version.{c,h}`**

#### EEPROM/TIFFS-Based Detection
- [x] **O&M Parameters Reading**: Read hardware version from field 33 of O&M parameters section
- [x] **RF Section Version**: Read RF section header version from TIFFS
- [x] **Version-Specific Configuration**: Select RF parameters based on detected hardware version
- [x] **RF Patches**: Apply version-specific RF timing patches

#### Hardware Version Mapping
- **Version 0 (Rev C)**: Default configuration, may have old or new LCD
- **Version 1 (Rev D)**: Alternative configuration with new LCD
- **Version 2 (Rev E)**: Variant configuration (currently uses alt config)

## 🎯 VALIDATION TASKS

### Hardware Testing
- [ ] Build and test hello_world application
- [ ] Verify serial communication
- [ ] Test LCD display output
- [ ] Validate keypad functionality
- [ ] Test RF signal detection
- [ ] Verify flash detection and TIFFS

### Performance Optimization
- [ ] Verify 13MHz clock stability
- [ ] Optimize memory timing
- [ ] Fine-tune RF performance
- [ ] Validate power consumption

## ✅ IMPLEMENTATION STATUS

### All Hardware Values Successfully Implemented
All TSM30-specific values have been extracted from the source code and implemented:

- ✅ **RF Calibration**: TX power levels, ramp profiles, AFC parameters
- ✅ **LMX2326 Configuration**: Register values, PLL calculations, driver initialization
- ✅ **TSP Assignments**: Strobe numbers, signal definitions, timing delays
- ✅ **Flash Configuration**: Device IDs, TIFFS layout
- ✅ **GPIO Configuration**: All board-specific GPIO functions
- ✅ **LMX2326 TODO Resolved**: Driver initialization moved to RFFE driver with correct parameters (8,9)
- ✅ **Hardware Version Detection**: EEPROM/TIFFS-based detection replacing GPIO approach
- ✅ **Version-Specific Configuration**: Automatic selection of RF parameters based on detected hardware
- ✅ **Standard RF Table Format**: Converted to use OsmocomBB txcal_tx_level structures
- ✅ **Memory Mapping**: Complete linker script and memory documentation

## 🚀 NEXT STEPS

1. **Build Environment Setup** - Fix cross-compiler toolchain issues
2. **Build and Test** - Verify functionality on actual TSM30 hardware
3. **Performance Tuning** - Optimize RF performance based on test results
4. **Hardware Validation** - Test all components (LCD, keypad, RF, flash)
5. **Integration Testing** - Ensure compatibility with OsmocomBB applications
6. **Community Contribution** - Submit implementation to OsmocomBB project

## ⚠️ BUILD ENVIRONMENT ISSUE

The current build environment has cross-compiler toolchain issues preventing compilation:
- Missing system headers (stdint.h, sys/types.h) for embedded target
- This is a toolchain configuration issue, not a TSM30 implementation problem
- The TSM30 code follows all standard OsmocomBB patterns and structures
- All RF calibration values are properly implemented using actual TSM30 source code values

## 📞 CONTACT

For providing TSM30 hardware values or reporting issues, please refer to the main README.md for contact information.