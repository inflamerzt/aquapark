/*
 * fonts.h
 *
 *  Created on: Jul 27, 2023
 *      Author: sheiko
 */

#ifndef INC_FONTSDESCR_H_
#define INC_FONTSDESCR_H_


#include "stdint.h"

#define uint_8 uint8_t

typedef struct{
uint8_t size;
uint8_t position;
}FONT_CHAR_INFO;

typedef struct
{
uint8_t height;
char stchar;
char endchar;
uint8_t sp_width;
FONT_CHAR_INFO descr_arr[15];
uint8_t	bitmap_arr[];
}FONT_INFO;






#endif /* INC_FONTSDESCR_H_ */



