#ifndef MPUSTEPPER_H
#define MPUSTEPPER_H

#include "Arduino.h"

class MpuStepper
{
  public:
    MpuStepper(int pin1, int pin2, int pin3, int pin4);
    void setSlowness(int d);
    void write(int steps);
  private:
    int tic, tac;
    bool even, forward;
    int inputPins[4];
    int stepperDelay;
};

#endif //MPUSTEPPER_H
