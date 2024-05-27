#pragma once

#include <arduino.h>
#include "Pin_definition.h"
#include "Button.h"


unsigned long move_compteur(unsigned long current_Value, bool* Up, int output_pin, int waiting_time);
unsigned long move_compteur_to(long current_Value, long final_Value, bool* Up);
unsigned long reset_compteur_to_0(int reset_pin);
bool change_sens(int Up_Down_pin);
unsigned long increase(long current_Value, bool* Up, int waiting_time);
unsigned long decrease(long current_Value, bool* Up, int waiting_time);
