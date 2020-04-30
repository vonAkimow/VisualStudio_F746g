#include "ImageProcessing.h"
/*STRUCTURES*/













/*FUNCTION PROTOTYPES*/
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
	
	if (Image[imagenumber].error)
	{
		/*DECODING ERROR*/
		sprintf(Image[imagenumber].ErrorStr,
			"error %u: %s\n", 
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
	const char PNGsign[8] = "504e47\n"; /*True sign of png image*/
	char Sign[7] = { 0 }; /*buffer for current sign*/
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