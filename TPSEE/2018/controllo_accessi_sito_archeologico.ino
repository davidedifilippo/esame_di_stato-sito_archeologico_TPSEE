#include <Wire.h>
#include <RTClib.h>
#include <SparkFun_SHTC3.h> 
#include <SparkFunTMP102.h>

//Dichiaro un oggetto di tipo RTC
RTC_DS3231 myRTC;

//Dichiaro un oggetto di tipo SHTC3 
SHTC3 mySHTC3;  

//Dichiaro un oggetto di tipo TMP102
TMP102 myTMP102;

const int pinRelayPompaNebulizzatore = 5;
const int pinRelayRiscaldatori = 2;
const int pinRelayAreatori = 3;


int mese;
int temp_Massima;
int temp_Mininima;
int temp_Media;
int temperatura_numerica = 0;
int temperatura = 0;

int RH = 0;
int RH_Mininima = 40;
int RH_Massima = 50;

bool nebulizzatoreAttivo = false;
bool areatoreAttivo = false;

long int tempoAvvioNebulizzatore;
long tempoAvvioAreatore;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  //Inizializzo la porta I2C
  Wire.setSDA(20);
  Wire.setSCL(21);
  Wire.begin();

  if(!mySHTC3.begin()){
    Serial.println("Controlla le linee di collegamento con SHTC3!");
    while(true); //Si blocca l'esecuzione
  }

  if(!myRTC.begin()){
    Serial.println("Controlla le linee di collegamento con RTC!");
    while(true); //Si blocca l'esecuzione
  }

  if(!myTMP102.begin()){
  Serial.println("Controlla le linee di collegamento con TMP102!");
  while(true); //Si blocca l'esecuzione
  }
  
  //Inizializzo data e ora nel real time clock
  myRTC.adjust(DateTime(F(__DATE__), F(__TIME__))); //si imposta sul tempo di compilazione 
 
}

void loop() {

  //myRTC.now() restituisce un oggetto di tipo DateTime e si può accedere alle singole parti giorno, ora, mese...
  DateTime data = myRTC.now();
  mese = data.month();
  Serial.print(mese);

  
  if(mese <= 5 || mese >= 11) { 
    //inverno
   temp_Massima = 12;
   temp_Mininima = 6;
   temp_Media = 9;
  }
  else{ 
    //estate
   temp_Massima = 24;
   temp_Mininima = 20;
   temp_Media = 22;
  }

 /*************************************
  *Controllo della temperatura ambiente
  *************************************/

  temperatura = myTMP102.readTempC(); //leggo dal sensore la temperatura in gradi centigradi

  Serial.print("temperatura: ");
  Serial.println(temperatura);
  
  if (temperatura < temp_Mininima) 
    digitalWrite(pinRelayRiscaldatori, HIGH);

  if (temperatura > temp_Media) 
    digitalWrite(pinRelayRiscaldatori, LOW); 
  else digitalWrite(pinRelayAreatori, LOW);

  if (temperatura > temp_Massima) 
    digitalWrite(pinRelayAreatori, HIGH);

 /*************************************
  *Controllo dell'umidità dell'ambiente
  *************************************/

  mySHTC3.update();   
  RH = mySHTC3.toPercent();
  Serial.print("RH = "); 
  Serial.println(RH); 

  if (RH < RH_Mininima) {
    digitalWrite(pinRelayPompaNebulizzatore, HIGH);  //Si attiva il nebulizzatore
    nebulizzatoreAttivo = true;
    tempoAvvioNebulizzatore = millis();
  }

  if (nebulizzatoreAttivo == true)
    if(millis()- tempoAvvioNebulizzatore > 10*60*1000)  digitalWrite(pinRelayPompaNebulizzatore, LOW);
  
  if (RH > RH_Massima) {
    digitalWrite(pinRelayAreatori, HIGH);  //Si attiva l'areatore
    areatoreAttivo = true;
    tempoAvvioAreatore = millis();
  }

  if (areatoreAttivo == true)
    if(millis()- tempoAvvioAreatore > 10*60*1000)  digitalWrite(pinRelayPompaNebulizzatore, LOW);
  
 /*************************************
  *Controllo accessi
  *************************************/
  
   
  
  

}
