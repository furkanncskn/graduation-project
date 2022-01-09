#include "main.h"

#include "FC_ADXL345.h"
#include "FC_UTILITY.h"

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;


/*---------------- Makro Tanımlamaları -----------------*/

#define FIR_FILTER_SIZE		(64)
#define	IIR_FILTER_RATE		(0.90)


/*--------------- Kullanılan Değişkenler ---------------*/

int16_t _accRaw[3];
double 	_accVal[2];
double  _FIRfilterAcc[3];
double  _IIRfilterAcc[3];
double  _accRecent[3];
double  _accVelocity[3];

uint32_t _previous_time;
double   _period;

uint8_t test;


void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);

int main(void) {

  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();

  // -------------- Asagida ki if kontrolleri debug amacli yazilmistir -------------- //


  /*!< ADXL345 sensoru var mi yok mu ? */
  if (ADXL345_IsWhoAmI(&hi2c1) == TRUE) test = FCSUCCESS;
  else test = FCFAIL;

  /*!< ADXL345 nesnem basarili bir sekilde olusturuldu mu ? */
  ADXL345* _ADXL345 = ADXL345_CreateObject();
  if (_ADXL345 != NULL) test = FCSUCCESS;
  else test = FCFAIL;

  /*!< ADXL345 sensoru kullanmaya hazir hale getirildi mi ? */
  if (ADXL345_Init(&hi2c1, _ADXL345, ADXL345_2G) != FCFAIL) test = FCSUCCESS;
  else test = FCFAIL;

  /*!< ADXL345 kalibrasyonu yapilir */
  ADXL345_SetOffsetValues(&hi2c1, _ADXL345);


  // ---------------------------------------------------------------------------- //


  while (1) {

    /*!< Registerlandan ham degerleri oku */
    ADXL345_ReadRawValueFromAccel(&hi2c1, _ADXL345);


    /*!< Okunan ham degerleri degiskenlere al */
    _accRaw[0] = ADXL345_GetRawXValue(_ADXL345);
    _accRaw[1] = ADXL345_GetRawYValue(_ADXL345);
    _accRaw[2] = ADXL345_GetRawZValue(_ADXL345);


    /*!< İvme değerlerini oku */
    ADXL345_SetAccelerations(&hi2c1, _ADXL345);


    /*!< İvme değerlerini değişkenlere al */
    _accVal[0] = ADXL345_GetAccelerationX(_ADXL345);
    _accVal[1] = ADXL345_GetAccelerationY(_ADXL345);
    _accVal[2] = ADXL345_GetAccelerationZ(_ADXL345);


    /*!< FIR filtreyi çalıştır */
    ADXL345_FIRAvarageFilter(&hi2c1, _ADXL345, FIR_FILTER_SIZE);


    /*!< FIR filtreden çıkan değerleri değişkenlere al */
    _FIRfilterAcc[0] = ADXL345_GetFIRFilterAccelerationX(_ADXL345);
    _FIRfilterAcc[1] = ADXL345_GetFIRFilterAccelerationY(_ADXL345);
    _FIRfilterAcc[2] = ADXL345_GetFIRFilterAccelerationZ(_ADXL345);


    /*!< IIR filtreyi calistir */
    ADXL345_IIRLowPassFilter(_ADXL345, IIR_FILTER_RATE);


    /*!< Filtrelenmiş değerleri değişkenlere al */
    _IIRfilterAcc[0] = ADXL345_GetIIRFilterAccelerationX(_ADXL345);
    _IIRfilterAcc[1] = ADXL345_GetIIRFilterAccelerationY(_ADXL345);
    _IIRfilterAcc[2] = ADXL345_GetIIRFilterAccelerationZ(_ADXL345);

    _accRecent[0] = _ADXL345->accRecent[0];
    _accRecent[1] = _ADXL345->accRecent[1];
    _accRecent[2] = _ADXL345->accRecent[2];


    /*!< Periyodu, zaman farkını hesapla */
    uint32_t _current_time = HAL_GetTick();
    _period = (_current_time - _previous_time);


    /* Hizi hesapla ve degerleri degiskenlere al */
    if (_period >= 10) {
      ADXL345_SetVelocity(_ADXL345, _period);
      _previous_time = _current_time;
      for (int i = 0; i < 3; ++i) {
        _accVelocity[i] = _ADXL345->accVelocity[i];
      }
    }

  } // while


  /*!< ADXL345 nesnesinin hayatini sonlandir */
  ADXL345_DeleteObject(_ADXL345);


  return 0;

} // main




void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
  RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
    | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    Error_Handler();
  }
}

static void MX_I2C1_Init(void) {
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
    Error_Handler();
  }
}

static void MX_I2C2_Init(void) {
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK) {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = { 0 };

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);
}

void Error_Handler(void) {
  __disable_irq();
  while (1) {}
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line) {}

#endif /* USE_FULL_ASSERT */
