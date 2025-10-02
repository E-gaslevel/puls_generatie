/**
 * @file bluetooth.h
 *
 * @date Jan 7, 2021
 * @author marcel
 */

#ifndef SRC_BLUETOOTH_H_
#define SRC_BLUETOOTH_H_

#include <stdint.h>

static const uint32_t adv_interval_min = 0x20;	/**< (0x0) Public device address */
static const uint32_t adv_interval_max = 0x20;	/**< (0x0) Public device address */
static const uint16_t adv_duration	   = 0xFFFF;	/**< (0x0) Public device address */
static const uint8_t  adv_maxevents    = 0x01;	/**< (0x0) Public device address */


void bluetooth_set_advertisement(void);
void bluetooth_create_advertisement(void);
void bluetooth_start_advertisement(void);

#endif /* SRC_BLUETOOTH_H_ */
