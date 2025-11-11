#ifndef _LMX2326_H
#define _LMX2326_H

#include <stdint.h>

/* LMX2326 registers */
#define LMX2326_REG_POWER_CTRL   0
#define LMX2326_REG_GAIN_CTRL    1
#define LMX2326_REG_MISC         2
#define LMX2326_REG_WB_PLL       3
#define LMX2326_REG_R_COUNTER    4
#define LMX2326_REG_IF_PLL       5
#define LMX2326_REG_TEST         6

/* LMX2326 register constants from TSM30 source code */
#define LMX2326_POWER_CTRL_CONST     0x00
#define LMX2326_GAIN_CTRL_RESET      0x00
#define LMX2326_MISC_CTL_RESET       0x307F7E
#define LMX2326_WB_PLL_CONST         0x004005
#define LMX2326_RCNT_PLL_CONST       0x800000
#define LMX2326_IF_PLL_CONST         0x000000
#define LMX2326_IF_PLL_RX            0x000000  /* RX mode */
#define LMX2326_IF_PLL_TX            0x000000  /* TX mode */
#define LMX2326_TEST_INIT            0x00000F

/* PLL configuration constants */
#define LMX2326_WB_PD_POS_EGSM       0x000000
#define LMX2326_WB_PD_POS_DCS        0x000000
#define LMX2326_RCNT_PLL_GSM_DIV     0x000000

/* Supported bands */
enum lmx2326_band {
    LMX2326_BAND_EGSM900,
    LMX2326_BAND_DCS1800,
    LMX2326_BAND_PCS1900
};

/* Public functions */
void lmx2326_init(uint8_t tsp_strobe, uint8_t tsp_reset);
void lmx2326_power_on(void);
void lmx2326_power_off(void);
void lmx2326_set_arfcn(uint16_t arfcn, int uplink);
int lmx2326_set_gain(uint8_t dbm);
uint8_t lmx2326_get_gain(void);
int lmx2326_iq_swapped(uint16_t band_arfcn, int tx);

#endif /* _LMX2326_H */