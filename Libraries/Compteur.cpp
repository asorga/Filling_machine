#include "compteur.h"


unsigned long move_compteur(unsigned long current_Value, bool* Up, int output_pin, int waiting_time){
  /*
   * This function will both change the printed value on the counter and return this value  
   * 
   */

  //If we are at 0, we won't decrease anymore

  if (button_pushed(Button_Up_Filling)){
    return increase(current_Value, Up, waiting_time);
  }
  return decrease(current_Value, Up, waiting_time);
}


unsigned long move_compteur_to(long current_value, long final_value, bool* Up){
  if (current_value == final_value){
    return final_value;
  }
  
  unsigned long position_compteur = current_value;
  
  while(position_compteur != final_value){
    if (final_value < position_compteur){
      position_compteur = decrease(position_compteur, Up, 10);
    }else{
      position_compteur = increase(position_compteur, Up, 10);
    }
    
  }
  return position_compteur;
}


unsigned long reset_compteur_to_0(int reset_pin){
  /*
   * we put the counter back to 0
   */
  digitalWrite(reset_pin, HIGH);
  delay(1);
  digitalWrite(reset_pin, LOW);
  delay(1);
  return 0;
}


bool change_sens(int Up_Down_pin){
  /*
   * We switch the pin of the counter and return the appropriate boolean (true = Up/ false = Down)
   */
  if (digitalRead(Up_Down_pin) == HIGH){
    digitalWrite(Up_Down_pin, LOW);
    return false;
  }
  digitalWrite(Up_Down_pin, HIGH);
  return true;
  
}




unsigned long increase(long current_Value, bool* Up, int waiting_time){
  if (!(*Up)){
    *Up = change_sens(Compteur_Up_Down);
  }

  
  
  digitalWrite(Compteur_Change_Number, HIGH);
  delay(1);
  digitalWrite(Compteur_Change_Number, LOW);
  delay(waiting_time);
  return current_Value + 1;
  
}


unsigned long decrease(long current_Value, bool* Up, int waiting_time){
  
  if (current_Value == 0){
   return 0;
  }

  if (*Up){
    *Up = change_sens(Compteur_Up_Down);
  }

  
  
  digitalWrite(Compteur_Change_Number, HIGH);
  delay(1);
  digitalWrite(Compteur_Change_Number, LOW);
  delay(waiting_time);
  return current_Value - 1;
  
}
