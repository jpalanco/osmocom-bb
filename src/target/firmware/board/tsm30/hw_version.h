/* Hardware version detection from EEPROM/TIFFS for TSM30 */

#ifndef TSM30_HW_VERSION_H
#define TSM30_HW_VERSION_H

/* Hardware version definitions */
#define TSM30_HW_VERSION_REV_C   0
#define TSM30_HW_VERSION_REV_D   1
#define TSM30_HW_VERSION_REV_E   2

/* Configuration indices for different hardware versions */
#define TSM30_CONFIG_PRIMARY     0  /* Default configuration */
#define TSM30_CONFIG_ALT         1  /* Alternative configuration */
#define TSM30_CONFIG_VARIANT     2  /* Variant configuration */

/* Function prototypes */
int tsm30_read_hw_version_from_eeprom(void);
int tsm30_detect_hardware_version(void);
int tsm30_get_internal_version(void);
int tsm30_get_rf_version(void);
void tsm30_apply_rf_patches(void);
void tsm30_hw_version_init(void);

#endif /* TSM30_HW_VERSION_H */