#ifndef FC_UTILITY_H
#define FC_UTILITY_H

#include "stm32f1xx_hal.h"

#include <stdint.h>

typedef enum {

  FCSUCCESS = 0U,
  FCFAIL = !SUCCESS,

} FCErrorStatus;

typedef enum {

  TRUE = 0U,
  FALSE = !TRUE

}FCStatus;

uint8_t* I2C_Read(I2C_HandleTypeDef* i2cHandle, uint8_t device_adress, uint8_t memory_adress, uint8_t number_of_bytes, uint8_t* data);

uint8_t I2C_Write(I2C_HandleTypeDef* i2cHandle, uint8_t device_adress, uint8_t memory_adress, uint8_t* data);

uint8_t ScanI2CReady(I2C_HandleTypeDef* i2cHandle);

#endif // FC_UTILITY_H
