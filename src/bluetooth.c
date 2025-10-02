/**
 * @file bluetooth.c
 *
 *  @date Jan 7, 2021
 *  @author marcel
 */


#include "bluetooth.h"

#include "logger.h"

#include "sl_status.h"
#include "sl_bluetooth.h"
#include "app_assert.h"



static uint8_t advertising_set_handle = 0xff; 	/**< Global var for our specific handle. Multiple can be made to change easily between modes. */
sl_status_t sc; 								/**< Global var for all the statuses that are returned */


void bluetooth_set_advertisement(){
    // Create an advertising set.
    sc = sl_bt_advertiser_create_set(&advertising_set_handle);
    app_assert(sc == SL_STATUS_OK, "[E: 0x%04x] Failed to create advertising set\n", (int)sc);

    // Set advertising interval to 100ms.
    sc = sl_bt_advertiser_set_timing(
      advertising_set_handle,
      adv_interval_min, // min. adv. interval (milliseconds * 1.6)
      adv_interval_max, // max. adv. interval (milliseconds * 1.6)
      adv_duration,   	// adv. duration
      adv_maxevents);  	// max. num. adv. events
    app_assert(sc == SL_STATUS_OK, "[E: 0x%04x] Failed to set advertising timing\n", (int)sc);

    sc = sl_bt_advertiser_set_report_scan_request(advertising_set_handle, 1);
    app_assert(sc == SL_STATUS_OK, "[E: 0x%04x] Failed to set advertising scan request\n", (int)sc);
}

void bluetooth_create_advertisement(){
    uint8_t flags[3] = {0x02, 0x01, 0x06};
    uint8_t shortend_name[5] = {0x04, 0x08, 'G', 'A', 'S'};

    uint8_t data[sizeof(flags)+sizeof(shortend_name)] = {};
    memcpy(data, flags, sizeof(flags));
    memcpy(&data[sizeof(flags)], shortend_name, sizeof(shortend_name));

    app_assert(sizeof(data) <= 31, "Size of advertisment more than 31 Bytes: %d!", sizeof(data));

    log_bt("complete advertisement: ");
    for(uint8_t i = 0; i < sizeof(data); i++){
    	log_bt("%.2x", data[i]);
    }log_bt("\r\n");

    sc = sl_bt_advertiser_set_data(advertising_set_handle, 0, sizeof(data), data);
    //if(sc != SL_STATUS_OK)
    	//sl_status_print(sc);
}


void bluetooth_start_advertisement(){
	//sc = sl_bt_advertiser_start(advertising_set_handle, advertiser_user_data, advertiser_connectable_scannable);
	app_assert(sc == SL_STATUS_OK, "[E: 0x%04x] Failed to start advertising\n",(int)sc);
}
