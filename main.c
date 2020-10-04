// PingPongOS - P2
// Aluno: Francisco S Miamoto - 1450522
#include "tiva_core.h"

int main(void) {
  SetupMain();

  // Descomente um teste de cada vez ;)
  // teste_task_control_1();
  teste_task_control_2();
  // teste_task_control_3();

  // We are finished.  Hang around flashing D1.
  while (1) {
    WriteInternalLED(0);
    Delay(SystemClock / 10 / 3);
    WriteInternalLED(1);
    Delay(SystemClock / 10 / 3);
  }
}
