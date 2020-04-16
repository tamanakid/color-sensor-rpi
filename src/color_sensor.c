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

	int fd_i2c = setup();



	while(1) {
		uint8_t read_ptr_reg = 0x00;
		uint8_t data_byte;
		uint8_t data[8] = {0};

		// Since the integration time is 700ms, we are to wait that amount of time
		puts("\nWait for 700 ms\n");
		usleep(700000);

		puts("TCS - read sequence begun.\n");
		read_ptr_reg = TCS_CMD_BYTE | TCS_REG_DATA_C_LOW; // TCS_CMD_AUTOINC | TCS_REG_DATA_C_LOW;

		int i;
		for (i = 0; i < 8; i++) {
			i2c_write(fd_i2c, &read_ptr_reg, 1);

			data_byte = i2c_read(fd_i2c);
			if (data_byte != -1) {
				data[i] = data_byte;
			}

			read_ptr_reg++;
		}
		puts("TCS - read sequence done:\n");

		/*
		for (i = 0; i < 8; i++) {
			printf("data at %d: %u\n", i, data[i]);
		}
		*/

		uint16_t conversions_16[4];
		float conversions[4];

		conversions_16[0] = (uint16_t) (data[1] << 8 | data[0]);
		conversions_16[1] = (uint16_t) (data[3] << 8 | data[2]);
		conversions_16[2] = (uint16_t) (data[5] << 8 | data[4]);
		conversions_16[3] = (uint16_t) (data[7] << 8 | data[6]);

		conversions[0] = (float) conversions_16[0] / 655.35;
		conversions[1] = (float) conversions_16[1] / 655.35;
		conversions[2] = (float) conversions_16[2] / 655.35;
		conversions[3] = (float) conversions_16[3] / 655.35;

		printf("Clarity: %.2f %% \n", conversions[0]);
		printf("Red: %.2f %% \n", conversions[1]);
		printf("Green: %.2f %% \n", conversions[2]);
		printf("Blue: %.2f %% \n", conversions[3]);

		if ((conversions[1] > conversions[2]*1.5) && (conversions[1] > conversions[3]*1.5)) {
			printf("Predominant color: RED \n");
		} else if ((conversions[2] > conversions[1]*1.5) && (conversions[2] > conversions[3]*1.5)) {
			printf("Predominant color: GREEN \n");
		} else if ((conversions[3] > conversions[2]*1.5) && (conversions[3] > conversions[1]*1.5)) {
			printf("Predominant color: BLUE \n");
		} else {
			printf("No clear predominant color.\n");
		}
	}


	return EXIT_SUCCESS;
}


int setup() {
	int fd_i2c;

	/* Step 1: Create instance of I2C interface's descriptor */
	fd_i2c = get_i2c_descriptor();


	/* Step 2: Write to ATIME Register (0x01) to set Integration Time (700ms - 0x00) */
	puts("Writing ATIME register\n");
	write_config_byte(fd_i2c, TCS_REG_ATIME, 0x00);


	/* Step 3: Write to WTIME Register (0x03) to set Waiting Time (2.4ms - 0xFF) */
	puts("Writing WTIME register\n");
	write_config_byte(fd_i2c, TCS_REG_WTIME, 0xFF);


	/* Step 4: Write to AGAIN bits of Control Register (0x0F) to set AGAIN Value (x1 Gain RGBC Control value: 0x00) */
	puts("Writing Control register\n");
	write_config_byte(fd_i2c, TCS_REG_CONTROL, 0x00);


	/* Step 5: Write to Enable Register (0x00) to set PON bit: sets sensor to "idle" state */
	puts("Writing Enable register: PON bits \n");
	write_config_byte(fd_i2c, TCS_REG_ENABLE, 0x01);

	/* There is a 2.4 ms warm-up delay when PON is enabled. */
	puts("Wait for 2.4 ms\n");
	usleep(2400);


	/* Step 6: Write to Enable Register (0x00) to set AEN bit (without WEN bit) to start sensor cycles */
	puts("Writing Enable register\n");
	write_config_byte(fd_i2c, TCS_REG_ENABLE, 0x03);
	/* NOTE: to set PON, AEN and WEN bits: 0, 1 and 3 respectively (0x0C) */


	return fd_i2c;
}




