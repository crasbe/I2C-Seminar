/***********************************************************************
 * i2cusb.h
 * 
 * Interface fuer das USB-ITS-Geraet fuer I2C-Seminar an der TUHH.
 * 
 * Diese Datei enthaelt alle Definitionen fuer die i2cusb.c und
 * wird zur Verwendung der Schnittstelle fuer das I2C-Interface
 * inkludiert.
 * 
 * Autoren: Christopher Buechse und Jan Burmeister
 * Datum: Sommersemester 2017
 **********************************************************************/
#ifndef I2CUSB_H_
#define I2CUSB_H_

#include <stdbool.h>

#define cTimeoutInMs 1000
#define cTimeoutInit 5000
#define SCL90 'A'  // SCL 90kHz PCD8584 Clocktakt
#define SCL45 'B'  // SCL 45kHz
#define SCL11 'C'  // SCL 11kHz
#define SCL1_5 'D' // SCL 1.5kHz


extern void init(unsigned int portNr, unsigned int takt);
extern void serialDump(void);

extern char start_iic(bool MRX_ACK, unsigned short dest, char mode);
extern char stop_iic(void);
extern char wr_byte_iic(char b);
extern char rd_byte_iic(char* b, bool NOACK);
extern char restart_iic(bool MRX_ACK, char dest, char mode);
extern void wr_byte_port(char zuSchreiben);
extern void rd_bate_port(char* gelesen);

extern bool is_initialized(void);
extern void relais_on(void);
extern void relais_off(void);
extern void led_on(void);
extern void led_off(void);

#endif // I2CUSB_H_
