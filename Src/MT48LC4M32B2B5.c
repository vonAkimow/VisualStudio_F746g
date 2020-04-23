#include "MT48LC4M32B2B5.h"

FMC_SDRAM_CommandTypeDef command;//для FMC команд
HAL_StatusTypeDef hal_stat;//для статуса возврата из функций




void MT48LC4M32B2_Init(SDRAM_HandleTypeDef *hsdram)
{
	volatile uint32_t tmpmrd =0;
	
  command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;//включаем тактирование
  command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;//работаем с первым банком
  command.AutoRefreshNumber = 1;//одно автообновление
  command.ModeRegisterDefinition = 0;
  hal_stat = HAL_SDRAM_SendCommand(hsdram, &command, SDRAM_TIMEOUT);
  HAL_Delay(1);//задержка для выполнения команды и отладки (hal_stat = HAL_OK)
	
	command.CommandMode = FMC_SDRAM_CMD_PALL;//деактивация всех банков памяти перед регенерацией
  command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  command.AutoRefreshNumber = 1;
  command.ModeRegisterDefinition = 0;
  hal_stat = HAL_SDRAM_SendCommand(hsdram, &command, SDRAM_TIMEOUT);
  
	
	command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;//команда включения регистра авторегенерации
  command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  command.AutoRefreshNumber = 8;
  command.ModeRegisterDefinition = 0;
  hal_stat = HAL_SDRAM_SendCommand(hsdram, &command, SDRAM_TIMEOUT);
  
	
	tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |//Program the external memory mode register
                     SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |//Размер пакета, последовательный режим передачи, латентность 2 такта, стандартный режим, одиночный режим записи пакета
                     SDRAM_MODEREG_CAS_LATENCY_2           |
                     SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                     SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
  
  command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
  command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  command.AutoRefreshNumber = 1;
  command.ModeRegisterDefinition = tmpmrd;
	hal_stat = HAL_SDRAM_SendCommand(hsdram, &command, SDRAM_TIMEOUT);
	
	HAL_SDRAM_ProgramRefreshRate(hsdram, REFRESH_COUNT ); 
}
