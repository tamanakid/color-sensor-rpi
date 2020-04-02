/*
 ============================================================================
 Name        : color_sensor.h
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



#define I2C_INTERFACE			"/dev/i2c-1"
#define TCS34725_ADDR			0x29

#define TCS_CMD_BYTE			0x80
#define TCS_CMD_AUTOINC			0xa0
#define TCS_REG_ENABLE			0x00
#define TCS_REG_ATIME			0x01
#define TCS_REG_WTIME			0x03
#define TCS_REG_CONTROL			0x0F
#define TCS_REG_DATA_C_LOW		0x14



/* Error Control */
void print_error(int error_code);


/* Color Sensor Interfacing */
int get_i2c_descriptor();

void write_cmd_byte(int fd_i2c, uint8_t reg, uint8_t byte);

void i2c_write(int fd_i2c, uint8_t* registers, int length);



