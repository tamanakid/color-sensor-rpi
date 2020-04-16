/*
 * sensor_interface.c
 *
 *  Created on: Apr 1, 2020
 *      Author: Nicolas Villanueva
 */

#include "color_sensor.h"


int get_i2c_descriptor() {
	int fd_i2c;

	/* Open descriptor to i2c bus' device file */
	fd_i2c = open(I2C_INTERFACE, O_RDWR);

	if (fd_i2c < 0)
		print_error(1);

	/* Specify descriptor as a slave to the sensor's address */
	if (ioctl(fd_i2c, I2C_SLAVE, TCS34725_ADDR) < 0)
		print_error(2);

	// Since PON is not enabled, the device will return to the Sleep state.

	return fd_i2c;
}



void write_config_byte(int fd_i2c, uint8_t reg, uint8_t byte) {
	uint8_t registers[2];

	registers[0] = TCS_CMD_BYTE | reg;
	registers[1] = byte;

	i2c_write(fd_i2c, registers, 2);
}



void i2c_write(int fd_i2c, uint8_t* registers, int length) {

	int sent_bytes = write(fd_i2c, registers, length);

	if (sent_bytes < 0)
		print_error(3);
	else if (sent_bytes != length)
		print_error(4);

}

int i2c_read(int fd_i2c) {
	uint8_t data_byte;

	int read_bytes = read(fd_i2c, &data_byte, 1);
	if (read_bytes != 1) {
		print_error(5);
		return -1;
	} else {
		return data_byte;
	}
}

