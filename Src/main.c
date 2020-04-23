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
#include "dma2d.h"
#include "fatfs.h"
#include "ltdc.h"
#include "sdmmc.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MT48LC4M32B2B5.h"
#include "lodepng.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

FATFS SDFatFs;  /* File system object for SD card logical drive, объект файловой системы, который должен быть зарегистрирован*/
FIL MyFile;     /* FIL - file object structure - структура данных объекта файла*/ 
FRESULT res;/* FatFs function common result code флаги, возвращаемые функциями */

#define IMAGE_COUNT 2 /*Количество используемых png изображений */
uint32_t byteswritten, bytesread; /* Счётчики записанных, считанных байт */

PNG_Properties Image[IMAGE_COUNT]; /*объявление массива структур*/


uint8_t sect[4096];//буфер 4кБ для считывания картинки с карты памяти
uint8_t* bmp1;

char FileSize[25] ={0};//размер считываемого файла
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */
uint32_t OpenBMP(uint8_t *ptr, const char* fname);
static uint32_t OpenPNG(uint8_t *ptr, const char* fname);

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
  MX_DMA2D_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
    MT48LC4M32B2_Init(&hsdram1);
	//framebuffer = (uint8_t*)malloc(1048576*sizeof(uint8_t));//выделили память под фрэймбуфер
	
	
	//HAL_LTDC_SetAddress(&hltdc, (uint32_t )framebuffer, 0); //задали адрес фрэймбуфера
    

	if(f_mount(&SDFatFs, "0", 0) != FR_OK)
    {
		HAL_UART_Transmit(&huart1,(uint8_t*)"Disk Mount - ERROR!\n",20,500); 
        Error_Handler();
    }
    else
    {
		HAL_UART_Transmit(&huart1,(uint8_t*)"Disk Mount - OK!\n",17,500); 
		
    }
	//HAL_LTDC_SetAddress(&hltdc,0xC0000000, 0);
	TFT_FillScreen(LCD_COLOR_WHITE);
	
	bmp1 = (uint8_t*)malloc(600000*sizeof(uint8_t)); //выделили память под bmp картинку
	OpenBMP(bmp1,"SLZ.bmp");
	TFT_DrawBitmap(0, 0, bmp1);
	//TFT_SetFont(&Font24);
	//TFT_SetColor(LCD_COLOR_RED);
	//TFT_DisplayString(350,75,(uint8_t*)"SDRAM",LEFT_MODE);
    free(bmp1);
	uint8_t* png_buffer = (uint8_t*)malloc(100000 * sizeof(uint8_t));
	Image[0].size = OpenPNG(png_buffer, "UP.png");
	Image[0].error = lodepng_decode32(&Image[0].storage, &Image[0].width, &Image[0].height, png_buffer, Image[0].size);
	if (Image[0].error) printf("error %u: %s\n", Image[0].error, lodepng_error_text(Image[0].error));
#if 0
	Image[1].size = OpenPNG(png_buffer, "DOWN.png");
	Image[1].error = lodepng_decode32(&Image[1].storage, &Image[1].width, &Image[1].height, png_buffer, Image[1].size);
	if (Image[1].error) printf("error %u: %s\n", Image[1].error, lodepng_error_text(Image[1].error));
#endif	
	free(png_buffer);
	
	//TFT_DrawRawPicture(10, 0, Image[1].width, Image[1].height, Image[1].storage, 0);
	TFT_DrawRawPicture(180, 0, Image[0].width, Image[0].height, Image[0].storage, 0);
	//pLayerCfg.FBStartAdress = (uint32_t)Image[0].storage;
	
	free(Image[0].storage);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
#if 0
	  for (uint16_t j = 272; j > 0; j -= 8) {
			
		  TFT_DrawRawPicture(100, j, Image[0].width, Image[0].height, Image[0].storage, 0);		
		  HAL_Delay(25);			
	  }		
	  for (uint16_t z = 0; z < 272; z += 8) {
			
		  TFT_DrawRawPicture(100, z, Image[1].width, Image[1].height, Image[1].storage, 0);
		  HAL_Delay(25);
			
	  }
#endif
		HAL_GPIO_TogglePin(GPIOI,GLED_PIN_Pin);
				
		HAL_Delay(250);
  }
 
			
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  
  /* USER CODE END 3 */
}

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
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 4;
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
static uint32_t OpenPNG(uint8_t *ptr, const char* fname)/*запись файла из карты по адресу ptr*/
{
	
	uint8_t sector[4096]; /*буфер 4кБ для считывания картинки с карты памяти */
	uint32_t  size = 0, i1 = 0, ind1 = 0, pngsize = 0;
	const char PNGsign[8] = "504e47\n"; /* подпись png файла*/
	char Sign[7] = { 0 };
	char FileSize[25] = { 0 }; /* размер считываемого файла */
	
	if (f_open(&MyFile, fname, FA_READ) != FR_OK)//открываем картинку "fname", которая лежит на SD карте

	{
		HAL_UART_Transmit(&huart1, (uint8_t*)"Error!!\n", 8, 1000); //ошибка открытия картинки
	}
	else
	{
		size = (uint32_t)f_size(&MyFile);
		pngsize = size;
		sprintf(FileSize, "PNG size: %lu B\n", size); //находим размер открытого файла и преобразуем его в char
		HAL_UART_Transmit(&huart1, (uint8_t*)"\nOpen Image - OK!\n", 18, 1000); //картинка успешно открыта
		HAL_UART_Transmit(&huart1, (uint8_t*)FileSize, 18, 1000);
		if (f_read(&MyFile, sector, 4, (UINT *)bytesread) != FR_OK)//проверяем, соответствует ли сигнатура файла PNG
		{
			HAL_UART_Transmit(&huart1, (uint8_t*)"\nReading - ERROR!\n", 18, 1000);
			Error_Handler();
		}
		else
		{ 
			sprintf(Sign, "%x%x%x\n", sector[1], sector[2], sector[3]);
			if (!(strcmp(PNGsign, Sign)))//сравниваем сигнатуры открытого файла и PNG
			{
				HAL_UART_Transmit(&huart1, (uint8_t*)"Sign - OK!\n", 11, 1000);
				
			}
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
				f_lseek(&MyFile, ind1); //перемещаем указатель чтения по файлу на ind1
				f_read(&MyFile, sector, i1, (UINT *)&bytesread); //считываем в sect i1 байт
				memcpy((void*)(ptr + ind1), (void*)sector, i1); //копируем i1 символов из сектора в SDRAM
				ind1 += i1;
			} while (size > 0);
			HAL_UART_Transmit(&huart1, (uint8_t*)"PNG was loaded into png_buffer! \n", 33, 1000);
			f_close(&MyFile);
		}
		ind1 = 0;
	}
	return pngsize;
}	
uint32_t OpenBMP(uint8_t *ptr, const char* fname)
{

  uint32_t  sz = 0, i1 = 0, ind1 = 0;
  static uint32_t bmp_addr;

  if(f_open(&MyFile, fname, FA_READ) != FR_OK)//открываем картинку "fname", которая лежит на SD карте

  {
    HAL_UART_Transmit(&huart1,(uint8_t*)"Open Image - ERROR!\\n",20,1000);//ошибка открытия картинки
  }

  else
  {
		sz = (uint32_t)f_size(&MyFile);
		sprintf(FileSize,"BMP size: %lu B\n",sz);//находим размер открытого файла и преобразуем его в char
		HAL_UART_Transmit(&huart1,(uint8_t*)"Open Image - OK!\n",17,1000);//картинка успешно открыта
		HAL_UART_Transmit(&huart1,(uint8_t*)FileSize,20,1000);
		if (f_read (&MyFile, sect, 30, (UINT *)bytesread) != FR_OK)
    {
			Error_Handler();
    }
    else
		{
			bmp_addr = (uint32_t)sect;//записываем данные в сектор 4 кБ
            sz = *(uint16_t *) (bmp_addr + 2);//Прочитали 4 байта - размер файла картинки
            sz |= (*(uint16_t *) (bmp_addr + 4)) << 16;
		
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
                f_lseek(&MyFile,ind1);//перемещаем указатель чтения по файлу на ind1
                f_read (&MyFile, sect, i1, (UINT *)&bytesread);//считываем в sect i1 байт
                memcpy((void*)(bmp1+ind1), (void*)sect, i1);//копируем i1 символов из сектора в SDRAM
                ind1 += i1;
            }
            while (sz > 0);
            f_close (&MyFile);
        }
    ind1=0;
    }
	 return 0;
  }

	
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
  MPU_InitStruct.Size = MPU_REGION_SIZE_2MB;
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
