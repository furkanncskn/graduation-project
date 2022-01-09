#ifndef FC_ADXL345_H
#define FC_ADXL345_H

#include "stm32f1xx_hal.h"

#include <stdint.h>

#define EARTH_GRAVITY       (9.800665F)


/*
 * ADXL345 nesnesi test asamasi boyunca baslik dosyasinda kalacak,
 * gereksiz structer elemanları test calismalari bittikten sorna cikartilacak
 *
 **/
typedef struct ADXL345 {

	double accVal[3];
	double accOffset[3];
	double accPrevious[3];
	double FIRfilterAcc[3];
	double IIRfilterAcc[3];
	double accRecent[3];
	double accVelocity[3];
	double accSumVelocity[3];
	double accPreviousVelocity[3];
	double accDiff[3];
	int16_t accRaw[3];
	int16_t sensivity;

}ADXL345;


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

void ADXL345_SetOffsetValues(I2C_HandleTypeDef* i2cHandle, ADXL345* adxl);

void ADXL345_SetAccelerations(I2C_HandleTypeDef* i2cHandle, ADXL345* adxl);

void ADXL345_FIRAvarageFilter(I2C_HandleTypeDef* i2cHandle, ADXL345* adxl, size_t FIRfilter_size);

void ADXL345_IIRLowPassFilter(ADXL345* adxl, double filter_rate);

void ADXL345_IsTheMovementStoped(ADXL345* adxl);

void ADXL345_SetVelocity(ADXL345* adxl, double deltaT);



int16_t ADXL345_GetRawXValue(const ADXL345* adxl);

int16_t ADXL345_GetRawYValue(const ADXL345* adxl);

int16_t ADXL345_GetRawZValue(const ADXL345* adxl);

double ADXL345_GetAccelerationX(const ADXL345* adxl);

double ADXL345_GetAccelerationY(const ADXL345* adxl);

double ADXL345_GetAccelerationZ(const ADXL345* adxl);

double ADXL345_GetFIRFilterAccelerationX(const ADXL345* adxl);

double ADXL345_GetFIRFilterAccelerationY(const ADXL345* adxl);

double ADXL345_GetFIRFilterAccelerationZ(const ADXL345* adxl);

double ADXL345_GetIIRFilterAccelerationX(const ADXL345* adxl);

double ADXL345_GetIIRFilterAccelerationY(const ADXL345* adxl);

double ADXL345_GetIIRFilterAccelerationZ(const ADXL345* adxl);

double ADXL345_GetVelocityX(const ADXL345* adxl);

double ADXL345_GetVelocityY(const ADXL345* adxl);

double ADXL345_GetVelocityZ(const ADXL345* adxl);

#endif // FC_ADXL345_H
