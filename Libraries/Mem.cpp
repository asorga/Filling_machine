#include "Mem.h"

int get_value(int addr){
  /*
   * We return the total value stored on 2 bytes
   * The Least Significant Bits (LSB) are stored in addr and the Most Significant Bits (MSB) are stored at "addr + 1"
   * 
   */
  return EEPROM.read(addr) + 256*EEPROM.read(addr + 1);
  
}


void update_value(int addr, int value){
  /*
   * This function change the value stored at the given adress
   */ 


  int LSB = value % 256;     //We get the value to store in addr by determine the 8 LSB and converting this value back to a decimal one
  int MSB = value / 256;     //Same thing with the MSB
  EEPROM.update(addr, LSB);
  EEPROM.update(addr + 1, MSB);
}
