 /**
 * @file I2C-Micro.ino
 *
 * @brief Microcontroller-Seite des Interfaces
 *
 * Mikrocontroller-Programm, um das USB-ITS-Gerät zu
 * ersetzen (die Kommunikation mit dem Delphi-Code
 * und dem Host-Code in C scheint nicht so zu
 * funktionieren, wie gedacht.
 *
 * @authors Christopher Büchse
 * @date Sommersemester 2018
 */

#include <Wire.h>
//#include <Serial.h>

#include "I2C-Micro.h"
//#include "i2cmaster.h"

char message[3] = { 0, 0, 0 };


uint8_t deviceStatus = 0;

int slaveAdress = 0;


void setup() {

  /*pinMode(IO_RELAY, OUTPUT);
  pinMode(IO_LED, OUTPUT);
  pinMode(IO_0, OUTPUT);
  pinMode(IO_1, OUTPUT);
  pinMode(IO_2, OUTPUT);
  pinMode(IO_3, OUTPUT);
  pinMode(IO_4, OUTPUT);
  pinMode(IO_5, OUTPUT);
  pinMode(IO_6, OUTPUT);
  pinMode(IO_7, OUTPUT);*/
  
  Serial.begin(BAUDRATE);
  Wire.begin();
  //Serial.setTimeout(cTimeoutInMs);

  //i2c_init();
}

void loop() {
  
  // die ganze Nachricht empfangen
  if(Serial.available() > 0) {

    //Serial.print(Serial.available());
    
    // erstes Zeichen lesen, 
    //Serial.readBytes(message, 2);
    message[0] = Serial.read();
    message[1] = Serial.read();
  

    // Überprüfen, ob die Nachricht dem vorgegebenen Format entspricht
    switch(message[0]) {

      // Startcondition zum Schreiben erzeugen
      case 'T':
      case 'U':
        Wire.beginTransmission(message[1]);
        message[1] = 0;
        Serial.write(message, 2);
        break;
      
      // Startcondition zum Lesen erzeugen
      case 'S':
      case 's':
        Wire.requestFrom(message[1], 1);
        message[1] = 0;
        Serial.write(message, 2);
        break;

      // Restart zum Lesen erzeugen
      case 'V':
      case 'v':
        // Kann die Arduino-Wire Library natürlich nicht.
        Wire.requestFrom(message[1], 1);
        message[1] = 0;
        Serial.write(message, 2);
        break;

      // Stop Condition Erzeugen
      case 'O':
        if(message[1] == 'P') {
          Wire.endTransmission();
        } else {
          // Fehler
        }
        Serial.write(message, 2);
        break;

      // Schreiben auf I2C-bus
      case 'N':
        Wire.write(message[1]);
        Serial.write(message, 2);
        break;

      // Lesen vom I2C-Bus
      case 'R':
        // Die Arduino-Wire Library ist relativ "simpel", also vertrauen wir darauf,
        // dass sie das richtige tut..
        message[1] = Wire.read();
        message[2] = 0;
        Serial.write(message, 3);
        break;

      // I2C-Timing verändern
      case 'C':
        if(message[1] == SCL90) {
          Wire.setClock(90000);
        } else if(message[1] == SCL45) {
          Wire.setClock(45000);
        } else if(message[1] == SCL11) {
          Wire.setClock(11000);
        } else if(message[1] == SCL1_5) {
          Wire.setClock(1500);
        } else if(message[1] == 'Y' || message[1] == 'Z') {
          // Timeout-Verhalten definieren, momentan nicht implementiert
        } else {
          // Fehler
        }
        Serial.write(message, 2);
        break;

      // Slave-Adresse setzen (und den Controller im Slave-Modus starten)
      case 'A':
        //Wire.begin(message[1]);

        Serial.write(message, 2);
        break;

      // Slave-Adresse auslesen
      case 'B':
        if(message[1] == 'B') {
          message[1] = slaveAdress;
        }
        Serial.write(message, 2);
        break;

      // Byte auf den IO-Port schreiben
      case 'W':
        /*digitalWrite(IO_0, message[1]&0b00000001);
        digitalWrite(IO_1, message[1]&0x00000010);
        digitalWrite(IO_2, message[1]&0x00000100);
        digitalWrite(IO_3, message[1]&0x00001000);
        digitalWrite(IO_4, message[1]&0x00010000);
        digitalWrite(IO_5, message[1]&0x00100000);
        digitalWrite(IO_6, message[1]&0x01000000);
        digitalWrite(IO_7, message[1]&0x10000000);*/


        Serial.write(message, 2);
        break;

      // Byte vom IO-Port lesen
      case 'D':
       /* message[1] = digitalRead(IO_0);
        message[1] += digitalRead(IO_1) << 1;
        message[1] += digitalRead(IO_2) << 2;
        message[1] += digitalRead(IO_3) << 3;
        message[1] += digitalRead(IO_4) << 4;
        message[1] += digitalRead(IO_5) << 5;
        message[1] += digitalRead(IO_6) << 6;
        message[1] += digitalRead(IO_7) << 7;*/

        Serial.write(message, 2);
        break;
        

  /*   // LED ein- oder ausschalten
      case 'L':
        if(message[1] == '0') {
          digitalWrite(IO_LED, LOW);
        } else if(message[1] == '1') {
          digitalWrite(IO_LED, HIGH);
        } else {
          // Fehler
        }
        Serial.write(message, 2);
        break;
        
      // Relais anziehen oder abfallen lassen
      case 'P':
        if(message[1] == '0') {
          digitalWrite(IO_RELAY, LOW);
        } else if(message[1] == '1') {
          digitalWrite(IO_RELAY, HIGH);
        } else {
          // Fehler
        }
        Serial.write(message, 2);
        break;*/
        
      // Reset
      case 'X':

        Serial.write(message, 2);
        break;

      // funktionslos
      case 'E':
        if(message == 'E') {
          // nichts tun...
        } else {
          // Fehler
        }
        Serial.write(message, 2);
        break;

      default: // Fehlerhafte Nachricht empfangen.
               break;



      
    }



    //Serial.flush();
  }

}
