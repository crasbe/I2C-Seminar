/**
 * @file i2cseminar.c
 *
 * @brief Hauptprogramm für die Ansteuerung der I2C-Geräte
 *
 * @authors Christopher Büchse und Jan Burmeister
 * @date Sommersemester 2017
 *
 * @todo MEHR DOKUMENTATION!
 */

#include "i2cusb/i2cusb.h"
#include "LCD_I2C.h"

int main(void) {
	Init(8, SCL90);
	initLCD(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
	begin(16, 2, LCD_5x8DOTS);
	DeInit();

	return 0;
}
