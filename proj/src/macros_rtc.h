#pragma once
#include <lcom/lcf.h>



/** @defgroup macros_rtc macros_rtc
 * @{
 *
 * Constants for programming the rtc
 */

/** @brief RTC's address internal register */
#define RTC_ADDR_REG  0x70 

/** @brief RTC's data register */
#define RTC_DATA_REG  0x71

/** @brief Second's address location*/
#define SECOND        0x00

/** @brief Minute's address location*/
#define MINUTE        0x02

/** @brief Hour's address location*/
#define HOUR          0x04

/** @brief Day's address location*/
#define DAY           0x07

/** @brief Month's address location*/
#define MONTH         0x08

/** @brief Year's address location*/
#define YEAR          0x09

/** @brief Update in progress flag*/
#define RTC_UIP       BIT(7)

/** @brief Data format is BCD*/
#define RTC_BCD_MODE  BIT(2)

/** @brief Address of register A*/
#define RTC_REG_A     0x0A

/** @brief Address of register B*/
#define RTC_REG_B     0x0B

/**@}*/


