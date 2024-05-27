#pragma once

#include <arduino.h>
#include "Pin_definition.h"

#define size_bin 4

int Bin2Dec(char* bin);
int is_switch_on(int pin);
int get_pos_switch();
