#include "tiva_core.h"

uint32_t g_ui32SysClock;

void TivaInit() {
  // Run from the PLL at 120 MHz.
  g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                           SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                                          120E6);

  // Configure the device pins.
  PinoutSet(false, false);

  // Enable the GPIO pins for the LED D1 (PN1).
  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);

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
