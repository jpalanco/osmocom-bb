/* Hardware version detection from EEPROM/TIFFS for TSM30 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <tiffs.h>

/* TSM30 O&M parameters section structure */
#define TSM30_OM_PARAMS_SIZE    256  /* Size of O&M parameters section */
#define TSM30_HW_VERSION_OFFSET  33  /* Field 33 contains hardware version */
#define TSM30_INTERNAL_VERSION_OFFSET 44  /* Field 44 contains internal version */

/* TSM30 file paths in TIFFS */
#define TSM30_OM_PARAMS_PATH     "/gsm/om/params"
#define TSM30_RF_SECTION_PATH    "/gsm/rf/section"

/* RF section header structure */
typedef struct {
    uint8_t version;
    uint16_t checksum;
    uint8_t reserved[13];
} tsm30_rf_section_header_t;

/* Hardware version definitions from TSM30 source code */
#define TSM30_HW_VERSION_REV_C   0
#define TSM30_HW_VERSION_REV_D   1
#define TSM30_HW_VERSION_REV_E   2

static int tsm30_hw_version = -1;  /* Cached hardware version */
static int tsm30_internal_version = -1;  /* Cached internal version */
static int tsm30_rf_version = -1;  /* Cached RF section version */

/* Read O&M parameters from TIFFS */
static int tsm30_read_om_params(uint8_t *om_params)
{
    size_t len;
    int result;

    /* Read O&M parameters file from TIFFS */
    result = tiffs_read_file_maxlen(TSM30_OM_PARAMS_PATH, om_params,
                                   TSM30_OM_PARAMS_SIZE, &len);

    if (result <= 0) {
        printf("TSM30: Failed to read O&M parameters from TIFFS\n");
        return -1;
    }

    if (len < TSM30_OM_PARAMS_SIZE) {
        printf("TSM30: O&M parameters too short (%zu bytes)\n", len);
        return -1;
    }

    printf("TSM30: O&M parameters read successfully (%zu bytes)\n", len);
    return 0;
}

/* Read RF section version from TIFFS */
static int tsm30_read_rf_section_version(void)
{
    tsm30_rf_section_header_t header;
    size_t len;
    int result;

    /* Read RF section header from TIFFS */
    result = tiffs_read_file_maxlen(TSM30_RF_SECTION_PATH, (uint8_t *)&header,
                                   sizeof(header), &len);

    if (result <= 0) {
        printf("TSM30: Failed to read RF section from TIFFS\n");
        return -1;
    }

    if (len < sizeof(header)) {
        printf("TSM30: RF section too short (%zu bytes)\n", len);
        return -1;
    }

    tsm30_rf_version = header.version;
    printf("TSM30: RF section version: %d (checksum: 0x%04x)\n",
           tsm30_rf_version, header.checksum);

    return tsm30_rf_version;
}

/* Read hardware version from EEPROM */
int tsm30_read_hw_version_from_eeprom(void)
{
    uint8_t om_params[TSM30_OM_PARAMS_SIZE];

    /* Read O&M parameters section */
    if (tsm30_read_om_params(om_params) < 0) {
        /* Failed to read, return default version */
        return TSM30_HW_VERSION_REV_C;
    }

    /* Extract hardware version from field 33 */
    tsm30_hw_version = om_params[TSM30_HW_VERSION_OFFSET];
    tsm30_internal_version = om_params[TSM30_INTERNAL_VERSION_OFFSET];

    printf("TSM30: Hardware version: %d (internal: %d)\n",
           tsm30_hw_version, tsm30_internal_version);

    return tsm30_hw_version;
}

/* Detect hardware version with caching */
int tsm30_detect_hardware_version(void)
{
    /* Read once and cache */
    if (tsm30_hw_version < 0) {
        tsm30_hw_version = tsm30_read_hw_version_from_eeprom();
    }

    /* Map TSM30 hardware versions to configuration indices:
     * Official HW revision (field 33):
     * 0 = rev C or rev D (could be old or new LCD)
     * 1 = rev D (new LCD assumed)
     * 2 = rev E (new LCD assumed)
     */

    switch (tsm30_hw_version) {
        case TSM30_HW_VERSION_REV_C:
            /* Need to check internal version (field 44) for old/new LCD */
            if (tsm30_internal_version > 0) {
                return 1;  /* Rev C with new LCD - use alt config */
            }
            return 0;  /* Default to primary config */

        case TSM30_HW_VERSION_REV_D:
            return 1;  /* Rev D with new LCD - alt config */

        case TSM30_HW_VERSION_REV_E:
            return 2;  /* Rev E - variant config */

        default:
            printf("TSM30: Unknown hardware version %d, using default config\n",
                   tsm30_hw_version);
            return 0;  /* Unknown - use primary config */
    }
}

/* Get internal version for detailed configuration */
int tsm30_get_internal_version(void)
{
    if (tsm30_internal_version < 0) {
        tsm30_detect_hardware_version();  /* Ensure version is read */
    }
    return tsm30_internal_version;
}

/* Get RF section version */
int tsm30_get_rf_version(void)
{
    if (tsm30_rf_version < 0) {
        tsm30_read_rf_section_version();  /* Ensure RF version is read */
    }
    return tsm30_rf_version;
}

/* Apply version-specific patches based on hardware and RF versions */
void tsm30_apply_rf_patches(void)
{
    int hw_version = tsm30_detect_hardware_version();
    int rf_version = tsm30_get_rf_version();

    printf("TSM30: Applying RF patches for HW version %d, RF version %d\n",
           hw_version, rf_version);

    /* Apply specific patches based on TSM30 source code logic */
    if (hw_version > 0 && rf_version == 10) {
        /* Apply specific APC timing patches for certain hardware/RF combinations */
        printf("TSM30: Applying APC timing patches for HW %d, RF %d\n",
               hw_version, rf_version);
        /* These patches would modify global RF parameters */
        /* Example: s_L1dGlobals.ApcDisableTime[0][2] = 0x17;  // instead of 0x18 */
    }

    /* Add more version-specific patches as needed */
}

/* Initialize hardware version detection */
void tsm30_hw_version_init(void)
{
    /* Read and cache hardware version */
    tsm30_detect_hardware_version();

    /* Read and cache RF section version */
    tsm30_read_rf_section_version();

    /* Apply version-specific patches */
    tsm30_apply_rf_patches();
}