/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "fatfs.h"
#include "ltdc.h"
#include "sdmmc.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "lodepng.h"
#include "decode.h"
#include "jpeglib.h"
#include "libjpeg.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct
{
	FATFS SDFatFs; 
	FIL MyFile; 
	FRESULT res; 
	uint32_t byteswritten; 
	uint32_t bytesread;
}SD;
/* User defined device identifier */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */


#define PNG_COUNT 29
PNG_PicTypedef Image[PNG_COUNT]; 
SD sdcard;
RGB_typedef *RGB_matrix;  
uint8_t   _aucLine[2048];
uint32_t  offset = 0;
uint32_t line_counter = 271;




/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */
static uint8_t Jpeg_CallbackFunction(uint8_t* Row, uint32_t DataLength);
static void OpenBMP(uint8_t *ptr, const char* fname);
static uint32_t OpenPNG(uint8_t *ptr, const char* fname);
static uint8_t DecodePNG(char* filename, uint8_t imagenumber);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	
	
  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_LTDC_Init();
  MX_FMC_Init();
  MX_SDMMC1_SD_Init();
  MX_FATFS_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	
	/*External SDRAM initialization*/
	MT48LC4M32B2_Init(&hsdram1);

	/*FatFS DISC MOUNTING*/
	if (f_mount(&sdcard.SDFatFs, "0", 0) != FR_OK)
	{
		HAL_UART_Transmit(&huart1, (uint8_t*)"Disk Mount - ERROR!\n", 20, 500); 
		Error_Handler();
	}
	else
	{
		HAL_UART_Transmit(&huart1, (uint8_t*)"Disk Mount - OK!\n", 17, 500);	  
	}
	
	TFT_FillScreen(0, 0);/*Clear screen*/
	TFT_FillScreen(0, 1);

	
#if 0
	/*BMP picture drawing*/
	uint8_t* bmp_buffer = (uint8_t*)malloc(600000*sizeof(uint8_t)); 
	OpenBMP(bmp_buffer, "SLZ.bmp");
	TFT_DrawBitmap(0, 0, bmp_buffer, 0);
	free(bmp_buffer);
#endif	
	
	
	f_chdir("JPG");/*CD to JPG directory*/
	if (f_open(&sdcard.MyFile, "1.jpg", FA_READ) == FR_OK)
	{
		jpeg_decode(&sdcard.MyFile, IMAGE_WIDTH, _aucLine, Jpeg_CallbackFunction);			
	}
	else
	{
		HAL_UART_Transmit(&huart1, (uint8_t*)"JPG - ERROR\n", 12, 500);
	}
	
	f_close(&sdcard.MyFile);
	f_chdir("/"); /*Back to root path*/
	
	if (DecodePNG("R.png", 28) != 1)
	{
		TFT_DrawRawPicture(0, 0, Image[28].width, Image[28].height, Image[28].storage, 1);
	}
	else
	{
		HAL_UART_Transmit(&huart1, (uint8_t*)"PNG - ERROR\n", 12, 500);
	}
	//free(Image[1].storage);
	TFT_SetFont(&Font24);
	TFT_SetColor(LCD_COLOR_RED);
	TFT_DisplayString(375, 5, (uint8_t*)"demo 3", LEFT_MODE, 1);
	TFT_SetFont(&Font20);
	TFT_SetColor(LCD_COLOR_YELLOW);
	TFT_DisplayString(370, 255, (uint8_t*)"30.04.20", LEFT_MODE, 1);
	
	f_chdir("GIF");
	char name[6] = { 0 };
	for (uint8_t i = 0; i < 24; i++)/*GIF png frames decoding*/
	{
		sprintf(name, "%d.png", i);
		DecodePNG(name, i);
	}
	
	

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
  
  while(1)
	{
		for (uint8_t i = 0; i < 22; i++)
		{
			TFT_DrawRawPicture(170, 50, Image[i].width, Image[i].height, Image[i].storage, 1);
			HAL_Delay(35);
			/*Future FreeRTOS Task!*/
		}	
		
	}
 
}			
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
/*Decode PNG into Image[imagenumber].storage field of Image Structure*/
static uint8_t DecodePNG(char* filename, uint8_t imagenumber)
{
	uint8_t* png_buffer = (uint8_t*)malloc(100000 * sizeof(uint8_t));
	/*Write png picture to sdram buffer and assign its size to field of Image structure*/
	Image[imagenumber].size = OpenPNG(png_buffer, filename);
	
	if (Image[imagenumber].size == 1) return 1;/*OpenPNG() ERROR*/
	
	/*Decode png to storage field of Image structure*/
	Image[imagenumber].error = lodepng_decode32(&Image[imagenumber].storage,
		&Image[imagenumber].width,
		&Image[imagenumber].height,
		png_buffer,
		Image[imagenumber].size);
	
	if (Image[1].error)
	{
		/*DECODING ERROR*/
		sprintf(Image[imagenumber].ErrorStr, "error %u: %s\n", 
			Image[imagenumber].error, 
			lodepng_error_text(Image[imagenumber].error));
		
		HAL_UART_Transmit(&huart1,
			(uint8_t*)Image[imagenumber].ErrorStr,
			sizeof(Image[imagenumber].ErrorStr),
			500);
	}
	free(png_buffer);
	return 0;
}

/*Write png from SD card to sdram buffer, and return its size in Bytes;
Return 1 if something goes wrong **/
static uint32_t OpenPNG(uint8_t *ptr, const char* fname)
{
		
		uint8_t sector[4096];
		uint32_t  size = 0, i1 = 0, ind1 = 0, pngsize = 0;
		const char PNGsign[8] = "504e47\n";/*True sign of png image*/
		char Sign[7] = { 0 };/*buffer for current sign*/
		char FileSize[25] = { 0 };
	
		if (f_open(&sdcard.MyFile, fname, FA_READ) != FR_OK)

		{
			HAL_UART_Transmit(&huart1, (uint8_t*)"Error!!\n", 8, 1000);  
		}
		else
		{
			size = (uint32_t)f_size(&sdcard.MyFile);
			pngsize = size;
			sprintf(FileSize, "PNG size: %lu B\n", size);  
			HAL_UART_Transmit(&huart1, (uint8_t*)"\nOpen Image - OK!\n", 18, 1000);  
			HAL_UART_Transmit(&huart1, (uint8_t*)FileSize, 18, 1000);
			if (f_read(&sdcard.MyFile, sector, 4, (UINT *)sdcard.bytesread) != FR_OK)
			{
				HAL_UART_Transmit(&huart1, (uint8_t*)"\nReading - ERROR!\n", 18, 1000);
				Error_Handler();
			}
			else
			{ 
				sprintf(Sign, "%x%x%x\n", sector[1], sector[2], sector[3]);
				if (!(strcmp(PNGsign, Sign)))
				{
					HAL_UART_Transmit(&huart1, (uint8_t*)"Sign - OK!\n", 11, 1000);
					do
					{
						if (size < 4096)
						{
							i1 = size;
						}
						else
						{
							i1 = 4096;
						}
						size -= i1;
						f_lseek(&sdcard.MyFile, ind1);   
						f_read(&sdcard.MyFile, sector, i1, (UINT *)&sdcard.bytesread);    
						memcpy((void*)(ptr + ind1), (void*)sector, i1);  
						ind1 += i1;
					} while (size > 0);
					HAL_UART_Transmit(&huart1, (uint8_t*)"PNG was loaded into png_buffer! \n", 33, 1000);
					f_close(&sdcard.MyFile);
				}
				else
				{
					return 1;/*It isn't PNG file*/
				}
				ind1 = 0;
			}		
		}
		return pngsize;		
	}	
static void OpenBMP(uint8_t *ptr, const char* fname)
{

		uint32_t  sz = 0, i1 = 0, ind1 = 0;
		static uint32_t bmp_addr;
		char FileSize[25] = { 0 }; 
		uint8_t sect[4096]; 

		if (f_open(&sdcard.MyFile, fname, FA_READ) != FR_OK)

		{
			HAL_UART_Transmit(&huart1, (uint8_t*)"Open Image - ERROR!\\n", 20, 1000); 
		}

		else
		{
			sz = (uint32_t)f_size(&sdcard.MyFile);
			sprintf(FileSize, "BMP size: %lu B\n", sz); 
			HAL_UART_Transmit(&huart1, (uint8_t*)"Open Image - OK!\n", 17, 1000); 
			HAL_UART_Transmit(&huart1, (uint8_t*)FileSize, 20, 1000);
			if (f_read(&sdcard.MyFile, sect, 30, (UINT *)sdcard.bytesread) != FR_OK)
			{
				Error_Handler();
			}
			else
			{
				bmp_addr = (uint32_t)sect; 
				sz = *(uint16_t *)(bmp_addr + 2); 
				sz |= (*(uint16_t *)(bmp_addr + 4)) << 16;
		
				do
				{
					if (sz < 4096)
					{
						i1 = sz;
					}
					else
					{
						i1 = 4096;
					}
					sz -= i1;
					f_lseek(&sdcard.MyFile, ind1);  
					f_read(&sdcard.MyFile, sect, i1, (UINT *)&sdcard.bytesread); 
					memcpy((void*)(ptr + ind1), (void*)sect, i1); 
					ind1 += i1;
				} while (sz > 0);
				f_close(&sdcard.MyFile);
			}
			ind1 = 0;
		}
	}

static uint8_t Jpeg_CallbackFunction(uint8_t* Row, uint32_t DataLength)
{
	/*DRAW JPG PICTURE ON LAYER0*/
	RGB_matrix =  (RGB_typedef*)Row;
	uint32_t  ARGB32Buffer[IMAGE_WIDTH];
   
	for (uint32_t ix = 0; ix < IMAGE_WIDTH; ix++)
	{
		ARGB32Buffer[ix]  = (uint32_t)
		(
		 ((RGB_matrix[ix].B << 16) |
		  (RGB_matrix[ix].G << 8) |
		  (RGB_matrix[ix].R) | 0xFF000000)
		);

		*(__IO uint32_t *)(hltdc.LayerCfg[0].FBStartAdress + (ix * 4) + (IMAGE_WIDTH * (IMAGE_HEIGHT - line_counter - 1) * 4)) = ARGB32Buffer[ix];
	}  

#ifdef SWAP_RB 
	uint32_t pixel = 0, width_counter, result = 0, result1 = 0;
  
	for (width_counter = 0; width_counter < IMAGE_WIDTH; width_counter++)
	{
		pixel = *(__IO uint32_t *)(LCD_FRAME_BUFFER + (width_counter * 4) + (IMAGE_WIDTH * (IMAGE_HEIGHT - line_counter - 1) * 4)); 
		result1 = (((pixel & 0x00FF0000) >> 16) | ((pixel & 0x000000FF) << 16));
		pixel = pixel & 0xFF00FF00;
		result = (result1 | pixel);
		*(__IO uint32_t *)(LCD_FRAME_BUFFER + (width_counter * 4) + (IMAGE_WIDTH * (IMAGE_HEIGHT - line_counter - 1) * 4)) = result;
     
	}  
#endif

	line_counter--;
	return 0;
}
  /* USER CODE END 3 */


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_SDMMC1|RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 100;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 5;
  PeriphClkInitStruct.PLLSAI.PLLSAIQ = 2;
  PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV2;
  PeriphClkInitStruct.PLLSAIDivQ = 1;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLLSAIP;
  PeriphClkInitStruct.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_CLK48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


	
/* USER CODE END 4 */

/* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();
  /** Initializes and configures the Region and the memory to be protected 
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0xC0000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_8MB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /** Initializes and configures the Region and the memory to be protected 
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.BaseAddress = 0xC0000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /** Initializes and configures the Region and the memory to be protected 
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER2;
  MPU_InitStruct.BaseAddress = 0xC0200000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
