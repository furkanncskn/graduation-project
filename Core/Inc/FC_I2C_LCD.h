#include "stm32f1xx_hal.h"

void lcd_init (I2C_HandleTypeDef* i2cHandle);   // initialize lcd

void lcd_send_cmd (I2C_HandleTypeDef* i2cHandle, char cmd);  // send command to the lcd

void lcd_send_data (I2C_HandleTypeDef* i2cHandle, char data);  // send data to the lcd

void lcd_send_string (I2C_HandleTypeDef* i2cHandle, char *str);  // send string to the lcd

void lcd_clear (I2C_HandleTypeDef* i2cHandle);
