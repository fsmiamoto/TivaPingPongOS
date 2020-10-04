#ifndef __INTERNAL__
#define __INTERNAL__

// System clock rate in Hz.
extern uint32_t g_ui32SysClock;

void ConfigureUART(void);
extern void UARTprintf(const char *pcString, ...);

#endif
