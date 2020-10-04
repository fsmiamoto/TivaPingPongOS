#ifndef __INTERNAL__
#define __INTERNAL__
#include <stdint.h>
#include <stdlib.h>

// System clock rate in Hz.
extern uint32_t g_ui32SysClock;

extern void ConfigureUART(void);
extern void UARTprintf(const char *pcString, ...);
extern void SetupMain();
extern void delay(uint32_t ms);
#endif
