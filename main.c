// PingPongOS - P2
// Aluno: Francisco S Miamoto - 1450522
#include <stdbool.h>
#include <stdint.h>

#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "drivers/pinout.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "internal.h"
#include "utils/uartstdio.h"

// System clock rate in Hz.
uint32_t g_ui32SysClock;

int main(void) {
  SetupMain();

  UARTprintf("***********************\n");
  UARTprintf("*     Ping Pong OS    *\n");
  UARTprintf("***********************\n");

  // Descomente um teste de cada vez ;)
  // teste_task_control_1();
  teste_task_control_2();
  //teste_task_control_3();

  // We are finished.  Hang around flashing D1.
  while (1) {
    LEDWrite(CLP_D1, 1);
    SysCtlDelay(g_ui32SysClock / 10 / 3);
    LEDWrite(CLP_D1, 0);
    SysCtlDelay(g_ui32SysClock / 10 / 3);
  }
}
