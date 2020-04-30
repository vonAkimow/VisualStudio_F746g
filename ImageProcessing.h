#ifndef IMAGEP_H
#define IMAGEP_H
#include <stdint.h>
#include <stdio.h>
#include "fatfs.h"
#include "ltdc.h"
#include "usart.h"
#include "lodepng.h"
typedef struct {
	
	unsigned 		width;
	unsigned 		height;
	unsigned 		error;
	size_t 			size;
	unsigned char* storage; 
	char ErrorStr[15];
	
}PNG_Properties;




































#endif // !IMAGEP_H

