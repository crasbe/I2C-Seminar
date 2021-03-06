/**
 * @file i2cseminar.c
 *
 * @brief Hauptprogramm für die Ansteuerung der I2C-Geräte
 *
 * @authors Christopher Büchse und Jan Burmeister
 * @date Sommersemester 2018
 *
 * @todo MEHR DOKUMENTATION!
 */

#include "i2cusb/i2cusb.h"
#include "LCD_I2C.h"
#include "ublox.h"

int main(void) {

    char puffer[65535];
    unsigned int readBytes;
    int *t;

	Init(2, SCL90);
	initDisp(0x27, 16, 2);
	init();
	backlight();

    printstr("Hallo!", 6);
	//print("Test???");
    /*
    // u-blox Teil
    //while(true) { //! @TODO sinnvolle Abbruchbedingung hinzufügen
        // Anzahl der zu lesenden Bytes auslesen
        randomReadUblox(253, puffer, 2);
        //readBytes = ((unsigned int) puffer[0]) << 8; // High-Byte
        //readBytes += (unsigned int) puffer[1];

        printf("%X   %X   %u\n", puffer[0], puffer[1], readBytes);
        getchar();
        randomReadUblox(255, puffer, readBytes);

        printf("Empfangene Nachricht: \n");
        for(int i = 0; i < readBytes; i++) {
            printf("%c", puffer[i]);
        }
        printf("\n\n");
   //}

    */
	DeInit();

	return 0;
}
