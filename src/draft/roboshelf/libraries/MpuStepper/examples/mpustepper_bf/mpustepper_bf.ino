#include <MpuStepper.h>

MpuStepper stppr(0, 1, 2, 3);

void setup() {
  stppr.setDelay(6);
}

void loop() {
  stppr.write(-400);
  stppr.write(400);
}
