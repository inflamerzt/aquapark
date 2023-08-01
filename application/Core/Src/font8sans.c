/*
 * font8sans.c
 *
 *  Created on: Jul 27, 2023
 *      Author: sheiko
 */


#include "font8sans.h"



//
//  Font data for Consolas 8pt
//

// Character bitmaps for Consolas 8pt
const uint_8 font_8ptBtmps[] =
{
	// @0 ',' (3 pixels wide)
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x03, //  ##
	0x03, //  ##
	0x02, //  #

	// @9 '.' (2 pixels wide)
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x03, // ##
	0x03, // ##
	0x00, //

	// @18 '0' (5 pixels wide)
	0x0E, //  ###
	0x11, // #   #
	0x13, // #  ##
	0x15, // # # #
	0x19, // ##  #
	0x11, // #   #
	0x0E, //  ###
	0x00, //

	// @27 '1' (5 pixels wide)
	0x0C, //  ##
	0x14, // # #
	0x04, //   #
	0x04, //   #
	0x04, //   #
	0x04, //   #
	0x1F, // #####
	0x00, //

	// @36 '2' (5 pixels wide)
	0x0E, //  ###
	0x11, // #   #
	0x01, //     #
	0x02, //    #
	0x04, //   #
	0x08, //  #
	0x1F, // #####
	0x00, //

	// @45 '3' (5 pixels wide)
	0x0E, //  ###
	0x11, // #   #
	0x01, //     #
	0x06, //   ##
	0x01, //     #
	0x11, // #   #
	0x0E, //  ###
	0x00, //

	// @54 '4' (5 pixels wide)
	0x06, //   ##
	0x0A, //  # #
	0x12, // #  #
	0x12, // #  #
	0x1F, // #####
	0x02, //    #
	0x02, //    #
	0x00, //

	// @63 '5' (5 pixels wide)
	0x1E, // ####
	0x10, // #
	0x10, // #
	0x1E, // ####
	0x01, //     #
	0x01, //     #
	0x1E, // ####
	0x00, //

	// @72 '6' (5 pixels wide)
	0x0E, //  ###
	0x10, // #
	0x10, // #
	0x1E, // ####
	0x11, // #   #
	0x11, // #   #
	0x0E, //  ###
	0x00, //

	// @81 '7' (5 pixels wide)
	0x1F, // #####
	0x01, //     #
	0x02, //    #
	0x02, //    #
	0x04, //   #
	0x04, //   #
	0x08, //  #
	0x00, //

	// @90 '8' (5 pixels wide)
	0x0E, //  ###
	0x11, // #   #
	0x11, // #   #
	0x0E, //  ###
	0x11, // #   #
	0x11, // #   #
	0x0E, //  ###
	0x00, //

	// @99 '9' (5 pixels wide)
	0x0E, //  ###
	0x11, // #   #
	0x11, // #   #
	0x0F, //  ####
	0x01, //     #
	0x02, //    #
	0x1C, // ###
	0x00, //

	// @108 ':' (2 pixels wide)
	0x00, //
	0x03, // ##
	0x03, // ##
	0x00, //
	0x00, //
	0x03, // ##
	0x03, // ##
	0x00, //
};

// Character descriptors for Consolas 8pt
// { [Char width in bits], [Offset into consolas_8ptCharBitmaps in bytes] }

const FONT_CHAR_INFO font_8ptChrsDescr[15] =
{
	{3, 0}, 		// ,
	{0, 0}, 		// -
	{2, 8}, 		// .
	{0, 0}, 		// /
	{5, 16}, 		// 0
	{5, 24}, 		// 1
	{5, 32}, 		// 2
	{5, 40}, 		// 3
	{5, 48}, 		// 4
	{5, 56}, 		// 5
	{5, 64}, 		// 6
	{5, 72}, 		// 7
	{5, 80}, 		// 8
	{5, 88}, 		// 9
	{2, 96}, 		// :
};

// Font information for Consolas 8pt
const FONT_INFO font_8pt_info =
{
	1, //  Character height
	',', //  Start character
	':', //  End character
	2, //  Width, in pixels, of space character
//    (*font_8ptChrsDescr), //  Character descriptor array
//	font_8ptBtmps, //  Character bitmap array
};

