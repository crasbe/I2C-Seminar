/**
 * @file i2cusb.h
 * 
 * @brief Interface für das USB-ITS-Gerät für I2C-Seminar an der TUHH.
 * 
 * Diese Datei enthält alle Definitionen für die i2cusb.c und
 * wird zur Verwendung der Schnittstelle für das I2C-Interface
 * inkludiert.
 * 
 * @authors Christopher Büchse und Jan Burmeister
 * @date Sommersemester 2017
 */
#ifndef I2CUSB_H_
#define I2CUSB_H_

#include <stdbool.h>

// fuer Linux, Windows und MacOSX die passende Version laden
#ifdef __linux__
#include "seriell_linux.h"
#elif defined __WIN32
#include "seriell_win32.h"
#elif defined __WIN64
#include "seriell_win64.h"
#elif defined __APPLE__ && __MACH__ // MacOS X
#include "seriell_macosx.h"
#endif

// Konstanten
#define cTimeoutInMs 1000
#define cTimeoutInit 5000
#define SCL90 'A'  /*!< SCL 90kHz PCD8584 Clocktakt */
#define SCL45 'B'  /*!< SCL 45kHz */
#define SCL11 'C'  /*!< SCL 11kHz */
#define SCL1_5 'D' /*!< SCL 1.5kHz */

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

// Globale Variablen
/*!
 * Um mit dem Delphi-Interface übereinzustimmen, ist der Filedeskriptor
 * global definiert.
 */
extern int fd;

// Funktionen
extern void Init(int portNr, int takt);
extern void DeInit(void);
extern void serialDump(void);
extern char start_iic(bool MRX_ACK, char dest, char mode);
extern char stop_iic(void);
extern char wr_byte_iic(char b);
extern char rd_byte_iic(char* b, bool NOACK);
extern char restart_iic(bool MRX_ACK, char dest, char mode);
extern void wr_byte_port(char zuSchreiben);
extern void rd_byte_port(char* gelesen);
extern bool is_initialized(void);
extern void relais_on(void);
extern void relais_off(void);
extern void led_on(void);
extern void led_off(void);

// Funktionen zur Debug-Ausgabe
#if DEGUG
extern void decodeStatus(char status);
#endif

#endif // I2CUSB_H_
