#ifndef __VIDEO_GR_H

#define __VIDEO_GR_H

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

/**
 * @brief Initializes the video module in graphics mode and private global variables
 *
 * @param mode 16-bit VBE mode to set
 *
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */

int vg_init(unsigned short mode);

/**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 *
 * @return 0 upon success, non-zero upon failure
 */

int vg_exit();


/**
 * @brief copy the secondary buffer into primary buffer
 *
 */
void copyBuffer();


/**
 * @brief return the secondary buffer address
 *
 * @return secondary buffer address
 */

char * getSecondaryBuffer();


/**
 * @brief gets vertical resolution
 *
 * @return vertical resolution
 */
int getVerResolution();


/**
 * @brief gets horizontal resolution
 *
 * @return horizontal resolution
 */
int getHorResolution();

/**@}*/

#endif /* __VIDEO_GR_H */
