/* TSM30 Keypad Mapping - 5x5 matrix */

#ifndef _TSM30_KEYMAP_H
#define _TSM30_KEYMAP_H

#include <keypad.h>

/* TSM30 keypad mapping (5x5 matrix)
 * TODO: Verify actual key positions from TSM30 hardware */
static const uint8_t keymap[] = {
	[KEY_0]		= 16,
	[KEY_1]		= 0,
	[KEY_2]		= 1,
	[KEY_3]		= 2,
	[KEY_4]		= 5,
	[KEY_5]		= 6,
	[KEY_6]		= 7,
	[KEY_7]		= 10,
	[KEY_8]		= 11,
	[KEY_9]		= 12,
	[KEY_STAR]	= 15,
	[KEY_HASH]	= 17,
	[KEY_MENU]	= 4,
	[KEY_LEFT_SB]	= 9,
	[KEY_RIGHT_SB]	= 14,
	[KEY_UP]	= 3,
	[KEY_DOWN]	= 8,
	[KEY_LEFT]	= 18,
	[KEY_RIGHT]	= 19,
	[KEY_OK]	= 13,
/* power button is not connected to keypad scan matrix but to TWL3025 */
	[KEY_POWER]	= 31,
	[KEY_MINUS]	= 22,	/* not existent */
	[KEY_PLUS]	= 23,	/* not existent */
	[KEY_CAMERA]	= 24,	/* not existent */
};

#endif /* _TSM30_KEYMAP_H */