#ifndef __INTERNAL__
#define __INTERNAL__
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "drivers/pinout.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "utils/uartstdio.h"

#define SystemClock g_ui32SysClock

// System clock rate in Hz.
extern uint32_t g_ui32SysClock;

extern void TivaInit();
extern void UARTprintf(const char *pcString, ...);
extern void WriteInternalLED(uint8_t onOrOff);
extern void Delay(uint32_t ms);

extern void __configure_uart(void);

#endif
