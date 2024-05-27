#include "read_pos_switch.h"


int Bin2Dec(char* bin){
  //this function return the decimal value of the bineary string in argument

   float val = 0;           //this variable is used to keep in memory the decimal value within the loop

   for (int i = 0; i <= size_bin; i++){      //we look through the bineary number 

                                                //we start by the end of the loop 
    if (bin[size_bin - 1 - i] == '1'){      //we check wether or not the value is one
        val += pow(2.0, float(i));
    }
   }
  return round(val);
}



int is_switch_on(int pin){
  return !(digitalRead(pin));
 
}


int get_pos_switch(){
  /*
   * this function return the position of the switch
   */
  int S2 = is_switch_on(S_2);
  int S4 = is_switch_on(S_4);
  int S6 = is_switch_on(S_6);
  int S8 = is_switch_on(S_8);
  char pos_bin[size_bin];
  sprintf(pos_bin, "%d%d%d%d", S8, S6, S4, S2);
  //Serial.print("code_interrupteur : ");
  //Serial.println(pos_bin);
  return Bin2Dec(pos_bin);
}
