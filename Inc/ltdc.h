/**
  ******************************************************************************
  * File Name          : LTDC.h
  * Description        : This file provides code for the configuration
  *                      of the LTDC instances.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ltdc_H
#define __ltdc_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include <string.h>
/* USER CODE END Includes */

extern LTDC_HandleTypeDef hltdc;

/* USER CODE BEGIN Private defines */
#define swap(a,b) {int16_t t=a; a=b; b=t;}//поменяли местами значения двух переменных
#define convert24to32(x) (x|0xFF000000)//приведение к формату ARGB8888
/* USER CODE END Private defines */

void MX_LTDC_Init(void);

/* USER CODE BEGIN Prototypes */
void TFT_DrawRawPicture(uint32_t Xpos, uint32_t Ypos, uint32_t width, uint32_t height, uint8_t *pbmp, uint8_t layer);
void TFT_DisplayString(uint16_t X, uint16_t Y, uint8_t* Text, Text_AlignModeTypdef Mode);
void TFT_DrawChar(uint16_t x,uint16_t y,const uint8_t c);
void TFT_SetColor (uint32_t color);
void TFT_SetBackColor(uint32_t color);
void TFT_SetFont(sFONT* font);
void TFT_FontInit(void);
void TFT_DrawBitmap(uint32_t Xpos, uint32_t Ypos, uint8_t *pbmp);
 void TFT_FillScreen(uint32_t color, uint8_t layer);
void TFT_FillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color, uint8_t layer);
	 void TFT_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t color, uint8_t layer);
	 void TFT_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color, uint8_t layer);
	 void TFT_DrawRectangle(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom, uint32_t color, uint8_t layer);
	 void TFT_DrawCircle(uint16_t x0, uint16_t y0, uint16_t radius, uint32_t color, uint8_t layer);
	 void TFT_FillCircle(uint16_t x, uint16_t y, uint16_t radius, uint32_t color, uint8_t layer);
void BitmapToMemory(uint32_t Xpos, uint32_t Ypos, uint8_t *pbmp, uint8_t* memaddr);
void DMA2D_LayersAlphaReconfig(uint8_t alpha1, uint8_t alpha2);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ ltdc_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
