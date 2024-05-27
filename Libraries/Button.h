#ifndef BUTTON_h
#define BUTTON_H

#include <arduino.h>


int button_pushed(int button);
int button_pushed_once(int button, bool* last_pos_button);


#endif
