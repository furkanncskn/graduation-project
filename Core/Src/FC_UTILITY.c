#include "FC_UTILITY.h"

#include <stdint.h>

#define PUBLIC
#define PRIVATE			static

/*!
 *  @brief  Istenen I2C hattindan okuma gerceklestirir
 *
 *	@param i2cHandle, i2c hattının adresi
 *	@param device_address, cihazın adresi
 *	@param memory_address, okunacak register adresi
 *	@param number_of_bytes, okuma yapilacak byte sayisi
 *	@param data, okunacak verinin tutulacagi dizi
 *
 *  @return
 *  		Basarizlik durumunda, NULL
 *  		Basari durumunda, okunan adresteki değer
 *
 */
PUBLIC uint8_t* I2C_Read(I2C_HandleTypeDef* i2cHandle, uint8_t device_address, uint8_t memory_address, uint8_t number_of_bytes, uint8_t* data) {
	if (HAL_I2C_Mem_Read(i2cHandle, device_address, memory_address, 1, data, number_of_bytes, 1000) != HAL_OK)
		return NULL;

	return data;
}


/*!
 * 	@brief 		Istenen I2C hattindan yazma gerceklestirir
 *
 * 	@param i2cHandle, i2c hattının adresi
 *	@param device_address, cihazın adresi
 *	@param memory_address, yazilacak register adresi
 *  @param data, yazilacak deger
 *
 *  @return
 * 				Basarisizlik durumunda '1'
 * 				Basari durumunda '0'
 */
PUBLIC uint8_t I2C_Write(I2C_HandleTypeDef* i2cHandle, uint8_t device_address, uint8_t memory_address, uint8_t* data) {
	if (HAL_I2C_Mem_Write(i2cHandle, device_address, memory_address, 1, data, 1, HAL_MAX_DELAY) != HAL_OK)
		return FCFAIL;

	return FCSUCCESS;
}


/*!
 *  @brief  Istenen i2c noktasında, bağlı cihaz var mı ?
 *
 *  @param i2cHandle, i2c hattının adresi
 *
 *  @return
 *	    	cihaz bağlı ise cihazın adresi
 *	    	cihaz bağlı değil ise sıfır
 */
PUBLIC uint8_t ScanI2CReady(I2C_HandleTypeDef* i2cHandle) {
	uint8_t i;
	for (i = 0; i < 255; ++i)
		if (HAL_I2C_IsDeviceReady(i2cHandle, i, 1, 100) == HAL_OK)
			break;

	if (i == 255)
		return FCFAIL;

	return i;
}
