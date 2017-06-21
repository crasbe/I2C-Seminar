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

int main(void) {
	Init(6, SCL90);

	start_iic(false, (char) 0xFF, 'r');

	char answer;

	rd_byte_iic(&answer, false);

	DeInit();

	return 0;
}
