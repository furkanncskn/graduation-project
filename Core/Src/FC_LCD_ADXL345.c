#include "FC_LCD_ADXL345.h"

#include "stm32f1xx_hal.h"

#include <stdio.h>
#include <string.h>


#define IS_GREATER_THAN_1_SECONDS(time_diff) 		(!((time_diff) < 1000))


void printToLcdDisplay(I2C_HandleTypeDef * i2cHandle, ADXL345* adxl)
{
  lcd_clear(i2cHandle);

  double temp_value;
  char _lcd_buffer[20];
  int isNegativeOrPositive = '+';

  /* Birinci satir, x ekseni ivme degeri */
  memset(_lcd_buffer,0,20);
  temp_value = ADXL345_GetRecentAccelValueX(adxl);
  if(temp_value < 0) isNegativeOrPositive = '-', temp_value *= -1;
  sprintf(_lcd_buffer,"IVME X: %c%.2f m/s^2",isNegativeOrPositive, temp_value );
  lcd_send_cmd (i2cHandle,0X80|0X00);
  lcd_send_string(i2cHandle,_lcd_buffer);

  /* İkinci satir, y ekseni ivme degeri */
  memset(_lcd_buffer,0,20);
  isNegativeOrPositive = '+';
  temp_value = ADXL345_GetRecentAccelValueY(adxl);
  if(temp_value  < 0) isNegativeOrPositive = '-', temp_value  *= -1;
  sprintf(_lcd_buffer,"IVME Y: %c%.2f m/s^2",isNegativeOrPositive,temp_value );
  lcd_send_cmd (i2cHandle,0X80|0X40);
  lcd_send_string(i2cHandle,_lcd_buffer);

  /* Ucuncu satir, x ekseni hiz degeri */
  memset(_lcd_buffer,0,20);
  isNegativeOrPositive = '+';
  temp_value = ADXL345_GetVelocityX(adxl);
  if(temp_value  < 0) isNegativeOrPositive = '-', temp_value  *= -1;
  sprintf(_lcd_buffer,"HIZ  X: %c%.2f m/s",isNegativeOrPositive, temp_value );
  lcd_send_cmd (i2cHandle,0X80|0X14);
  lcd_send_string(i2cHandle, _lcd_buffer);

  /* Dorduncu saitir, y ekseni hiz degeri */
  memset(_lcd_buffer,0,20);
  isNegativeOrPositive = '+';
  temp_value = ADXL345_GetVelocityY(adxl);
  if(temp_value  < 0) isNegativeOrPositive = '-', temp_value  *= -1;
  sprintf(_lcd_buffer,"HIZ  Y: %c%.2f m/s", isNegativeOrPositive, temp_value);
  lcd_send_cmd (i2cHandle, 0X80|0X54);
  lcd_send_string(i2cHandle, _lcd_buffer);
}


void printToLCDEverySecond(I2C_HandleTypeDef* i2cHande, ADXL345* adxl)
{
	static uint32_t lcd_previous_time = 0;

    /* Yaklaşık bir saniyede bir LCD ekrana yazi yazdir */
    uint32_t current_time = HAL_GetTick();
    if(IS_GREATER_THAN_1_SECONDS(current_time - lcd_previous_time)) {
    	lcd_previous_time = current_time;
    	printToLcdDisplay(i2cHande, adxl);
    }
}
