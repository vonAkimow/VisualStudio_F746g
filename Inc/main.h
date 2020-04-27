/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "MT48LC4M32B2B5.h"
#include "fonts.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct //структура настройки вывода текста (цвет символа, фон за символом, шрифт)
{
  uint32_t TextColor;
  uint32_t BackColor;
  sFONT *pFont;
}TFT_DrawPropTypeDef;

typedef enum
{
  CENTER_MODE = 0x01, /* Center mode */
  RIGHT_MODE = 0x02, /* Right mode */
  LEFT_MODE = 0x03 /* Left mode */
}Text_AlignModeTypdef;

	
	
	
	typedef struct {
	
		unsigned 		width;
		unsigned 		height;
		unsigned 		error;
		size_t 			size;
		unsigned char* storage; /* Указатель на буфер, где будет храниться декодированная картинка */		
		char ErrorStr[15];
	
}PNG_Properties;	
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */


/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_BL_CTRL_Pin GPIO_PIN_3
#define LCD_BL_CTRL_GPIO_Port GPIOK
#define GLED_PIN_Pin GPIO_PIN_1
#define GLED_PIN_GPIO_Port GPIOI
#define LCD_DISP_Pin GPIO_PIN_12
#define LCD_DISP_GPIO_Port GPIOI
/* USER CODE BEGIN Private defines */
#define LCD_FRAME_BUFFER_1   SDRAM_BANK_ADDR //0xC0000000(начальный адрес рамки)
#define convert24to32(x) (x|0xFF000000)//3 байта RGB888 -> 4 байта ARGB8888 с MSB - прозрачностью

//ARGB8888 цвета
#define LCD_COLOR_BLUE ((uint32_t)0xFF0000FF)
#define LCD_COLOR_GREEN ((uint32_t)0xFF00FF00)
#define LCD_COLOR_RED ((uint32_t)0xFFFF0000)
#define LCD_COLOR_CYAN ((uint32_t)0xFF00FFFF)
#define LCD_COLOR_MAGENTA ((uint32_t)0xFFFF00FF)
#define LCD_COLOR_YELLOW ((uint32_t)0xFFFFFF00)
#define LCD_COLOR_LIGHTBLUE ((uint32_t)0xFF8080FF)
#define LCD_COLOR_LIGHTGREEN ((uint32_t)0xFF80FF80)
#define LCD_COLOR_LIGHTRED ((uint32_t)0xFFFF8080)
#define LCD_COLOR_LIGHTCYAN ((uint32_t)0xFF80FFFF)
#define LCD_COLOR_LIGHTMAGENTA ((uint32_t)0xFFFF80FF)
#define LCD_COLOR_LIGHTYELLOW ((uint32_t)0xFFFFFF80)
#define LCD_COLOR_DARKBLUE ((uint32_t)0xFF000080)
#define LCD_COLOR_DARKGREEN ((uint32_t)0xFF008000)
#define LCD_COLOR_DARKRED ((uint32_t)0xFF800000)
#define LCD_COLOR_DARKCYAN ((uint32_t)0xFF008080)
#define LCD_COLOR_DARKMAGENTA ((uint32_t)0xFF800080)
#define LCD_COLOR_DARKYELLOW ((uint32_t)0xFF808000)
#define LCD_COLOR_WHITE ((uint32_t)0xFFFFFFFF)
#define LCD_COLOR_LIGHTGRAY ((uint32_t)0xFFD3D3D3)
#define LCD_COLOR_GRAY ((uint32_t)0xFF808080)
#define LCD_COLOR_DARKGRAY ((uint32_t)0xFF404040)
#define LCD_COLOR_BLACK ((uint32_t)0xFF000000)
#define LCD_COLOR_BROWN ((uint32_t)0xFFA52A2A)
#define LCD_COLOR_ORANGE ((uint32_t)0xFFFFA500)
#define LCD_COLOR_TRANSPARENT ((uint32_t)0xFF000000)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
