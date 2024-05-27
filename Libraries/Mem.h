#pragma once

#include <EEPROM.h>
#include <arduino.h>

#define size_bin_mem 8                      
int get_value(int addr);
void update_value(int addr, int value);
