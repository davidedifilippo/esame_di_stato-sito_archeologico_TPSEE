volatile unsigned int impulsi = 0; //incrementa orario/decrementa antiorario - 1 impulso = 1 mm

//pin di connessione

const int EncoderPin = 2; 

const int pinStartButton = 10;

const int pin_depressione = 11;   //Attiva/disattiva depressione per le ventose
const int pin_fine_corsa_1 = 11;  //Fine corsa sulla catasta
const int pin_fine_corsa_2 = 12;  //Fine corsa sul nastro

//pin di controllo del driver motore asse y 

const int pin_cw_y = 3;
const int pin_ccw_y = 4; 
const int pin_vel_y = 5; //deve essere un pin tipo PWM

//pin di controllo del driver motore asse z 

const int pin_cw_z = 7;
const int pin_ccw_z = 8; 
const int pin_vel_z = 9; //deve essere un pin tipo PWM


// variabili per gestire il processo

int marcia_z = 0; //motore z in stop 0 ; motore z marcia avanti = 1; motore z marcia indietro -1
bool start = false;
int conta_lastre = 20;

// ISR - interrupt service routine 

void conta_impulsi() {
  //attivato nella transizione da Livello Basso a Livello Alto
  if(marcia_z == 1) {
    //Se si muove in senso orario aumento i conteggi -> movimento z verso il basso
    impulsi++;
  }
  else{
    //Se si muove in senso orario diminuisco i conteggi -> movimento z verso l'alto
    impulsi--;
    }
  }
  
void setup() {
  pinMode(pin_cw_y, OUTPUT);
  pinMode(pin_ccw_y, OUTPUT);
  pinMode(pin_cw_z, OUTPUT);
  pinMode(pin_ccw_z, OUTPUT);
  
  pinMode(EncoderPin, INPUT_PULLUP); // internal pullup input pin 2 
    // attivazione interrupt sul pin dell'encoder
  attachInterrupt(EncoderPin, conta_impulsi, RISING); 
}
   
  void loop() {
    
  while(!start)
  start = digitalRead(pinStartButton);  //attesa inizio processo
  
  if (conta_lastre == 0 ) {
    //aspetto che le lastre siano sul pallet
    //se il sensore rileva solo il pallet scarico si bloca il programma
    while(analogRead(pinSensorewUltrasuoni)>=970); //il pallet è vuoto 
                                                 //100 mm * 5mV  = 0.5V ossia il 5% di 10V
                                                 //970 è il 95% di 1023
    conta_lastre = 20;
    //pallet carico
    }
  

  
  //movimento orario lungo Z per prendere la lastra
  
  digitalWrite(pin_cw_z, HIGH);
  marcia_z = 1;
  unsigned int distanza_riposo_pallet = 1500 - (100 + 6*(20-conta_lastre));
  while(impulsi < distanza_riposo_pallet){ 
    //i millimetri dipendono da quante lastre sono rimaste
    analogWrite(pin_vel_z, 0.4*256);  //PWM al 40% per regolare la velocità
    }
  digitalWrite(pin_cw_y, LOW);  //spengo il motore
  marcia_z = 0;
  
  digitalWrite(pin_depressione, HIGH); //afferro la lastra
  
  //movimento antiorario indietro lungo Z per tornare a riposo
  
  marcia_z = -1;
  analogWrite(pin_vel_z, 0.4*256);  //PWM al 40% per regolare la velocità
  digitalWrite(pin_ccw_z, HIGH);
  while(impulsi == 0) { //devo tormare alla posizione di riposo 
  }
  
  digitalWrite(pin_ccw_z, LOW);  //spengo il motore
  marcia_z = 0;  //motore in stop
 
  //movimento orario fino al centro del nastro 
  
  analogWrite(pin_vel_y, 0.4*256);  //PWM al 40% per regolare la velocità
  digitalWrite(pin_cw_y, HIGH);
 
  while(!pin_fine_corsa_2){ 
   
  }  
  
  analogWrite(pin_vel_y, 0);  //fermo il motore
  digitalWrite(pin_cw_y, LOW);  //spengo il motore

  
  //movimento orario lungo Z per posare la lastra
  analogWrite(pin_vel_y, 0.4*256);  //PWM al 40% per regolare la velocità
  digitalWrite(pin_cw_z, HIGH);
    
  marcia_z = 1;
  while(impulsi < 494){ //devo coprire 494 mm con 494 conteggi
  }
  analogWrite(pin_vel_z, 0);
  digitalWrite(pin_cw_y, LOW);  //spengo il motore
  marcia_z = 0;
  
  digitalWrite(pin_depressione, LOW); //mollo la lastra
  
  //movimento antiorario indietro lungo Z per tornare alla posizione z di riposo
  
  marcia_z = -1;
  analogWrite(pin_vel_y, 0.4*256);  //PWM al 40% per regolare la velocità
  digitalWrite(pin_ccw_z, HIGH);
  while(impulsi == 0) { //devo coprire 494 mm scalando 494 conteggi
    }
  marcia_z = 0;  //motore in stop
  analogWrite(pin_vel_z, 0);
  digitalWrite(pin_cw_z, LOW);  //spengo il motore
  
  
  //gripper in posizione di riposo
  //movimento antiorario fino al centro del pallet 
  analogWrite(pin_vel_y, 0.4*256);  //PWM al 40% per regolare la velocità
  digitalWrite(pin_ccw_y, HIGH);
 
  while(!pin_fine_corsa_1){ 
    
  }  
  
  analogWrite(pin_vel_y, 0);  //fermo il motore
  digitalWrite(pin_ccw_y, LOW);  //spengo il motore
  
  conta_lastre = conta_lastre - 1;
  }
  
  
   
