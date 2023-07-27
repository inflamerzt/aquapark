/*
 * font8sans.h
 *
 *  Created on: Jul 27, 2023
 *      Author: sheiko
 */

#ifndef INC_FONT8SANS_H_
#define INC_FONT8SANS_H_

#include <fontsdescr.h>
#include "stdint.h"

#define uint_8 uint8_t

// Font data for Consolas 8pt
extern const uint_8 font_8ptBtmps[];
extern const FONT_INFO font_8pt_info;
extern const FONT_CHAR_INFO font_8ptChrsDescr[15];


//#define symb_8s_pos(ch) font_8pt_info.descr_arr[ch - font_8pt_info.stchar].position;
//#define symb_8s_siz(ch) font_8pt_info.descr_arr[ch - font_8pt_info.stchar].size;
#define fnt_8s_hght = font_8pt_info.height;
#define sp_8s_wdth = font_8pt_info.sp_width;


#endif /* INC_FONT8SANS_H_ */
