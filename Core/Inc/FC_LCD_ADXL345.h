#ifndef FC_LCD_ADXL345_H
#define FC_LCD_ADXL345_H

#include "FC_ADXL345.h"
#include "FC_I2C_LCD.h"

/*----------------------- Fonksiyon Bildirimleri -----------------------*/

void printToLcdDisplay(I2C_HandleTypeDef * i2cHandle, ADXL345* adxl);

void printToLCDEverySecond(I2C_HandleTypeDef* i2cHande, ADXL345* adxl);


#endif
