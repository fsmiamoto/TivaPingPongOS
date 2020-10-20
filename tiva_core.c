#include "tiva_core.h"

#include "ppos.h"

uint32_t g_ui32SysClock;
uint32_t g_ui32Flags;

extern unsigned int system_tick_count;
extern task_t* current_task;

void ppos_tick_handler();

void TivaInit() {
  // Run from the PLL at 120 MHz.
  g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                           SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                                          120E6);

  // Configure the device pins.
  PinoutSet(false, false);

  // Enable the GPIO pins for the LED D1 (PN1).
  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);

  //
  // Enable the peripherals used by this example.
  //
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

  //
  // Enable processor interrupts.
  //
  ROM_IntMasterEnable();

  //
  // Configure the two 32-bit periodic timers.
  //
  ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
  ROM_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
  ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, g_ui32SysClock);
  ROM_TimerLoadSet(TIMER1_BASE, TIMER_A, g_ui32SysClock / 2);

  //
  // Setup the interrupts for the timer timeouts.
  //
  ROM_IntEnable(INT_TIMER0A);
  ROM_IntEnable(INT_TIMER1A);
  ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
  ROM_TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

  //
  // Enable the timers.
  //
  ROM_TimerEnable(TIMER0_BASE, TIMER_A);
  ROM_TimerEnable(TIMER1_BASE, TIMER_A);

  __configure_uart();
}

// Configure the UART and its pins.  This must be called before UARTprintf().
void __configure_uart(void) {
  // Enable the GPIO Peripheral used by the UART.
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

  // Enable UART0
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

  // Configure GPIO Pins for UART mode.
  ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
  ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
  ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

  // Initialize the UART for console I/O.
  UARTStdioConfig(0, 115200, g_ui32SysClock);
}

void WriteInternalLED(uint8_t onOrOff) { LEDWrite(CLP_D1, onOrOff); }

void Delay(uint32_t ms) { SysCtlDelay(ms); }

// The interrupt handler for the first timer interrupt.
void Timer0IntHandler(void) {
  char cOne, cTwo;

  // Clear the timer interrupt.
  ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

  // Toggle the flag for the first timer.
  HWREGBITW(&g_ui32Flags, 0) ^= 1;

  // Use the flags to Toggle the LED for this timer
  GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, g_ui32Flags);

  // Update the interrupt status.
  ROM_IntMasterDisable();
  cOne = HWREGBITW(&g_ui32Flags, 0) ? '1' : '0';
  cTwo = HWREGBITW(&g_ui32Flags, 1) ? '1' : '0';
  ROM_IntMasterEnable();
}

// The interrupt handler for the second timer interrupt.
void Timer1IntHandler(void) {
  char cOne, cTwo;

  // Clear the timer interrupt.
  ROM_TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

  // Toggle the flag for the second timer.
  HWREGBITW(&g_ui32Flags, 1) ^= 1;

  // Use the flags to Toggle the LED for this timer
  GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, g_ui32Flags);

  // Update the interrupt status.
  ROM_IntMasterDisable();
  cOne = HWREGBITW(&g_ui32Flags, 0) ? '1' : '0';
  cTwo = HWREGBITW(&g_ui32Flags, 1) ? '1' : '0';
  ROM_IntMasterEnable();

  ppos_tick_handler();
}

void ppos_tick_handler() {
  system_tick_count++;
  current_task->tick_count++;

  if (current_task->is_system_task) return;

  current_task->tick_budget -= 1;

  if (current_task->tick_budget == 0) {
    task_yield();
  }
}
