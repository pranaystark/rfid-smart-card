/*

* Pin layout should be as follows (on Arduino Uno):
* MOSI: Pin 11 / ICSP-4
* MISO: Pin 12 / ICSP-1
* SCK: Pin 13 / ISCP-3
* SS: Pin 10
* RST: Pin 9
*/
 
#include <SPI.h>
#include <RFID.h>
 
#define SS_PIN 10
#define RST_PIN 9
 
RFID rfid(SS_PIN,RST_PIN);
 
int startAlarm = false;
int resetAlarm = 2;
int relay = 7;
int alarm = 8;
 
int serNum[5];
 
int cards[][5] = {
  {185,5,133,158,167
}, // card 1
  {136,4,142,68,70

} // card 2
};
 
bool access = false;
 
void setup(){
 
    Serial.begin(9600);
    SPI.begin();
    rfid.init();
    pinMode(resetAlarm, INPUT);
    pinMode(relay, OUTPUT);
    pinMode(alarm, OUTPUT);
    digitalWrite(relay, HIGH);// or LOW if you have a regular relay
   
     //digitalWrite(relay, LOW);// or LOW if you have a regular relay
    
  
    //attachInterrupt(0, reset_alarm, LOW);
}
void loop(){
    
    if(rfid.isCard()){
    
        if(rfid.readCardSerial()){
            Serial.print(rfid.serNum[0]);
            Serial.print(",");
            Serial.print(rfid.serNum[1]);
            Serial.print(",");
            Serial.print(rfid.serNum[2]);
            Serial.print(",");
            Serial.print(rfid.serNum[3]);
            Serial.print(",");
            Serial.print(rfid.serNum[4]);
            Serial.println("");
            
            for(int x = 0; x < sizeof(cards); x++){
              for(int i = 0; i < sizeof(rfid.serNum); i++ ){
                  if(rfid.serNum[i] != cards[x][i]) {
                      access = false;
                      break;
                  } else {
                      access = true;
                  }
              }
              if(access) break;
            }
           
        }
        
       if(access){
          Serial.println("Welcome! \n pranay!"); 
          startAlarm = false;
          digitalWrite(relay, LOW); 
            delay(1500); 
            digitalWrite(relay, HIGH);  
           
           // delay(5000);
       } else {
           Serial.println("Not allowed!"); 
           startAlarm = true; 
           
       }        
    }
    
    if(startAlarm) {
       digitalWrite(alarm, HIGH);
       //delay(5); 
    } else {
      digitalWrite(alarm, LOW);
      
       
    }
    
    rfid.halt();
 
}
 
void reset_alarm(){
    startAlarm = false;
}
