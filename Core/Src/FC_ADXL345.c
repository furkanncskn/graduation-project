#include "FC_ADXL345.h"
#include "FC_UTILITY.h"

#include <stdint.h>
#include <stdlib.h>

/*! Macros Declarations */

#define 	PUBLIC
#define 	PRIVATE 										static

#define   	ADXL345_I2C_ADRESS      	(0x53 << 1)                    // ADXL345 device address
#define   	ADXL345_WHO_AM_I					(0x00)               	       // a fixed device ID code of 0xE5
#define   	ADXL345_FIXED_VAL					(0xE5)
#define   	ADXL345_DATA_FORMAT     	(0x31)
#define   	ADXL345_POWER_CTL       	(0x2D)
#define   	ADXL345_DATAX0          	(0x32)


/*!< Typedef Declarations */

typedef struct ADXL345 {

	int16_t sensivity;
	int16_t rawX;
	int16_t rawY;
	int16_t rawZ;

}ADXL345;

typedef enum {

	ADXL345_2G_RANGE_SEN = 256,
	ADXL345_4G_RANGE_SEN = 128,
	ADXL345_8G_RANGE_SEN = 64,
	ADXL345_16G_RANGE_SEN = 32,

}ADXL345SensivityRange;

/*!< Static Function Declarations */

PRIVATE uint8_t ADXL345_Reset(I2C_HandleTypeDef* i2cHandle);
PRIVATE uint8_t ADXL345_SelectSensivityRange(I2C_HandleTypeDef* i2cHandle, ADXL345* adxl, uint8_t range);


/*!
 * @bried ADXL345 nesnesi hayata getirr
 *
 * return
 * 		  Hayata getirilen nesnenin adresi
 */
PUBLIC ADXL345* ADXL345_CreateObject(void) {
	ADXL345* adxl = (ADXL345*)malloc(sizeof(ADXL345));
	if (!adxl)
		return NULL;

	return adxl;
}

/*!
 * @bried ADXL345 nesnesinin hayatini sonlandirir
 *
 */
PUBLIC void ADXL345_DeleteObject(ADXL345* adxl) {
	if (!adxl)
		return;

	free(adxl);
}

/*!
 *  @brief  Sensorun varliginin kontrol eder.
 *
 *  @return
 *	    	Cihaz var ise, '0',
 *	    	Cihaz yok ise  '1'
 */
PUBLIC uint8_t ADXL345_IsWhoAmI(I2C_HandleTypeDef* i2cHandle) {
	uint8_t data = 0;
	if (I2C_Read(i2cHandle, ADXL345_I2C_ADRESS, ADXL345_WHO_AM_I, 1, &data))
		if (data == ADXL345_FIXED_VAL)
			return TRUE;

	return FALSE;
}

/*
 * @brief
 * 			Sensor kullanima hazir hale getirilir
 *
 * @return
 * 			Basarisizlik durumunda 	'1'
 * 			Basari durumunda 		'0'
 */
PUBLIC uint8_t ADXL345_Init(I2C_HandleTypeDef* i2cHandle, ADXL345* adxl, uint8_t range) {
	if (ADXL345_Reset(i2cHandle)) // reset all bits
		return FCFAIL;

	uint8_t val = 0x08; // measure and wake up 8hz
	if (I2C_Write(i2cHandle, ADXL345_I2C_ADRESS, ADXL345_POWER_CTL, &val) == FCFAIL)
		return FCFAIL;

	if (ADXL345_SelectSensivityRange(i2cHandle, adxl, range))
		return FCFAIL;

	return SUCCESS;
}

/*!
 *
 * @brief
 * 			Ham degerleri sensorden okur
 *
 * @return
 * 			Basari durumunda '0'
 * 			Basarisizlik durumunda '1'
 *
 */
PUBLIC uint8_t ADXL345_ReadRawValueFromAccel(I2C_HandleTypeDef* i2cHandle, ADXL345* adxl) {
	uint8_t data[6] = { 0 };

	HAL_I2C_Mem_Read(i2cHandle, ADXL345_I2C_ADRESS, ADXL345_DATAX0, 1, data, 6, 100);

	adxl->rawX = (int16_t)((data[0]) | (data[1] << 8));
	adxl->rawY = (int16_t)((data[2]) | (data[3] << 8));
	adxl->rawZ = (int16_t)((data[4]) | (data[5] << 8));

	return FCSUCCESS;
}

PUBLIC int16_t ADXL345_GetRawXValue(const ADXL345* adxl) {
	return adxl->rawX;
}

PUBLIC int16_t ADXL345_GetRawYValue(const ADXL345* adxl) {
	return adxl->rawY;
}

PUBLIC int16_t ADXL345_GetRawZValue(const ADXL345* adxl) {
	return adxl->rawZ;
}

PRIVATE uint8_t ADXL345_Reset(I2C_HandleTypeDef* i2cHandle) {
	if (I2C_Write(i2cHandle, ADXL345_I2C_ADRESS, ADXL345_POWER_CTL, 0x00))
		return FCFAIL;

	return FCSUCCESS;
}

PRIVATE uint8_t ADXL345_SelectSensivityRange(I2C_HandleTypeDef* i2cHandle, ADXL345* adxl, uint8_t range) {
	uint8_t val;
	switch (range) {
	case ADXL345_2G:
		val = 0x00;
		if (I2C_Write(i2cHandle, ADXL345_I2C_ADRESS, ADXL345_DATA_FORMAT, &val) == FCFAIL)
			return FCFAIL;

		adxl->sensivity = ADXL345_2G_RANGE_SEN;
		break;
	case ADXL345_4G:
		val = 0x01;
		if (I2C_Write(i2cHandle, ADXL345_I2C_ADRESS, ADXL345_DATA_FORMAT, &val) == FCFAIL)
			return FCFAIL;

		adxl->sensivity = ADXL345_4G_RANGE_SEN;
		break;
	case ADXL345_8G:
		val = 0x10;
		if (I2C_Write(i2cHandle, ADXL345_I2C_ADRESS, ADXL345_DATA_FORMAT, &val) == FCFAIL)
			return FCFAIL;

		adxl->sensivity = ADXL345_8G_RANGE_SEN;
		break;
	case ADXL345_16G:
		val = 0x11;
		if (I2C_Write(i2cHandle, ADXL345_I2C_ADRESS, ADXL345_DATA_FORMAT, &val) == FCFAIL)
			return FCFAIL;

		adxl->sensivity = ADXL345_16G_RANGE_SEN;
		break;
	default:
		val = 0x00;
		if (I2C_Write(i2cHandle, ADXL345_I2C_ADRESS, ADXL345_DATA_FORMAT, &val) == FCFAIL)
			return FCFAIL;

		adxl->sensivity = ADXL345_2G_RANGE_SEN;
		break;
	}

	return FCSUCCESS;
}
