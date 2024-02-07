#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 RTC;
int mese;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.setSDA(20);
  Wire.setSCL(21);
  Wire.begin();

  if(!RTC.begin()){
    Serial.println("Controlla le linee di collegamento con RTC, SDA e SCL. ");
    while(true);
  }

  RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
 
}

void loop() {
  // put your main code here, to run repeatedly:

  mese = RTC.now().month();
  Serial.print(mese);

}
