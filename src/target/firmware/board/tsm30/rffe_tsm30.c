/* RF Frontend driver for TSM30 with LMX2326 */

#include <stdint.h>
#include <stdio.h>

#include <rffe.h>
#include <calypso/tsp.h>
#include <rf/lmx2326.h>

/* TSM30 RF timings extracted from source code (quarter bits) */
#define TSM30_LDO_ON_TIME     0x1e0  /* 480 quarter bits */
#define TSM30_VCO_ON_TIME     0x1d0  /* 464 quarter bits */
#define TSM30_SYNTH_ON_TIME   0x1c2  /* 450 quarter bits */
#define TSM30_PA_ON_TIME_GSM  0x22   /* 34 quarter bits */
#define TSM30_PA_ON_TIME_DCS  0x2d   /* 45 quarter bits */
#define TSM30_PA_OFF_TIME     0x1e   /* 30 quarter bits */
#define TSM30_TX_PATH_ON_TIME 0xa0   /* 160 quarter bits */
#define TSM30_RX_PATH_ON_TIME 0xa0   /* 160 quarter bits */
#define TSM30_RF_ROUNDTRIP    0x45   /* 69 quarter bits */

/* TSP signal assignments from TSM30 source code */
#define TSM30_VCO_ON          TSPACT(1)  /* 0x0002 */
#define TSM30_PA_ON           TSPACT(2)  /* 0x0004 */
#define TSM30_SWITCH_CTL      TSPACT(3)  /* 0x0008 */
#define TSM30_RF_SWITCH       TSPACT(4)  /* 0x0010 */
#define TSM30_RAMP_DAC        TSPACT(8)  /* 0x0100 */
#define TSM30_NRF_SWITCH      TSPACT(9)  /* 0x0200 */
#define TSM30_PCS_ON          TSPACT(10) /* 0x0400 */

void rffe_mode(enum gsm_band band, int tx)
{
    uint16_t tspact = tsp_act_state();

    /* First mask off all relevant bits */
    tspact &= ~(TSM30_PA_ON | TSM30_RF_SWITCH | TSM30_PCS_ON);
    tspact |= TSM30_NRF_SWITCH;  /* Low-active */

#ifdef CONFIG_TX_ENABLE
    /* Set bits for transmit mode */
    if (tx) {
        tspact &= ~TSM30_NRF_SWITCH;  /* Enable antenna switch for TX */
        tspact |= TSM30_PA_ON;        /* Enable power amplifier */

        /* Set band-specific switches */
        switch (band) {
            case GSM_BAND_900:
                tspact |= TSM30_RF_SWITCH;  /* EGSM band */
                break;
            case GSM_BAND_1800:
            case GSM_BAND_1900:
                tspact |= TSM30_PCS_ON;     /* DCS/PCS band */
                break;
            default:
                break;
        }
    }
#endif /* CONFIG_TX_ENABLE */

    tsp_act_update(tspact);
}

uint32_t rffe_get_rx_ports(void)
{
    /* TSM30 supports EGSM900 and DCS1800 */
    return (1 << PORT_LO) | (1 << PORT_DCS1800);
}

uint32_t rffe_get_tx_ports(void)
{
    return (1 << PORT_LO) | (1 << PORT_HI);
}

int rffe_iq_swapped(uint16_t band_arfcn, int tx)
{
    return lmx2326_iq_swapped(band_arfcn, tx);
}

#define ARM_CONF_REG 0xfffef006

void rffe_init(void)
{
    uint16_t reg;

    /* Configure ARM_CONF for TSP */
    reg = readw(ARM_CONF_REG);
    reg |= 0x08;  /* ADD(22) mode */
    writew(reg, ARM_CONF_REG);

    /* Configure TSP for TWL3025 */
    tsp_setup(0, 1, 0, 0);  /* IOTA_STROBE */

    /* Initialize LMX2326 with correct TSP strobes */
    /* LMX2326 uses TSPACT8 for strobe and TSPACT9 for reset */
    lmx2326_init(8, 9);  /* strobe=TSPACT8, reset=TSPACT9 */
}

uint8_t rffe_get_gain(void)
{
    return lmx2326_get_gain();
}