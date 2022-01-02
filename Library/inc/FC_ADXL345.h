#ifndef FC_ADXL345_H
#define FC_ADXL345_H

#include "stm32f1xx_hal.h"

#include <stdint.h>

/*! Typedef Definitions */

struct ADXL345;
typedef struct ADXL345 ADXL345;

typedef enum {

	ADXL345_2G = 2,
	ADXL345_4G = 4,
	ADXL345_8G = 8,
	ADXL345_16G = 16,

}ADXL345Sensivity;

/*! Function Definitions */

ADXL345* ADXL345_CreateObject(void);

void ADXL345_DeleteObject(ADXL345* adxl);

uint8_t ADXL345_IsWhoAmI(I2C_HandleTypeDef*);

uint8_t ADXL345_Init(I2C_HandleTypeDef* i2cHandle, ADXL345* adxl, uint8_t range);

uint8_t ADXL345_ReadRawValueFromAccel(I2C_HandleTypeDef* i2cHandle, ADXL345* adxl);

int16_t ADXL345_GetRawXValue(const ADXL345* adxl);

int16_t ADXL345_GetRawYValue(const ADXL345* adxl);

int16_t ADXL345_GetRawZValue(const ADXL345* adxl);

#endif // FC_ADXL345_H
