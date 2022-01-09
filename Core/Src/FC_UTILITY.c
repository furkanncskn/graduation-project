#include "FC_UTILITY.h"

#include <stdint.h>

/*!
 *  @brief  Istenen I2C hattindan okuma gerceklestirir
 *
 *  @return
 *  		Basarizlik durumunda, NULL
 *  		Basari durumunda, okunan adresteki değer
 *
 */
uint8_t* I2C_Read(I2C_HandleTypeDef* i2cHandle, uint8_t device_adress, uint8_t memory_adress, uint8_t number_of_bytes, uint8_t* data) {
	if (HAL_I2C_Mem_Read(i2cHandle, device_adress, memory_adress, 1, data, number_of_bytes, 1000) != HAL_OK)
		return NULL;

	return data;
}


/*!
 * 	@brief 		Istenen I2C hattindan yazma gerceklestirir
 *
 * 	@return
 * 				Basarisizlik durumunda '1'
 * 				Basari durumunda '0'
 */
uint8_t I2C_Write(I2C_HandleTypeDef* i2cHandle, uint8_t device_adress, uint8_t memory_adress, uint8_t* data) {
	if (HAL_I2C_Mem_Write(i2cHandle, device_adress, memory_adress, 1, data, 1, HAL_MAX_DELAY) != HAL_OK)
		return FCFAIL;

	return FCSUCCESS;
}


/*!
 *  @brief  Istenen i2c noktasında, bağlı cihaz var mı ?
 *
 *  @return
 *	    	cihaz bağlı ise cihazın adresi
 *	    	cihaz bağlı değil ise sıfır
 */
uint8_t ScanI2CReady(I2C_HandleTypeDef* i2cHandle) {
	uint8_t i;
	for (i = 0; i < 255; ++i)
		if (HAL_I2C_IsDeviceReady(i2cHandle, i, 1, 100) == HAL_OK)
			break;

	if (i == 255)
		return FCFAIL;

	return i;
}