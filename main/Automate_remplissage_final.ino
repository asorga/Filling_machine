//####################################################################################################################################################
// Automate remplissage
// nombre d'heures perdues à améliorer cette version : 4
//####################################################################################################################################################

//importation des bibliotèques
#include "Pin_definition.h"
#include "read_pos_switch.h"
#include "Button.h"
#include "Compteur.h"
#include "Motor_control.h"
#include "Mem.h"

//definition du type Etat représentant l'état du système à tout instant
enum Etat {
  Et_Dem,
  Et_Selection,
  Et_Remplissage,
  Et_Apprentissage
};



//####################################################################################################################################################
//####################################################################################################################################################
//DECLARATION DES VARIABLES

Etat Etat_Automate = Et_Dem;


/*
int V_GR10_12 = 1;          //adresse memoire des volumes
int V_GR10_35 = 3;  
int DI31 = 5;               //17
int DI33 = 7;               //34
int CLA3 = 9;               //160
int CLA4 = 11;
int CLA5 = 13;
int TS59_3 = 15;
int TS59_4 = 17;
*/

int MlToInc = 260;         //1 ml equals 214 increments pour l'eau, 260 pour l'huile
int red_stockage = 4;
//const long MlToInc = 214; 


bool last_pos_button_start = 0;            
bool last_pos_button_learn = 0;


//variables du compteur
int volume;                               //valeur affichée par le compteur à tout instant 
int counted_now = 0;                      //nombre de valeures comptées en une fois
bool Up = true;                           //sens de variation du compteur


//motor_control

int target = 0;                           //position voulue (= volume final)
int position_motor = 0;                   //position actuelle (= volume versé)
int direction_motor = 0;

int addr;                                 // Variable de stockage des addresses mémoires
//int temp = 0;                           //Variable de test




//####################################################################################################################################################
//####################################################################################################################################################
//FONCTIONS UTILES


void GestionInterruptionCodeurPinA()
{
  /*
   * Incremental Codeur Management 
   */
  if (digitalRead(codeurB) == HIGH){
    position_motor--;                     // If B leads A, we increment
  }else{
    position_motor++;                     // If A leads B, we decrement
  }
}


void move_to(int target){
  /*
   * This function makes the motor move to a given position. 
   * WARNING - This position is not a volume but a number of increment
   */

  int e;                                  // Static error 
  int wait = 0;                           // Number of loops at the correct position
  int Kp = 1;                             // Proportionnal action coefficient
  int sign = 1;
  int U;                                  // Output instruction

  while(1){
    /*
     * We stay in this loop until the end of the function
     */

    e = target - position_motor;          // Static error determination
    U = Kp*e;                             // Output instruction determination
    
    if (abs(e) < 10){                     // Any time we are close enough of our final position, we increment "wait"
      wait++;
    }else{                                // If we are too far, "wait" is reset
      wait = 0;
    }

    if (wait == 50){                      // After 50 loops near our target, we stop the function 
      stop_motor();
      return;
    }
    
    if(U > 0){                            // Sense of rotation 
      sign = 1;                           
    }else if (U < 0){
      sign = -1;
    }
    
    if (abs(U) > 250){                    // Max saturation
      U = 250;
    }
    
    else if (abs(U) < 50){                // Min Saturation 
      U = 50;
    }
    
    else{
      U = abs(U);
    }
    
    biDirectionPower(sign*U);           // Applying speed to the motor
    
    Serial.println(position_motor);     // Print the current position
    
    
  }  
}


int get_addr(){
  /*
   * this function will return the appropriate address depending on the users requierment
   */
  return get_pos_switch()*2;
}




void biDirectionPower(int inputBiPower){
  /*
   * This function controls the motor
   */
  if(inputBiPower>0){
    analogWrite(IN_1,inputBiPower);
    analogWrite(IN_2,0);
  
  }
  else{
    if(position_motor != 0){
      analogWrite(IN_1,0);
      analogWrite(IN_2,abs(inputBiPower));
    }
  }
}




/*
float compute_PID(float Kp, float Kd, int target, int current_position, int* eprev, float deltaT){
  //error
  int e = target-current_position;

  //derivative
  //float dedt = (e-*eprev)/(deltaT);
  
  *eprev = e;

  return Kp*e  + Kd*(e-*eprev)/(deltaT);  
  //return Kp*e;
}
*/

//####################################################################################################################################################
//####################################################################################################################################################
//FONCTIONS ETAT


void State_machine(){
  /*
   * This function will control our systeme using a State-Machine model
   */


   
  switch (Etat_Automate){
    case Et_Dem:      
      digitalWrite(Led_Filling, HIGH);
      delay(1500);                              // Ligthing the Led for 1.5s to indicate that the machine is turning on
      digitalWrite(Led_Filling, LOW);
      Etat_Automate = Et_Selection;
    break;



    case Et_Selection:
    /* 
     *  In this state, the user can either select the filling process with a stored volume from EEPROM or switch to learning mode
     */

    if (button_pushed(Learning_mode)){
      Etat_Automate = Et_Apprentissage;           // Switch to Learning-mode
      digitalWrite(Learning_Led, HIGH);      
      delay(500);
      digitalWrite(Learning_Led, LOW);
      delay(500);                       
      digitalWrite(Learning_Led, HIGH);      
      delay(500);
      digitalWrite(Learning_Led, LOW);
      Serial.println("Apprentissage");
      break;
    }


    
    if (button_pushed_once(Button_START_STOP, &last_pos_button_start)){
      addr = get_addr();                          // We determine the correct EEPROM address
      //Serial.println(addr); 
      target = get_value(addr) * red_stockage;    // We determine the number of increment  
      Serial.println(target);
      Etat_Automate = Et_Remplissage;             // Switch to Filling-mode
      //Serial.println(volume);
    }
      
    break;


    case Et_Remplissage:
    /*
     * In this state, we run the motor to the required position
     */

    digitalWrite(Led_Filling, HIGH); 
    Serial.println("debut remplissage");
    
    delay(500);
    move_to(target);
    Serial.println(position_motor);
    position_motor = 0;
    Serial.println("fin remplissage");
    Etat_Automate = Et_Selection;
    digitalWrite(Led_Filling, LOW);               // Turn off the Led
    break;
    

    case Et_Apprentissage:
      
    /*
     * In this State, the user fills the appropriate amount of oil and can save it in memory
     */
 
    while(button_pushed(Button_Up_Filling) || button_pushed(Button_Down)){
      // If either button is pushed, we run the loop
      if (button_pushed(Button_Up_Filling)){
        direction_motor = 1;
      }else{
        direction_motor = 0;
      }
      
      
     biDirectionPower(-200 + direction_motor*400);       // The PWM input is -200 if "-" is pushed and 200 if "+" is
/*
      
      if (Up == 1 || temp >= 10){           // Test version
        temp += - 10 + 20*Up;
      }
*/      
      //Serial.println(temp);
      //Serial.println((int)temp/MlToInc);
      Serial.println(position_motor);
      
    }
    stop_motor();
    
    volume = move_compteur_to(volume, long(position_motor/MlToInc), &Up);           // Update the counter to the correct position


    if (!button_pushed(Learning_mode)){                                             // When learning mode is set to 0, We exit the state
      position_motor = 0;
      Etat_Automate = Et_Selection;      
      Serial.println("Et_Selection");
      delay(500);
    }


    if(button_pushed_once(Button_Learn, &last_pos_button_learn)){                   // When the correct volume has been poured
      digitalWrite(Learning_Led, HIGH);                                             // Start Learning phase
      
      addr = get_addr();                                                            // We get the address
      //int to_learn = int(position_motor/MlToInc);
      //position_motor = temp;
      Serial.println("On apprend : ");
      Serial.println(position_motor);
      Serial.print("à l'adresse : ");
      Serial.println(addr);
      delay(1500);                                                                  // Delay to avoid errors in EEPROM update
      update_value(addr, (1.1*position_motor)/red_stockage);
      Serial.println("fin apprentissage");
      position_motor = 0;                                                           // Reset position
      volume = reset_compteur_to_0(Compteur_Reset_Pin);                             // Reset counter
      digitalWrite(Learning_Led, LOW);                                              // End Learning phase
      
    }

    break;

     
  }
}








//####################################################################################################################################################
//####################################################################################################################################################
//ACTIVE CODE







void setup() {
  Serial.begin(9600);

  pinMode(IS_1, INPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(INH_1, OUTPUT);
  pinMode(IS_2, INPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(INH_2, OUTPUT);

  analogWrite(IN_1, 0); 
  digitalWrite(INH_1, HIGH); 
  analogWrite(IN_2, 0); 
  digitalWrite(INH_2, HIGH);

  
  pinMode(S_2, INPUT_PULLUP);
  pinMode(S_4, INPUT_PULLUP);
  pinMode(S_6, INPUT_PULLUP);
  pinMode(S_8, INPUT_PULLUP);
  
  pinMode(Button_START_STOP, INPUT_PULLUP);
  pinMode(Button_Up_Filling, INPUT_PULLUP);
  pinMode(Button_Down, INPUT_PULLUP);
  pinMode(Learning_mode, INPUT_PULLUP);
  pinMode(Button_Learn, INPUT_PULLUP);
  
  pinMode(Compteur_Up_Down, OUTPUT);
  pinMode(Compteur_Change_Number, OUTPUT);
  pinMode(Compteur_Reset_Pin, OUTPUT); 

  pinMode(Learning_Led, OUTPUT);
  pinMode(Led_Filling, OUTPUT);
  
  pinMode(codeurA, INPUT_PULLUP);
  pinMode(codeurB, INPUT_PULLUP);


  digitalWrite(Compteur_Up_Down, HIGH);

  attachInterrupt(digitalPinToInterrupt(codeurA), GestionInterruptionCodeurPinA, RISING);
  volume = reset_compteur_to_0(Compteur_Reset_Pin);
}

void loop() {
  State_machine();
}
