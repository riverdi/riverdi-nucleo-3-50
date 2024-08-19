/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : STM32TouchController.cpp
  ******************************************************************************
  * This file was created by TouchGFX Generator 4.23.2. This file is only
  * generated once! Delete this file from your project and re-generate code
  * using STM32CubeMX or change this file manually to update it.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* USER CODE BEGIN STM32TouchController */

#include <STM32TouchController.hpp>
#include <main.h>

extern I2C_HandleTypeDef hi2c2;
extern volatile uint8_t touch_irq;

void STM32TouchController::init()
{
    /**
     * Initialize touch controller and driver
     *
     */
	HAL_GPIO_WritePin(CTP_RST_GPIO_Port, CTP_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(CTP_RST_GPIO_Port, CTP_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(CTP_RST_GPIO_Port, CTP_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
}

bool STM32TouchController::sampleTouch(int32_t& x, int32_t& y)
{
    /**
     * By default sampleTouch returns false,
     * return true if a touch has been detected, otherwise false.
     *
     * Coordinates are passed to the caller by reference by x and y.
     *
     * This function is called by the TouchGFX framework.
     * By default sampleTouch is called every tick, this can be adjusted by HAL::setTouchSampleRate(int8_t);
     *
     */
	if (touch_irq == 1) {

		HAL_StatusTypeDef status;
		uint8_t rx_buf[16] = {0};

		touch_irq = 0;

		/* read x/y coordinates */
		status = HAL_I2C_Mem_Read (&hi2c2, (0x41 << 1), 0x10, 1, rx_buf, sizeof(rx_buf), 100);
		if (status == HAL_OK) {
			*(int32_t*)&x = ((rx_buf[3] & 0x0F) << 8 | rx_buf[2]);
			*(int32_t*)&y = ((rx_buf[5] & 0x0F) << 8 | rx_buf[4]);
			return true;
	    } else {
            return false;
	    }
	}

	return false;
}

/* USER CODE END STM32TouchController */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
