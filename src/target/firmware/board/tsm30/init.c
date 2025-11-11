/* Initialization for Vitelcom TSM30 */

#include <stdint.h>
#include <stdio.h>

#include <debug.h>
#include <ctors.h>
#include <memory.h>
#include <board.h>
#include <keypad.h>
#include <console.h>
#include <flash/cfi_flash.h>
#include <tiffs.h>

#include <calypso/irq.h>
#include <calypso/clock.h>
#include <calypso/dma.h>
#include <calypso/rtc.h>
#include <calypso/timer.h>
#include <uart.h>
#include <calypso/backlight.h>

#include <comm/sercomm.h>
#include <comm/timer.h>

#include <abb/twl3025.h>
#include <rf/lmx2326.h>
#include <fb/framebuffer.h>
#include <battery/dummy.h>
#include "keymap.h"
#include "hw_version.h"

/* Calypso GPIO registers */
#define ARMIO_LATCH_OUT 0xfffe4802
#define IO_CNTL_REG     0xfffe4804
#define ASIC_CONF_REG   0xfffef008
#define IO_CONF_REG     0xfffef00a
#define ARM_CONF_REG    0xfffef006

/* TSM30 Watchdog */
#define WATCHDOG_REG    0xFFFFF804
#define WDOG_DISABLE_1  0xF5
#define WDOG_DISABLE_2  0xA0

static void board_io_init(void)
{
    uint16_t reg;

    /* Configure ASIC_CONF_REG for TWL3025 SPI rising edge */
    reg = readw(ASIC_CONF_REG);
    reg |= 0x6000;  /* Bits 13,14 for SPI rising edge */
    writew(reg, ASIC_CONF_REG);

    /* Configure ARM_CONF_REG for ADD(22) */
    reg = readw(ARM_CONF_REG);
    reg |= 0x08;  /* Bit 3 for ADD(22) instead of CS4 */
    writew(reg, ARM_CONF_REG);

    /* GPIO 0 (DSP_WAKEUP): output, high */
    reg = readw(IO_CNTL_REG);
    reg &= ~(1 << 0);  /* Set as output */
    writew(reg, IO_CNTL_REG);

    reg = readw(ARMIO_LATCH_OUT);
    reg |= (1 << 0);   /* Set high */
    writew(reg, ARMIO_LATCH_OUT);

    /* GPIO 6 (KEYPAD_DET): input */
    reg = readw(IO_CONF_REG);
    reg |= (1 << 2);   /* Set to GPIO mode */
    writew(reg, IO_CONF_REG);

    reg = readw(IO_CNTL_REG);
    reg |= (1 << 6);   /* Set as input */
    writew(reg, IO_CNTL_REG);

    /* GPIO 9 (CMOS_PWDN): output, high */
    reg = readw(IO_CONF_REG);
    reg |= (1 << 5);
    writew(reg, IO_CONF_REG);

    reg = readw(IO_CNTL_REG);
    reg &= ~(1 << 9);
    writew(reg, IO_CNTL_REG);

    reg = readw(ARMIO_LATCH_OUT);
    reg |= (1 << 9);
    writew(reg, ARMIO_LATCH_OUT);

    /* GPIO 11 (CMOS_RESET): output, high */
    reg = readw(IO_CONF_REG);
    reg |= (1 << 7);
    writew(reg, IO_CONF_REG);

    reg = readw(IO_CNTL_REG);
    reg &= ~(1 << 11);
    writew(reg, IO_CNTL_REG);

    reg = readw(ARMIO_LATCH_OUT);
    reg |= (1 << 11);
    writew(reg, ARMIO_LATCH_OUT);
}

static void board_flash_init(void)
{
    uint16_t manufacturer_id, device_id[3];

    /* Read flash ID from CS2 (External FLASH) */
    flash_get_id((void *)0x02000000, &manufacturer_id, device_id);

    printf("TSM30 Flash ID: mfr=0x%04x dev=0x%04x 0x%04x 0x%04x\n",
           manufacturer_id, device_id[0], device_id[1], device_id[2]);

    /* Configure TIFFS according to actual TSM30 flash */
    /* TSM30 uses Fujitsu MBM29LV800BA: 8MB, 142 sectors of 64KB */
    if (manufacturer_id == 0x0004 && device_id[0] == 0x227E) {
        /* Fujitsu MBM29LV800BA: 8MB, 142 sectors */
        /* TIFFS uses sectors 103-141 (39 sectors, ~2.5MB) */
        tiffs_init(0x02000000, 0x10000, 39);  /* Last 39 sectors */
        printf("TSM30: Fujitsu MBM29LV800BA detected, TIFFS initialized\n");
    } else if (manufacturer_id == CFI_MANUF_SPANSION) {
        /* Fallback: Spansion S71PL064J: 8MB, 128 sectors */
        tiffs_init(0x02000000, 0x10000, 15);  /* Last 15 sectors */
        printf("TSM30: Spansion flash detected, TIFFS initialized\n");
    } else {
        printf("TSM30: Unknown flash (mfr=0x%04x), TIFFS not initialized\n",
               manufacturer_id);
    }
}

void board_init(int with_irq)
{
    /* Disable TSM30 watchdog */
    writew(WDOG_DISABLE_1, WATCHDOG_REG);
    writew(WDOG_DISABLE_2, WATCHDOG_REG);

    /* Configure memory interface for TSM30 */
    calypso_mem_cfg(CALYPSO_nCS0, 1, CALYPSO_MEM_16bit, 1);  /* Internal FLASH */
    calypso_mem_cfg(CALYPSO_nCS1, 0, CALYPSO_MEM_16bit, 1);  /* External SRAM */
    calypso_mem_cfg(CALYPSO_nCS2, 1, CALYPSO_MEM_16bit, 1);  /* External FLASH */
    calypso_mem_cfg(CALYPSO_nCS3, 1, CALYPSO_MEM_8bit, 1);   /* LCD */
    calypso_mem_cfg(CALYPSO_nCS6, 0, CALYPSO_MEM_32bit, 1);  /* Internal SRAM */
    calypso_mem_cfg(CALYPSO_nCS7, 0, CALYPSO_MEM_32bit, 0);

    /* TSM30 uses 13MHz ARM clock, not 104MHz like other boards */
    calypso_clock_set(0, CALYPSO_PLL13_13_MHZ, ARM_MCLK_DIV_6);

    /* Configure RHEA bridge */
    calypso_rhea_cfg(2, 2, 0x7F, 0, 1, 0, 0);

    /* Initialize board-specific GPIO */
    board_io_init();

    /* Enable bootrom mapping for exception vectors */
    calypso_bootrom(with_irq);
    calypso_exceptions_install();

    /* Initialize interrupt controller */
    if (with_irq)
        irq_init();

    /* Configure UARTs - TSM30 uses UART_MODEM for sercomm */
    sercomm_bind_uart(UART_MODEM);
    cons_bind_uart(UART_IRDA);

    uart_init(UART_MODEM, with_irq);
    uart_baudrate(UART_MODEM, UART_115200);

    uart_init(UART_IRDA, with_irq);
    uart_baudrate(UART_IRDA, UART_115200);

    /* Initialize hardware timers */
    hwtimer_init();

    /* Initialize DMA controller */
    dma_init();

    /* Initialize real time clock */
    rtc_init();

    /* Initialize system timers */
    timer_init();

    /* Initialize LCD driver and backlight */
    fb_init();
    bl_mode_pwl(1);
    bl_level(50);

    /* Initialize keypad driver */
    keypad_init(keymap, with_irq);

    /* Initialize ABB driver (TWL3025) */
    twl3025_init();

    /* Initialize board flash and TIFFS */
    board_flash_init();

    /* Initialize hardware version detection from EEPROM */
    tsm30_hw_version_init();

    /* LMX2326 RF driver initialized in rffe_tsm30.c */
}