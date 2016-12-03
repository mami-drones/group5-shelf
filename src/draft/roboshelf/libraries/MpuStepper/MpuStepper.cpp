/*
	MpuStepper.cpp -- Library for controlling
	4-pin stepper motors with half-steps.
	
	Differences from standart arduino library:
	* this library define speed if motor by simple 
	  delay in milliseconds beetween steps;
*/




#include "Arduino.h"
#include "MpuStepper.h"

MpuStepper::MpuStepper(int pin1, int pin2, int pin3, int pin4)
{
  this->inputPins[0] = pin1; pinMode(pin1, OUTPUT);
  this->inputPins[1] = pin2; pinMode(pin2, OUTPUT);
  this->inputPins[2] = pin3; pinMode(pin3, OUTPUT);
  this->inputPins[3] = pin4; pinMode(pin4, OUTPUT);
  this->_tic=3;
  this->_tac=2;
  this->even=true;
  this->forward=true;
  this->stepperDelay=2;
};

void MpuStepper::setSlowness(int d)
{
  this->stepperDelay = d;
};

void MpuStepper::write(int steps)
{
  int times = abs(steps);
  int dir = steps/times;
  int tic=this->_tic;
  int tac=this->_tac;
  int swaptic;
  int stepdelay=this->stepperDelay;
  #ifdef DEBUG_STEPPER
  char debugmsg[256];
  static char debugpin[4];// = {'?', '?', '?', '?'};
  #endif

  if (( dir<0 && forward) || (dir>0 && !forward))
  {
    forward=!forward;
    swaptic = tic;
    tic=tac;
    tac=swaptic;
  }

  #ifdef MPUSTEPPER_DEBUG
  sprintf(
    debugmsg,
    "\nTIC=%i TAC=%i\n DIR=%i STEPS=%i TIMES=%i\n",
    tic, tac, dir, steps, times
  );
  Serial.write(debugmsg);
  #endif
    
  
  for (int i=0; i<times; i++)
  {
    if (even)
    {
      digitalWrite(inputPins[tac], LOW);
      
      #ifdef MPUSTEPPER_DEBUG_IO
      sprintf(debugmsg, "E dW%i=%u; TIC=%i, TAC=%i\n",inputPins[tac], 0, tic, tac);
      Serial.write(debugmsg);
      #endif
      #ifdef MPUSTEPPER_DEBUG
      debugpin[tac]='.';
      #endif
      
      tic=(tic+dir+4)%4;
    }
    else
    {
      digitalWrite(inputPins[tic], HIGH);
      
      #ifdef MPUSTEPPER_DEBUG_IO
      sprintf(debugmsg, "O dW%i=%u; TIC=%i, TAC=%i\n",inputPins[tic], 1, tic, tac);
      Serial.write(debugmsg);
      #endif
      #ifdef MPUSTEPPER_DEBUG
      debugpin[tic]='#';
      #endif
      
      tac=(tac+dir+4)%4;
    }
    if (delay) delay(stepdelay);
    even=!even;
    
    #ifdef MPUSTEPPER_DEBUG
    sprintf(debugmsg, "%c%c%c%c\n", debugpin[0], debugpin[1], debugpin[2], debugpin[3] );
    Serial.write(debugmsg);
    #endif
  }
  
  this->_tac = tac;
  this->_tic = tic;
};

