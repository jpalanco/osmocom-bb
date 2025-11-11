/* TSM30 R61505 LCD Driver - Memory Mapped Interface */

#include <stdint.h>
#include <fb/framebuffer.h>
#include <fb/fb_rgb565.h>
#include <delay.h>

/* Memory-mapped LCD addresses for TSM30 */
#define LCD_CMD_ADDR  ((volatile uint16_t *)0x400000)
#define LCD_DATA_ADDR ((volatile uint16_t *)0x400001)

/* LCD write macros */
#define lcd_write_cmd(cmd)   (*LCD_CMD_ADDR = (cmd))
#define lcd_write_data(data) (*LCD_DATA_ADDR = (data))

/* R61505 display specifications */
#define LCD_WIDTH  128
#define LCD_HEIGHT 160
#define LCD_GRAM_WIDTH 132

/* RGB565 color palette */
static const uint16_t palette_values[16] = {
    0x0000, 0x0101, 0x0202, 0x0303,
    0x0404, 0x0505, 0x0606, 0x0707,
    0x0808, 0x0909, 0x0A0A, 0x0B0B,
    0x0C0C, 0x0D0D, 0x0E0E, 0x0F0F
};

static uint8_t fb_tsm30_mem[LCD_WIDTH * LCD_HEIGHT * 2]; /* RGB565: 2 bytes per pixel */

static void fb_tsm30_init_display(void)
{
    /* R61505 initialization sequence */

    /* Display Control */
    lcd_write_cmd(0x07);
    lcd_write_data(0x0000);

    /* Contrast Control */
    lcd_write_cmd(0x04);
    lcd_write_data(0x0148);

    /* Display Control 2 */
    lcd_write_cmd(0x0C);
    lcd_write_data(0x0000);
    delay_ms(100);

    /* Power Control 1 - Phase 1 */
    lcd_write_cmd(0x03);
    lcd_write_data(0x8808);
    delay_ms(250);

    /* Power Control 1 - Phase 2 */
    lcd_write_cmd(0x03);
    lcd_write_data(0x8C08);
    delay_ms(250);

    /* Driver Output Control */
    lcd_write_cmd(0x01);
    lcd_write_data(0x0113);

    /* Entry Mode */
    lcd_write_cmd(0x05);
    lcd_write_data(0x0018);

    /* Display Control 3 */
    lcd_write_cmd(0x06);
    lcd_write_data(0x0000);

    /* Display Control 4 */
    lcd_write_cmd(0x0B);
    lcd_write_data(0x0000);

    /* External Display Interface Control */
    lcd_write_cmd(0x11);
    lcd_write_data(0x0000);

    /* Horizontal Window Address */
    lcd_write_cmd(0x14);
    lcd_write_data(0x9F00);

    lcd_write_cmd(0x16);
    lcd_write_data(0x7F00);

    /* Vertical Window Address */
    lcd_write_cmd(0x17);
    lcd_write_data(0x9F00);

    /* GRAM Address Set */
    lcd_write_cmd(0x20);
    lcd_write_data(0x0000);

    lcd_write_cmd(0x21);
    lcd_write_data(0x0000);

    /* Configure color palette */
    for (int i = 0; i < 16; i++) {
        lcd_write_cmd(0x30 + i);
        lcd_write_data(palette_values[i]);
    }

    /* Display Control - Turn on display */
    lcd_write_cmd(0x07);
    lcd_write_data(0x0003);
}

static void fb_tsm30_set_window(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    /* Configure write window */
    lcd_write_cmd(0x20);  /* GRAM X address */
    lcd_write_data(x);

    lcd_write_cmd(0x21);  /* GRAM Y address */
    lcd_write_data(y);

    lcd_write_cmd(0x14);  /* Horizontal window start */
    lcd_write_data((x + width - 1) << 8 | x);

    lcd_write_cmd(0x17);  /* Vertical window */
    lcd_write_data((y + height - 1) << 8);
}

static void fb_tsm30_init(void)
{
    printf("Initializing TSM30 R61505 LCD...\n");
    fb_tsm30_init_display();
}

static void fb_tsm30_flush(void)
{
    /* For memory-mapped display, we can write directly */
    /* This implementation uses the RGB565 backend for rendering */

    /* Set window to entire display */
    fb_tsm30_set_window(0, 0, LCD_WIDTH, LCD_HEIGHT);

    /* Prepare for GRAM write */
    lcd_write_cmd(0x22);

    /* Write RGB565 data from framebuffer */
    uint16_t *fb_data = (uint16_t *)fb_rgb565->mem;
    for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++) {
        lcd_write_data(fb_data[i]);
    }
}

static struct framebuffer fb_tsm30_framebuffer = {
    .name = "tsm30_r61505",
    .init = fb_tsm30_init,
    .clear = fb_rgb565_clear,
    .boxto = fb_rgb565_boxto,
    .lineto = fb_rgb565_lineto,
    .putstr = fb_rgb565_putstr,
    .flush = fb_tsm30_flush,
    .width = LCD_WIDTH,
    .height = LCD_HEIGHT
};

static struct fb_rgb565 fb_tsm30_rgb565 = {
    .mem = fb_tsm30_mem
};

struct framebuffer *framebuffer = &fb_tsm30_framebuffer;
struct fb_rgb565 *fb_rgb565 = &fb_tsm30_rgb565;