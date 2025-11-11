/* RF calibration tables for TSM30 */

#include <stdint.h>
#include <rf/txcal.h>
#include <rf/lmx2326.h>
#include "hw_version.h"

/* TX power levels - actual values from TSM30 source code */
/* 16 power levels (0-15) for each band */
static const int16_t tsm30_tx_levels_900[] = {
    /* EGSM band APC DAC values */
    0x320, 0x290, 0x240, 0x1e0, 0x180, 0x130, 0x0f0, 0x0c0,
    0x097, 0x077, 0x05c, 0x048, 0x039, 0x02c, 0x020, 0x000
};

static const int16_t tsm30_tx_levels_1800[] = {
    /* DCS1800 band APC DAC values */
    0x330, 0x2b0, 0x240, 0x1d0, 0x170, 0x118, 0x0d9, 0x0a5,
    0x080, 0x062, 0x04e, 0x03d, 0x02e, 0x023, 0x013, 0x00c
};

static const int16_t tsm30_tx_levels_1900[] = {
    /* PCS1900 band APC DAC values (same as DCS1800) */
    0x330, 0x2b0, 0x240, 0x1d0, 0x170, 0x118, 0x0d9, 0x0a5,
    0x080, 0x062, 0x04e, 0x03d, 0x02e, 0x023, 0x013, 0x00c
};

/* Standard OsmocomBB RF calibration structures */
struct txcal_tx_level rf_tx_levels_850[RF_TX_LEVELS_TABLE_SIZE] = {
    /* TSM30 doesn't support 850MHz band, fill with zeros */
    { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
    { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
    { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
    { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
    { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
    { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
    { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
    { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }
};

struct txcal_tx_level rf_tx_levels_900[RF_TX_LEVELS_TABLE_SIZE] = {
    /* EGSM900 band - first 16 levels from TSM30 source, rest filled with last value */
    { 0x320, 0, 0 }, { 0x290, 0, 0 }, { 0x240, 0, 0 }, { 0x1e0, 0, 0 },
    { 0x180, 0, 0 }, { 0x130, 0, 0 }, { 0x0f0, 0, 0 }, { 0x0c0, 0, 0 },
    { 0x097, 0, 0 }, { 0x077, 0, 0 }, { 0x05c, 0, 0 }, { 0x048, 0, 0 },
    { 0x039, 0, 0 }, { 0x02c, 0, 0 }, { 0x020, 0, 0 }, { 0x000, 0, 0 },
    /* Fill remaining entries with last value */
    { 0x000, 0, 0 }, { 0x000, 0, 0 }, { 0x000, 0, 0 }, { 0x000, 0, 0 },
    { 0x000, 0, 0 }, { 0x000, 0, 0 }, { 0x000, 0, 0 }, { 0x000, 0, 0 },
    { 0x000, 0, 0 }, { 0x000, 0, 0 }, { 0x000, 0, 0 }, { 0x000, 0, 0 },
    { 0x000, 0, 0 }, { 0x000, 0, 0 }, { 0x000, 0, 0 }, { 0x000, 0, 0 }
};

struct txcal_tx_level rf_tx_levels_1800[RF_TX_LEVELS_TABLE_SIZE] = {
    /* DCS1800 band - first 16 levels from TSM30 source, rest filled with last value */
    { 0x330, 0, 0 }, { 0x2b0, 0, 0 }, { 0x240, 0, 0 }, { 0x1d0, 0, 0 },
    { 0x170, 0, 0 }, { 0x118, 0, 0 }, { 0x0d9, 0, 0 }, { 0x0a5, 0, 0 },
    { 0x080, 0, 0 }, { 0x062, 0, 0 }, { 0x04e, 0, 0 }, { 0x03d, 0, 0 },
    { 0x02e, 0, 0 }, { 0x023, 0, 0 }, { 0x013, 0, 0 }, { 0x00c, 0, 0 },
    /* Fill remaining entries with last value */
    { 0x00c, 0, 0 }, { 0x00c, 0, 0 }, { 0x00c, 0, 0 }, { 0x00c, 0, 0 },
    { 0x00c, 0, 0 }, { 0x00c, 0, 0 }, { 0x00c, 0, 0 }, { 0x00c, 0, 0 },
    { 0x00c, 0, 0 }, { 0x00c, 0, 0 }, { 0x00c, 0, 0 }, { 0x00c, 0, 0 },
    { 0x00c, 0, 0 }, { 0x00c, 0, 0 }, { 0x00c, 0, 0 }, { 0x00c, 0, 0 }
};

struct txcal_tx_level rf_tx_levels_1900[RF_TX_LEVELS_TABLE_SIZE] = {
    /* PCS1900 band - same as DCS1800 */
    { 0x330, 0, 0 }, { 0x2b0, 0, 0 }, { 0x240, 0, 0 }, { 0x1d0, 0, 0 },
    { 0x170, 0, 0 }, { 0x118, 0, 0 }, { 0x0d9, 0, 0 }, { 0x0a5, 0, 0 },
    { 0x080, 0, 0 }, { 0x062, 0, 0 }, { 0x04e, 0, 0 }, { 0x03d, 0, 0 },
    { 0x02e, 0, 0 }, { 0x023, 0, 0 }, { 0x013, 0, 0 }, { 0x00c, 0, 0 },
    /* Fill remaining entries with last value */
    { 0x00c, 0, 0 }, { 0x00c, 0, 0 }, { 0x00c, 0, 0 }, { 0x00c, 0, 0 },
    { 0x00c, 0, 0 }, { 0x00c, 0, 0 }, { 0x00c, 0, 0 }, { 0x00c, 0, 0 },
    { 0x00c, 0, 0 }, { 0x00c, 0, 0 }, { 0x00c, 0, 0 }, { 0x00c, 0, 0 },
    { 0x00c, 0, 0 }, { 0x00c, 0, 0 }, { 0x00c, 0, 0 }, { 0x00c, 0, 0 }
};

/* TX ramp profiles from TSM30 source code */
/* Primary ramp configuration (16 ramp profiles) */
const uint32_t tsm30_ramp_profiles[] = {
    /* MC_L1D_APC_RAMP_INIT(up,down) macro encoding: (((down) << 5) | (up)) << 6 */
    0x1b00,  /* (0,11) */
    0x2800,  /* (0,20) */
    0x1e80,  /* (3,12) */
    0x1e00,  /* (0,15) */
    0x2280,  /* (5,17) */
    0x1a00,  /* (4,10) */
    0x3680,  /* (17,13) */
    0x3000,  /* (19,16) */
    0x3480,  /* (22,18) */
    0x3680,  /* (31,17) */
    0x3680,  /* (24,17) */
    0x3680,  /* (31,17) */
    0x3200,  /* (26,16) */
    0x2e00,  /* (22,14) */
    0x3480,  /* (27,18) */
    0x3a80   /* (25,25) */
};

/* Alternative ramp configuration for different hardware */
const uint32_t tsm30_ramp_profiles_alt[] = {
    0x0a80,  /* (25,5) */
    0x1e00,  /* (0,15) */
    0x2a80,  /* (0,21) */
    0x2e00,  /* (0,23) */
    0x2a80,  /* (0,21) */
    0x1c00,  /* (0,14) */
    0x0a00,  /* (0,5) */
    0x0000,  /* (0,0) */
    0x0000,  /* (0,0) */
    0x0000,  /* (0,0) */
    0x0000,  /* (0,0) */
    0x0080,  /* (10,0) */
    0x3400,  /* (26,0) */
    0x3e00,  /* (31,0) */
    0x3400,  /* (26,0) */
    0x1a80   /* (10,24) */
};

/* Timing arrays for power amplifier control */
/* PaOnTime arrays (16 values each, in quarter bits) */
const uint8_t tsm30_pa_on_time_900[] = {
    21, 22, 21, 21, 21, 23, 23, 23,
    23, 25, 25, 25, 25, 25, 25, 21
};

const uint8_t tsm30_pa_on_time_1800[] = {
    21, 22, 21, 21, 21, 23, 23, 23,
    23, 25, 25, 25, 25, 25, 25, 25
};

const uint8_t tsm30_pa_on_time_1900[] = {
    10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10
};

/* PaOffTime arrays (16 values each, in quarter bits) */
const uint8_t tsm30_pa_off_time_900[] = {
    30, 30, 30, 30, 30, 30, 30, 30,
    30, 25, 25, 25, 25, 25, 25, 25
};

const uint8_t tsm30_pa_off_time_1800[] = {
    30, 30, 30, 30, 30, 30, 30, 30,
    30, 25, 25, 25, 25, 25, 25, 25
};

const uint8_t tsm30_pa_off_time_1900[] = {
    30, 30, 30, 30, 30, 30, 30, 30,
    30, 25, 25, 25, 25, 25, 25, 25
};

/* AFC parameters from TSM30 source code */
const int16_t tsm30_afc_initial = 0xFD00;  /* -768 decimal */
const int16_t tsm30_afc_slope = 0xA0;      /* 160 decimal */

/* Get ramp profiles based on detected hardware version from EEPROM */
const uint32_t *tsm30_get_ramp_profiles(void)
{
    int hw_version = tsm30_detect_hardware_version();

    switch (hw_version) {
        case TSM30_CONFIG_ALT:
            return tsm30_ramp_profiles_alt;
        case TSM30_CONFIG_VARIANT:
            /* Future: return tsm30_ramp_profiles_variant3; */
            return tsm30_ramp_profiles_alt;
        default:
            return tsm30_ramp_profiles;
    }
}

/* Get timing arrays based on hardware version */
const uint8_t *tsm30_get_pa_on_time(int band)
{
    int hw_version = tsm30_detect_hardware_version();

    switch (band) {
        case 0: /* EGSM900 */
            return tsm30_pa_on_time_900;
        case 1: /* DCS1800 */
            return tsm30_pa_on_time_1800;
        case 2: /* PCS1900 */
            return tsm30_pa_on_time_1900;
        default:
            return tsm30_pa_on_time_900;
    }
}

const uint8_t *tsm30_get_pa_off_time(int band)
{
    int hw_version = tsm30_detect_hardware_version();

    switch (band) {
        case 0: /* EGSM900 */
            return tsm30_pa_off_time_900;
        case 1: /* DCS1800 */
            return tsm30_pa_off_time_1800;
        case 2: /* PCS1900 */
            return tsm30_pa_off_time_1900;
        default:
            return tsm30_pa_off_time_900;
    }
}

/* Standard OsmocomBB RF table access functions */
/* These functions are defined in txcal.h as extern, so we need to implement them */
struct txcal_tx_level *rf_get_tx_levels(int band)
{
    switch (band) {
        case 0: /* EGSM900 */
            return rf_tx_levels_900;
        case 1: /* DCS1800 */
            return rf_tx_levels_1800;
        case 2: /* PCS1900 */
            return rf_tx_levels_1900;
        default:
            return rf_tx_levels_900;
    }
}

/* TSM30 ramp profiles - using standard txcal_ramp_def structure */
struct txcal_ramp_def rf_tx_ramps_850[RF_TX_RAMP_SIZE];
struct txcal_ramp_def rf_tx_ramps_900[RF_TX_RAMP_SIZE];
struct txcal_ramp_def rf_tx_ramps_1800[RF_TX_RAMP_SIZE];
struct txcal_ramp_def rf_tx_ramps_1900[RF_TX_RAMP_SIZE];

/* TSM30 channel calibration - using standard txcal_chan_cal structure */
struct txcal_chan_cal rf_tx_chan_cal_850[RF_TX_CHAN_CAL_TABLE_SIZE][RF_TX_NUM_SUB_BANDS];
struct txcal_chan_cal rf_tx_chan_cal_900[RF_TX_CHAN_CAL_TABLE_SIZE][RF_TX_NUM_SUB_BANDS];
struct txcal_chan_cal rf_tx_chan_cal_1800[RF_TX_CHAN_CAL_TABLE_SIZE][RF_TX_NUM_SUB_BANDS];
struct txcal_chan_cal rf_tx_chan_cal_1900[RF_TX_CHAN_CAL_TABLE_SIZE][RF_TX_NUM_SUB_BANDS];