/**
 * @file I2C-Micro.h
 *
 * @brief Microcontroller-Seite des Interfaces
 *
 * Diese Datei enthält alle Definitionen für die I2C-Micro.ino und
 * wird zur Verwendung der Schnittstelle für das I2C-Interface
 * inkludiert.
 *
 * @authors Christopher Büchse
 * @date Sommersemester 2018
 */
 
#ifndef I2C_Micro_H
#define I2C_Micro_H

// Pindefinitionen
#define IO_RELAY 1
#define IO_LED 2
#define IO_0 3
#define IO_1 4
#define IO_2 5
#define IO_3 6
#define IO_4 7
#define IO_5 8
#define IO_6 9
#define IO_7 10



// Konstanten
#define cTimeoutInMs 1000
#define cTimeoutInit 5000
#define SCL90 'A'  /*!< SCL 90kHz PCD8584 Clocktakt */
#define SCL45 'B'  /*!< SCL 45kHz */
#define SCL11 'C'  /*!< SCL 11kHz */
#define SCL1_5 'D' /*!< SCL 1.5kHz */

#define BAUDRATE 38400

/**
 * @defgroup Busstatus Busstatus-Rückgabewert für I2C-Befehle
 * @{
 * Die Funktionen #start_iic, #stop_iic, #wr_byte_iic, #rd_byte_iic und #restart_iic
 * besitzen einen char-Rückgabewert, der den aktuellen Status des
 * I2C-Busses bzw. des PCD8584-Controllers widergeben.
 *
 * Zum leichteren Überprüfen der Statusbits sind die folgende Masken
 * als Makros definiert.
 *
 * @see start_iic
 * @see stop_iic
 * @see wr_byte_iic
 * @see rd_byte_iic
 * @see restart_iic
 *
 * @def PIN
 * 'PIN', Bit 7, (sollte nach der Übertragung 0 sein)
 *
 * @def STS
 * 'STS', Bit 5, bei externer Stop-Condition aktiv
 *
 * @def BER
 * 'BER', Bit 4, Bus-Error: falsches Start oder Stop
 *
 * @def AD0LRB
 * 'AD0/BRB', Bit 3, falls AAS=0: letztes empangenes Bit, also Ack-Bit
 *
 * @def AAS
 * 'AAS', Bit 2, Adressed As Slave (als Slave-Receiver)
 *
 * @def LAB
 * 'LAB', Bit 1, Lost arbitration
 *
 * @def BB
 * 'BB-', Bit 0, Bus Busy, 1= Bus frei (nach Stop-Condition)
 * @}
 */

 /*
 * Bit | Bez. PCD8584 | Beschreibung
 * ----+--------------+------------------------------------------
 *  7  | PIN          | (sollte nach der Übertragung 0 sein)
 *  6  |              | 0
 *  5  | STS          | bei externer Stop-Condition aktiv
 *  4  | BER          | Bus-Error: falsches Start oder Stop
 *  3  | AD0/LRB      | falls AAS=0: letztes empf. Bit, also Ack-Bit
 *  2  | AAS          | Adressed As Slave (als Slave-Receiver)
 *  1  | LAB          | Lost arbitration
 *  0  | BB-          | Bus Busy, 1=Bus frei (nach Stop-Cond.)
 *
 * Zum leichteren Überprüfen der Bits stehen Masken zur Verfügung.
 */
#define PIN    (1 << 7) //0b10000000
#define STS    (1 << 5) //0b00100000
#define BER    (1 << 4) //0b00010000
#define AD0LRB (1 << 3) //0b00001000
#define AAS    (1 << 2) //0b00000100
#define LAB    (1 << 1) //0b00000010
#define BB     (1 << 0) //0b00000001

#endif I2C_Micro_H
