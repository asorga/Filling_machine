#include "Button.h"


int button_pushed(int button){
  /*
   * This function will return true as long as the button is pressed
   * button must be an input_pullup pin
   */
  return !(digitalRead(button));
};


int button_pushed_once(int button, bool* last_pos_button){
  /*
   * same as before exept that we will only detect the position once
   */
  int pos_button  = !(digitalRead(button));

  if (pos_button && !(*last_pos_button)){
    *last_pos_button = pos_button;
    return pos_button;
  }
  
  *last_pos_button = pos_button;
  return 0;
}
