/*
 ============================================================================
 Name        : color_sensor.c
 Author      : Nicolas Villanueva
 Version     : 1.0.0 (March 2020)
 Description : Small program to detect colors with the TCS34725 RGB sensor
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdint.h>


#include "color_sensor.h"


int main(void) {

	puts("== TCS34725 RGB SENSOR ==\n\n");


	/* Step 0: Create instance of I2C interface's descriptor */
	int fd_i2c = get_i2c_descriptor();


	/* Step 2: Write to ATIME Register (0x01) to set Integration Time (700ms - 0x00) */
	puts("Writing ATIME register\n");
	write_cmd_byte(fd_i2c, TCS_REG_ATIME, 0x00);


	/* Step 3: Write to WTIME Register (0x03) to set Waiting Time (2.4ms - 0xFF) */
	puts("Writing WTIME register\n");
	write_cmd_byte(fd_i2c, TCS_REG_WTIME, 0xFF);


	/* Step 4: Write to AGAIN bits of Control Register (0x0F) to set AGAIN Value (x1 Gain RGBC Control value: 0x00) */
	puts("Writing Control register\n");
	write_cmd_byte(fd_i2c, TCS_REG_CONTROL, 0x00);


	/* Step 1: Write to Enable Register (0x00) to set PON, AEN and WEN bits (0, 1 and 3 respectively: 0x0C) */
	puts("Writing Enable register: PON bits \n");
	write_cmd_byte(fd_i2c, TCS_REG_ENABLE, 0x01);

	// There is a 2.4 ms warm-up delay if PON is enabled. (Maybe should be after something else)
	puts("Wait for 2.4 ms\n");
	usleep(2400);


	/* Step 1: Write to Enable Register (0x00) to set PON, AEN and (NOT WEN) bits (0, 1 and 3 respectively: 0x0C) */
	puts("Writing Enable register\n");
	write_cmd_byte(fd_i2c, TCS_REG_ENABLE, 0x03);

	// Since the integration time is 700ms, we are to wait that amount of time
	puts("Wait for 700 ms\n");
	usleep(700000);


	/* Step 5 - Read RGBC Channel Data Registers */

	// To ensure the data is read correctly, a two-byte read I2C transaction should be used with a read word protocol bit set in the command register:
	// Command Register: Bit5 = 1 for "Auto-increment protocol transaction": (0xa0 instead of 0x80)


	// For now, we'll avoid auto-increment
	/*
	registers[0] = TCS_CMD_AUTOINC | TCS_REG_DATA_C_LOW;
	sent_bytes = write(fd_i2c, registers, 1);
	if (sent_bytes < 0)
		print_error(3);
	else
		puts("\nTCS - read sequence begun\n");
	*/

	while(1) {
		uint8_t reg = 0x00;
		uint8_t data_byte;
		uint8_t data[8] = {0};
		ssize_t sent_bytes;
		ssize_t read_bytes;

		puts("\nTCS - read sequence begun.\n");
		reg = TCS_CMD_BYTE | TCS_REG_DATA_C_LOW; // TCS_CMD_AUTOINC | TCS_REG_DATA_C_LOW;

		int i;
		for (i = 0; i < 8; i++) {
			sent_bytes = write(fd_i2c, &reg, 1);
			if (sent_bytes < 0)
				print_error(3);

			read_bytes = read(fd_i2c, &data_byte, 1);
			if (read_bytes != 1)
				print_error(4);
			else
				data[i] = data_byte;

			reg++;
		}
		for (i = 0; i < 8; i++) {
			printf("data at %d: %u\n", i, data[i]);
		}

		puts("\nTCS - read sequence done.\n");

		puts("Wait for 700 ms\n");
		usleep(700000);
	}


	/* Sample code */
	/*
	char regX[1] = {0x94}; //CDATAL (r0x14)
	write(file, regX, 1);
	char data[8] = {0};
	if (read(file, data, 8) != 8) { //Read 8 bytes
		// printf("Error : Input/output Error \n");
		print_error(4);
	} else
	*/
	return EXIT_SUCCESS;
}



