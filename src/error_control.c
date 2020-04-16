/*
 * error_control.c
 *
 *  Created on: Apr 1, 2020
 *      Author: Nicolas Villanueva
 */

#include "color_sensor.h"


void print_error(int error_code) {

	switch(error_code) {
		case 1:
			puts("ERROR: Could not open file descriptor");
			break;
		case 2:
			puts("ERROR: Could not find the I2C slave");
			break;
		case 3:
			puts("ERROR: Data write to I2C slave failed");
			break;
		case 4:
			puts("ERROR: Bytes sent do not match length expected.\n");
			// puts("ERROR: Data read from I2C slave failed");
			break;
		case 5:
			puts("ERROR: Input/Output error at data read");
			break;
		case 6:
			printf("ERROR: Data headers are incorrect (Function code & Number of bytes queried)");
			break;
	}
}
