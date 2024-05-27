#pragma once

#include <arduino.h>
#include "Pin_definition.h"





//float compute_PID(float Kp, float Ki, float Kd, int target, int current_position, int* eprev, float* eintegral, float deltaT);
void stop_motor();
void uniDirectionPowerOne(int inputUniPower);
void uniDirectionPowerTwo(int inputUniPower);
//void biDirectionPower(int inputBiPower);
