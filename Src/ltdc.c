/**
  ******************************************************************************
  * File Name          : LTDC.c
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

/* Includes ------------------------------------------------------------------*/
#include "ltdc.h"

/* USER CODE BEGIN 0 */
const uint16_t X_SIZE = 480;
const uint16_t Y_SIZE = 272;
TFT_DrawPropTypeDef TFT; //объявляем переменную - экземпляр структуры TFT_DrawPropTypeDef 
/* #define BACKCOLOR если определено, то используем задний фон для символов, которые выводятся на экран */

/* USER CODE END 0 */

LTDC_HandleTypeDef hltdc;

/* LTDC init function */
void MX_LTDC_Init(void)
{
  LTDC_LayerCfgTypeDef pLayerCfg = {0};
  LTDC_LayerCfgTypeDef pLayerCfg1 = {0};

  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Init.HorizontalSync = 40;
  hltdc.Init.VerticalSync = 9;
  hltdc.Init.AccumulatedHBP = 53;
  hltdc.Init.AccumulatedVBP = 11;
  hltdc.Init.AccumulatedActiveW = 533;
  hltdc.Init.AccumulatedActiveH = 283;
  hltdc.Init.TotalWidth = 565;
  hltdc.Init.TotalHeigh = 285;
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 480;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 272;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  pLayerCfg.Alpha = 255;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  pLayerCfg.FBStartAdress = 0xC0200000;
  pLayerCfg.ImageWidth = 480;
  pLayerCfg.ImageHeight = 272;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg1.WindowX0 = 0;
  pLayerCfg1.WindowX1 = 480;
  pLayerCfg1.WindowY0 = 0;
  pLayerCfg1.WindowY1 = 272;
  pLayerCfg1.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  pLayerCfg1.Alpha = 255;
  pLayerCfg1.Alpha0 = 0;
  pLayerCfg1.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg1.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  pLayerCfg1.FBStartAdress = 0xC0000000;
  pLayerCfg1.ImageWidth = 480;
  pLayerCfg1.ImageHeight = 272;
  pLayerCfg1.Backcolor.Blue = 0;
  pLayerCfg1.Backcolor.Green = 0;
  pLayerCfg1.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg1, 1) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_LTDC_MspInit(LTDC_HandleTypeDef* ltdcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(ltdcHandle->Instance==LTDC)
  {
  /* USER CODE BEGIN LTDC_MspInit 0 */

  /* USER CODE END LTDC_MspInit 0 */
    /* LTDC clock enable */
    __HAL_RCC_LTDC_CLK_ENABLE();
  
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOJ_CLK_ENABLE();
    __HAL_RCC_GPIOK_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();
    /**LTDC GPIO Configuration    
    PE4     ------> LTDC_B0
    PJ13     ------> LTDC_B1
    PK7     ------> LTDC_DE
    PK6     ------> LTDC_B7
    PK5     ------> LTDC_B6
    PG12     ------> LTDC_B4
    PJ14     ------> LTDC_B2
    PI10     ------> LTDC_HSYNC
    PK4     ------> LTDC_B5
    PJ15     ------> LTDC_B3
    PI9     ------> LTDC_VSYNC
    PK1     ------> LTDC_G6
    PK2     ------> LTDC_G7
    PI15     ------> LTDC_R0
    PJ11     ------> LTDC_G4
    PK0     ------> LTDC_G5
    PI14     ------> LTDC_CLK
    PJ8     ------> LTDC_G1
    PJ10     ------> LTDC_G3
    PJ7     ------> LTDC_G0
    PJ9     ------> LTDC_G2
    PJ6     ------> LTDC_R7
    PJ4     ------> LTDC_R5
    PJ5     ------> LTDC_R6
    PJ3     ------> LTDC_R4
    PJ2     ------> LTDC_R3
    PJ0     ------> LTDC_R1
    PJ1     ------> LTDC_R2 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_11 
                          |GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_7|GPIO_PIN_9 
                          |GPIO_PIN_6|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_3 
                          |GPIO_PIN_2|GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4 
                          |GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_LTDC;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_9|GPIO_PIN_15|GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /* USER CODE BEGIN LTDC_MspInit 1 */

  /* USER CODE END LTDC_MspInit 1 */
  }
}

void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef* ltdcHandle)
{

  if(ltdcHandle->Instance==LTDC)
  {
  /* USER CODE BEGIN LTDC_MspDeInit 0 */

  /* USER CODE END LTDC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_LTDC_CLK_DISABLE();
  
    /**LTDC GPIO Configuration    
    PE4     ------> LTDC_B0
    PJ13     ------> LTDC_B1
    PK7     ------> LTDC_DE
    PK6     ------> LTDC_B7
    PK5     ------> LTDC_B6
    PG12     ------> LTDC_B4
    PJ14     ------> LTDC_B2
    PI10     ------> LTDC_HSYNC
    PK4     ------> LTDC_B5
    PJ15     ------> LTDC_B3
    PI9     ------> LTDC_VSYNC
    PK1     ------> LTDC_G6
    PK2     ------> LTDC_G7
    PI15     ------> LTDC_R0
    PJ11     ------> LTDC_G4
    PK0     ------> LTDC_G5
    PI14     ------> LTDC_CLK
    PJ8     ------> LTDC_G1
    PJ10     ------> LTDC_G3
    PJ7     ------> LTDC_G0
    PJ9     ------> LTDC_G2
    PJ6     ------> LTDC_R7
    PJ4     ------> LTDC_R5
    PJ5     ------> LTDC_R6
    PJ3     ------> LTDC_R4
    PJ2     ------> LTDC_R3
    PJ0     ------> LTDC_R1
    PJ1     ------> LTDC_R2 
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_4);

    HAL_GPIO_DeInit(GPIOJ, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_11 
                          |GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_7|GPIO_PIN_9 
                          |GPIO_PIN_6|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_3 
                          |GPIO_PIN_2|GPIO_PIN_0|GPIO_PIN_1);

    HAL_GPIO_DeInit(GPIOK, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4 
                          |GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_0);

    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_12);

    HAL_GPIO_DeInit(GPIOI, GPIO_PIN_10|GPIO_PIN_9|GPIO_PIN_15|GPIO_PIN_14);

  /* USER CODE BEGIN LTDC_MspDeInit 1 */

  /* USER CODE END LTDC_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
void TFT_FillScreen(uint32_t color, uint8_t layer)
{
	uint32_t n = hltdc.LayerCfg[layer].ImageHeight*hltdc.LayerCfg[layer].ImageWidth;
	
	for (uint32_t i = 0; i < n; i++)
	{
		*(__IO uint32_t*)(hltdc.LayerCfg[layer].FBStartAdress + (4*i)) = color;
	}
}

void TFT_FillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color, uint8_t layer)
{
  
	uint32_t xpos, ypos;
	
	if (x1 > x2) swap(x1, x2);
	if (y1 > y2) swap(y1, y2);
	//смещение = ширина экрана минус ширина прямоугольника
	for(ypos = y1 ; ypos <= y2 ; ypos++)
	{
		for (xpos = x1; xpos <= x2; xpos++)
		{
			*(__IO uint32_t*)(hltdc.LayerCfg[layer].FBStartAdress + (4*(ypos*hltdc.LayerCfg[layer].ImageWidth + xpos))) = color;
		}
	}
}

void TFT_DrawRawPicture(uint32_t Xpos, uint32_t Ypos, uint32_t width, uint32_t height, uint8_t *pbmp, uint8_t layer)
{
	uint32_t address = 0, index = 0;
	const uint8_t bit_pixel = 32;
	/* адрес начала данных будущего рисунка в памяти SDRAM */
	address = hltdc.LayerCfg[layer].FBStartAdress + (((480*Ypos) + Xpos) * 4);
		
	for (index = 0; index < height; index++) //Передвигаемся по оси y
		{
	
			for (uint32_t ix = 0; ix < width; ix++)//по х
			{	
				*(uint32_t *)(address + (ix * 4)) = ((*(uint32_t *)(pbmp + (ix * 4)))&(0xFF00FF00)) + (((*(uint32_t *)(pbmp + (ix * 4)))&(0x000000FF)) << 16) + (((*(uint32_t *)(pbmp + (ix * 4)))&(0x00FF0000)) >> 16);  //данные о цвете точки по адресу
			}
			
			address += (480 * 4); /*запись по 4 байта*/
			pbmp += width*(bit_pixel / 8);
		}
}
void TFT_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t color, uint8_t layer)
{
	*(__IO uint32_t*)(hltdc.LayerCfg[layer].FBStartAdress + (4*(Ypos*hltdc.LayerCfg[layer].ImageWidth + Xpos))) = color;
}
void TFT_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color, uint8_t layer)
{
	int deltaX = abs(x2 - x1);
	int deltaY = abs(y2 - y1);
	int signX = x1 < x2 ? 1 : -1;
	int signY = y1 < y2 ? 1 : -1;
	int error = deltaX - deltaY;
	
	for (;;)
	{
		TFT_DrawPixel(x1, y1, color, layer);
		
		if (x1 == x2 && y1 == y2)
			break;
		
		int error2 = error * 2;
		
		if (error2 > -deltaY)
		{
			error -= deltaY;
			x1 += signX;
		}
		
		if (error2 < deltaX)
		{
			error += deltaX;
			y1 += signY;
		}
	}
}
void TFT_DrawHorizontLine(uint16_t x1, uint16_t y1, uint16_t y2, uint32_t color, uint8_t layer)
{
	TFT_DrawLine(x1, y1, x1, y2, color, layer);
}
void TFT_DrawVerticalLine(uint16_t x1, uint16_t x2, uint16_t y1, uint32_t color, uint8_t layer)
{
	TFT_DrawLine(x1, y1, x2, y1, color, layer);
}
void TFT_DrawRectangle(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom, uint32_t color, uint8_t layer)
{
	TFT_DrawLine(left, top, right, top, color, layer);
	TFT_DrawLine(left, bottom, right, bottom, color, layer);
	TFT_DrawLine(left, bottom, left, top, color, layer);
	TFT_DrawLine(right, bottom, right, top, color, layer);	
}
void TFT_DrawCircle(uint16_t x0, uint16_t y0, uint16_t radius, uint32_t color, uint8_t layer)
{
	int x = 0;
	int y = radius;
	int delta = 2 - 2 * radius;
	int error = 0;
	while (y >= 0)
	{
		TFT_DrawPixel(x0 + x, y0 + y, color, layer);
		TFT_DrawPixel(x0 + x, y0 - y, color, layer);
		TFT_DrawPixel(x0 - x, y0 + y, color, layer);
		TFT_DrawPixel(x0 - x, y0 - y, color, layer);
		error = 2 * (delta + y) - 1;
		if (delta < 0 && error <= 0)
		{
			++x;
			delta += 2 * x + 1;
			continue;
		}
		error = 2 * (delta - x) - 1;
		if (delta > 0 && error > 0)
		{
			--y;
			delta += 1 - 2 * y;
			continue;
		}
		++x;
		delta += 2 * (x - y);
		--y;
	}
}
void TFT_FillCircle(uint16_t x, uint16_t y, uint16_t radius, uint32_t color, uint8_t layer)
{
	int a_, b_, P;
	a_ = 0;
	b_ = radius;
	P = 1 - radius;
	while (a_ <= b_)
	{		
		TFT_FillRectangle(y - a_, x - b_, y + a_, x + b_, color, layer);
		TFT_FillRectangle(y - b_, x - a_, y + b_, x + a_, color, layer);
		
		if (P < 0)
		{
			P = (P + 3) + (2* a_);
			a_++;
		}
		else
		{
			P = (P + 5) + (2* (a_ - b_));
			a_++;
			b_--;
		}
	}
}
void TFT_DrawBitmap(uint32_t Xpos, uint32_t Ypos, uint8_t *pbmp, uint8_t layer)
{
	uint32_t address, index = 0, width = 0, height = 0, bit_pixel = 0;

	index = *(__IO uint16_t *)(pbmp + 10); //смещение изображения от начала файла
	index |= (*(__IO uint16_t *)(pbmp + 12)) << 16;
	
	/* Read bitmap width */
	width = *(uint16_t *)(pbmp + 18); //информационный заголовок, смещение 18, ширина
	width |= (*(uint16_t *)(pbmp + 20)) << 16;
	
	/* Read bitmap height */
	height = *(uint16_t *)(pbmp + 22); //информационный заголовок, смещение 22, длина
	height |= (*(uint16_t *)(pbmp + 24)) << 16;
	
	/* Read bit/pixel */
	bit_pixel = *(uint16_t *)(pbmp + 28); //информационный заголовок, смещение 28, бит на точку

	/* Set the address */
	address = hltdc.LayerCfg[layer].FBStartAdress + (((X_SIZE*Ypos) + Xpos) * 4);  // адрес начала данных будущего рисунка в памяти SDRAM
	
	/* Bypass the bitmap header */
	pbmp += (index + (width * (height - 1) * (bit_pixel / 8))); // указатель памяти на начало самого рисунка (минуем заголовок bmp)
	
	if ((bit_pixel / 8) == 4)//Если открыто изображение не с тем форматом 
	{
		TFT_FillScreen(0xFFFF0000, layer);
	}
	else if((bit_pixel / 8) == 2)//(RGB565)
	{
		TFT_FillScreen(0xFF00FF00, layer);
	}
	else
	{
		
		/* Convert picture to ARGB8888 pixel format */
		for (index = 0; index < height; index++) //Передвигаемся по оси y
			{
				for (uint32_t ix = 0; ix < width; ix++)//по х
				{	
					*(uint32_t *)(address + (ix * 4)) = convert24to32(*(uint32_t *)(pbmp + (ix * 3)));  //данные о цвете точки по адресу
				}
				address += (X_SIZE * 4);
				pbmp -= width*(bit_pixel / 8);
			}
		
	}
	bit_pixel = 0;	
}

void TFT_FontInit(void)//стартовая инициализация текста
{
	TFT.BackColor = LCD_COLOR_BLACK;
	TFT.TextColor = LCD_COLOR_YELLOW;
	TFT.pFont = &Font16;
}

void TFT_SetFont(sFONT* font)//выбор используемого шрифта
{
	TFT.pFont = font;
}
void TFT_SetColor(uint32_t color)//выбор цвета шрифта
{	
	TFT.TextColor = color;
}
void TFT_SetBackColor(uint32_t color)//выбор фона вокруг символа
{
	TFT.BackColor = color;
}
void TFT_DrawChar(uint16_t x, uint16_t y, const uint8_t c, uint8_t layer)
{
	uint32_t i = 0, j = 0;
	const uint8_t* ch;
	uint8_t* pchar;
	uint8_t offset;
	uint16_t height, width;
	uint32_t line;
	uint8_t W = ((TFT.pFont->Width + 7) / 8);
	//Вычисляем смещение символа в таблице шрифта (c-'0x20'=номер символа в таблице):
	ch = &TFT.pFont->table[(c - ' ')*TFT.pFont->Height*W];
	//Сохраняем ширину и высоту символа в переменные:
	width = TFT.pFont->Width;
	height = TFT.pFont->Height;
	//Вычисляем смещение в памяти:
	offset = 8*W - width;
	//Цикл вывода горизонтальных линий символа:
	for(i = 0 ; i < height ; i++)
	{
		pchar = ((uint8_t*)ch + W*i); //указатель на линию в символе
		//В зависимости от ширины символа добавляем в line все пиксели линии
		switch(W)
		{
		case 1:
			line = pchar[0];
			break;
			
		case 2:
			line = (pchar[0] << 8) | pchar[1];
			break;
			
		default:
			line = (pchar[0] << 16) | (pchar[1] << 8) | pchar[2];
			break;
		}
		//Цикл для вывода пикселей цвета символа и фона
		for(j = 0 ; j < width ; j++)
		{
			if (line & (1 << (width - j + offset - 1)))
			{
				TFT_DrawPixel(x + j, y, TFT.TextColor, layer);
			}
			else
			{
#ifdef BACKCOLOR
				TFT_DrawPixel(x + j, y, TFT.BackColor); //заполняем
#else
				__NOP();
#endif			
			}		
		}
		y++;
	}
}
void TFT_DisplayString(uint16_t X, uint16_t Y, uint8_t* Text, Text_AlignModeTypdef Mode, uint8_t layer)
{
	uint16_t ref_column = 1, i = 0;
	uint32_t size = 0, xsize = 0;
	uint8_t* ptr = Text;
	//Определение размера строки (движение по тексту до первого нуля)
	while(*ptr++) size++;
	//Вычисление макс. количество символов, которые смогут поместиться в ширину экрана
	xsize = (X_SIZE / TFT.pFont->Width);
	//Горизонтальная координата вывода первого символа строки в зависимости от способа выравнивания
	switch(Mode)
	{
	case CENTER_MODE:
		{
			ref_column = X + ((xsize - size)*TFT.pFont->Width / 2);
			break;
		}
	case RIGHT_MODE:
		{
			ref_column = -X + ((xsize - size)*TFT.pFont->Width);
			break;
		}
	case LEFT_MODE:
		{
			ref_column = X;
			break;
		}
	default:
		{
			ref_column = X;
			break;
		}
	}
	//Если выходим за левую часть экрана
	if((ref_column < 1) || (ref_column >= 0x800))
	{
		ref_column = 1;
	}
	//Вывод символов на экран:
	while((*Text != 0)&&((X_SIZE - (i*TFT.pFont->Width)) & 0xFFFF) >= TFT.pFont->Width)
	{
		TFT_DrawChar(ref_column, Y, *Text, layer);
		ref_column += TFT.pFont->Width;
		Text++;
		i++;
	}
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
