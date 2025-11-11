/* Driver for LMX2326 RF Transceiver (TSM30) */

#include <stdint.h>
#include <stdio.h>
#include <osmocom/gsm/gsm_utils.h>

#include <calypso/tpu.h>
#include <calypso/tsp.h>
#include <rf/lmx2326.h>
#include <rffe.h>

static uint8_t lmx2326_tsp_strobe;
static uint8_t lmx2326_tsp_reset;
static uint8_t lmx2326_gain = 40;

/* Write 8-bit register to LMX2326 */
static void lmx2326_write_8bit(uint8_t reg, uint8_t value)
{
    uint16_t data = ((reg & 0x07) << 8) | value;
    tsp_act_update(lmx2326_tsp_strobe, data);
}

/* Write 24-bit register to LMX2326 */
static void lmx2326_write_24bit(uint8_t reg, uint32_t value)
{
    /* LMX2326 uses 24 bits for some registers */
    uint32_t data = ((reg & 0x07) << 24) | (value & 0xFFFFFF);

    /* Send in 3 transfers of 8 bits */
    tsp_act_update(lmx2326_tsp_strobe, (data >> 16) & 0xFF);
    tsp_act_update(lmx2326_tsp_strobe, (data >> 8) & 0xFF);
    tsp_act_update(lmx2326_tsp_strobe, data & 0xFF);
}

void lmx2326_init(uint8_t tsp_strobe, uint8_t tsp_reset)
{
    lmx2326_tsp_strobe = tsp_strobe;
    lmx2326_tsp_reset = tsp_reset;

    /* Reset the chip */
    tsp_act_toggle(lmx2326_tsp_reset);

    /* Initial configuration from TSM30 source code */
    /* Power Control register */
    lmx2326_write_8bit(LMX2326_REG_POWER_CTRL, LMX2326_POWER_CTRL_CONST);

    /* Gain Control register */
    lmx2326_write_8bit(LMX2326_REG_GAIN_CTRL, LMX2326_GAIN_CTRL_RESET);

    /* Misc Control register (includes XtalTuning=0x10) */
    lmx2326_write_24bit(LMX2326_REG_MISC, LMX2326_MISC_CTL_RESET);

    /* Wide Band PLL register */
    lmx2326_write_24bit(LMX2326_REG_WB_PLL, LMX2326_WB_PLL_CONST | LMX2326_WB_PD_POS_EGSM);

    /* R-Counter register */
    lmx2326_write_24bit(LMX2326_REG_R_COUNTER, LMX2326_RCNT_PLL_CONST | LMX2326_RCNT_PLL_GSM_DIV);

    /* IF PLL for RX */
    lmx2326_write_24bit(LMX2326_REG_IF_PLL, LMX2326_IF_PLL_CONST | LMX2326_IF_PLL_RX);

    /* Test register */
    lmx2326_write_24bit(LMX2326_REG_TEST, LMX2326_TEST_INIT);
}

void lmx2326_power_on(void)
{
    lmx2326_write_8bit(LMX2326_REG_POWER_CTRL, 0xFF);
}

void lmx2326_power_off(void)
{
    lmx2326_write_8bit(LMX2326_REG_POWER_CTRL, 0x00);
}

void lmx2326_set_arfcn(uint16_t arfcn, int uplink)
{
    uint32_t ncnt;
    enum gsm_band band;
    int gsm_mode = 0;
    int rx_tx = uplink ? 1 : 0;

    band = gsm_arfcn2band(arfcn);

    /* Calculate PLL N-counter values based on TSM30 formulas */
    switch (band) {
        case GSM_BAND_900:
            gsm_mode = 1;
            if (uplink) {
                /* TX EGSM: Ncnt = (2082 * 5) + (Arfcn << 1) */
                ncnt = (2082 * 5) + (arfcn << 1);
            } else {
                /* RX EGSM: Ncnt = (2052 * 5) + ((Arfcn << 2) - Arfcn) */
                ncnt = (2052 * 5) + ((arfcn << 2) - arfcn);
            }
            break;

        case GSM_BAND_1800:
            if (uplink) {
                /* TX DCS: Ncnt = (2036 * 5) + (Arfcn - 174) */
                ncnt = (2036 * 5) + (arfcn - 174);
            } else {
                /* RX DCS: Ncnt = (2046 * 5) + (Arfcn - 174) */
                ncnt = (2046 * 5) + (arfcn - 174);
            }
            break;

        default:
            /* Default to EGSM RX calculation */
            ncnt = (2052 * 5) + ((arfcn << 2) - arfcn);
            break;
    }

    /* Configure Wide Band PLL with calculated N-counter */
    lmx2326_write_24bit(LMX2326_REG_WB_PLL, LMX2326_WB_PLL_CONST | ncnt);
}

int lmx2326_set_gain(uint8_t dbm)
{
    if (dbm > 127)
        return -1;

    lmx2326_gain = dbm;
    lmx2326_write_8bit(LMX2326_REG_GAIN_CTRL, dbm);
    return 0;
}

uint8_t lmx2326_get_gain(void)
{
    return lmx2326_gain;
}

int lmx2326_iq_swapped(uint16_t band_arfcn, int tx)
{
    /* TSM30 does not require IQ swap */
    return 0;
}