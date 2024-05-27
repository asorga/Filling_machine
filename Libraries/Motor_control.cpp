#include "Motor_control.h"








void stop_motor(){
  analogWrite(IN_1, 0);
  analogWrite(IN_2, 0);

}

void uniDirectionPowerOne(int inputUniPower){
  analogWrite(IN_1, inputUniPower);
}

void uniDirectionPowerTwo(int inputUniPower){
  analogWrite(IN_2, inputUniPower);
}
