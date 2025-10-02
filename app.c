/***************************************************************************//**
 * @file
 * @brief Core application logic.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
#include "em_common.h"
#include "app_assert.h"
#include "sl_bluetooth.h"
#include "app.h"

//#include "app_log.h"
//#include "sl_iostream_swo.h"
//#include "sl_udelay.h"
#include "sl_sleeptimer.h"
#include "gatt_db.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_system.h"

#include "src/adc.h"
#include "src/gpio.h"
#include "src/timer.h"
#include "src/logger.h"
#include "src/bluetooth.h"

 uint8_t Pulse = 3;   //amount of periods per pulse

// The advertising set handle allocated from Bluetooth stack.
static uint8_t advertising_set_handle = 0xff;
static uint32_t time = 0;
static uint8_t measurements_per_pulse;
static bool adv_done = false;

uint32_t sampleIndex = 0;                     //index of samples to be displayed in logger
uint16_t analog_data[sampleCount] = {0};

/**************************************************************************//**
 * Application Init.
 *****************************************************************************/
SL_WEAK void app_init(void)
{
  //app_log_iostream_set(SL_IOSTREAM_TYPE_SWO);
  //sl_iostream_swo_init();
  timer_set_tictoc_us();
  //app_log("init the program to check time of print\r\n");

  // Init DCDC regulator with kit specific parameters
  EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_DEFAULT;
  EMU_DCDCInit (&dcdcInit);

  adc_init(); //initializes the ADC
  gpio_init();

  timer_set_cryto(cryotimerPeriod_1k);
  Pulse = 3;
  /////////////////////////////////////////////////////////////////////////////
  // Put your additional application init code here!                         //
  // This is called once during start-up.                                    //
  /////////////////////////////////////////////////////////////////////////////
}

/**************************************************************************//**
 * Application Process Action.
 *****************************************************************************/
SL_WEAK void app_process_action(void)
{
  if(measurements_per_pulse >= 5){
      Pulse+=1;
      measurements_per_pulse = 0;
      if(Pulse >=4){
          Pulse = 1;
      }
  }

  //if(!adv_done) // we wait until the advertisment is done.
  //    return;
  gpio_wakeup();

  uint32_t pulse_frequency = 1000000;
  uint32_t dutycycle = 50;
  timer_pwm_pulse_init(pulse_frequency, dutycycle, Pulse); //pulz that piezo and wait.
  while(!timer_get_pulse_done());

  //timer_pulse_init(pulse_frequency); //pulz that piezo and wait.
  //while(!timer_get_pulse_done());

  timer_tick(); // start tick tock timer.
  adc_measure(analog_data, sampleCount);
  time = timer_tock();

  //app_log("SampleID=%d,pulses=%d,", measurements_per_pulse, Pulse);
  sampleIndex++;

  for (int i = 0; i < sampleCount; i++)
  {
      //app_log("%d,", analog_data[i]);
  }
  //app_log("\r\n");

  float time_sec = time/1000000.0;
  //app_log("time: [%d]us, [%.2f]sec\r\n", time, time_sec);
  bluetooth_create_advertisement();
  bluetooth_start_advertisement();
  adv_done = false;
  measurements_per_pulse++;
  timer_go_to_sleep(); // sleep in EM1 while sending advertisement.
  /////////////////////////////////////////////////////////////////////////////
  // Put your additional application code here!                              //
  // This is called infinitely.                                              //
  // Do not call blocking functions from here!                               //
  /////////////////////////////////////////////////////////////////////////////
}

/**************************************************************************//**
 * Bluetooth stack event handler.
 * This overrides the dummy weak implementation.
 *
 * @param[in] evt Event coming from the Bluetooth stack.
 *****************************************************************************/
void sl_bt_on_event(sl_bt_msg_t *evt)
{
  sl_status_t sc;

  switch (SL_BT_MSG_ID(evt->header)) {
    // -------------------------------
    // This event indicates the device has started and the radio is ready.
    // Do not call any stack command before receiving this boot event!
    case sl_bt_evt_system_boot_id:
      // Create an advertising set.
      sc = sl_bt_advertiser_create_set(&advertising_set_handle);
      app_assert_status(sc);

      // Generate data for advertising
      sc = sl_bt_legacy_advertiser_generate_data(advertising_set_handle,
                                                 sl_bt_advertiser_general_discoverable);
      app_assert_status(sc);

      // Set advertising interval to 100ms.
      sc = sl_bt_advertiser_set_timing(
        advertising_set_handle,
        160, // min. adv. interval (milliseconds * 1.6)
        160, // max. adv. interval (milliseconds * 1.6)
        0,   // adv. duration
        0);  // max. num. adv. events
      app_assert_status(sc);
      // Start advertising and enable connections.
      sc = sl_bt_legacy_advertiser_start(advertising_set_handle,
                                         sl_bt_legacy_advertiser_connectable);
      app_assert_status(sc);
      break;

    // -------------------------------
    // This event indicates that a new connection was opened.
    case sl_bt_evt_connection_opened_id:
      break;

    // -------------------------------
    // This event indicates that a connection was closed.
    case sl_bt_evt_connection_closed_id:
      // Generate data for advertising
      sc = sl_bt_legacy_advertiser_generate_data(advertising_set_handle,
                                                 sl_bt_advertiser_general_discoverable);
      app_assert_status(sc);

      // Restart advertising after client has disconnected.
      sc = sl_bt_legacy_advertiser_start(advertising_set_handle,
                                         sl_bt_legacy_advertiser_connectable);
      app_assert_status(sc);
      break;

    ///////////////////////////////////////////////////////////////////////////
    // Add additional event handlers here as your application requires!      //
    ///////////////////////////////////////////////////////////////////////////

    // -------------------------------
    // Default event handler.
    default:
      break;
  }
}
