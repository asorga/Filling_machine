# Description
This Arduino project aims to automate the filling process of different oil products using a volumetric vacuum pump. The system allows up to 10 different volumes to be stored in the Arduino's EEPROM memory, providing great flexibility for filling different quantities of oil. It is also equiped with a learning mode in order to update the memory whenever it is necessary.

# Key Features

- Volume Storage: Up to 10 different volumes can be saved in the EEPROM memory.

- Automatic Filling: Uses a volumetric vacuum pump equiped with a rotary encoder for precise filling.

- Can fill with a precision of half a milliliter.

- Equipped with a learning mode for manual filling and updates in the memory system.

- User Interface: rotary knob and pushbuttons for selecting and adjusting the volumes to be filled.

- Reliability: EEPROM memory is used to retain volumes even after a system restart.


# Hardware

- Arduino mega 2560 REV3

- [Infineon DC-Motor-Control-BTN8982TA](https://github.com/Infineon/DC-Motor-Control-BTN8982TA/tree/master?tab=readme-ov-file#dc-motor-control-btn8982ta-library) 

- [15KU DC Gear Peristaltic Pump with encoder](https://www.boxerpumps.com/15ku-dc-gear-peristaltic-pump/15412.301)

- [24V alim](https://fr.rs-online.com/web/p/alimentations-a-decoupage/1065846?searchId=119f4df4-a18b-4abf-b1a1-37b2674d6acb&gb=s)

- Mild Steel IP66Wall Box

- Waterproof Plexo switch
