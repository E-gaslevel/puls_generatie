/**
 * @file logger.h
 *
 * @date Jan 6, 2021
 * @author: marcel
 */

#ifndef SRC_LOGGER_H_
#define SRC_LOGGER_H_

//#include "printf.h"
//#include "app_log_config.h"


/*
 * Add new log names here. By setting the 0 to 1 it will print out the information.
 */
#if defined(APP_LOG_ENABLE) && APP_LOG_ENABLE
#define LOG_BT 1
#endif


#if defined(LOG_BT) && LOG_BT
#define log_bt(...)    printf(__VA_ARGS__)
#else
#define log_bt(...)
#endif

#endif /* SRC_LOGGER_H_ */
