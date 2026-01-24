#ifndef UNICODE_COMMON
#define UNICODE_COMMON


#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "chars.h"
#include "uc_bitmask.h"
#include "mbstate.h"
#include "codepoints.h"


#ifndef UNICODE_MACROS
#define UNICODE_MACROS

////////////////// UTF8
#define UTF8_MASK_ONE_BYTE    0x80
#define UTF8_MASK_TWO_BYTES   0xC0
#define UTF8_MASK_THREE_BYTES 0xE0
#define UTF8_MASK_FOUR_BYTES  0xF0

////////////////// UTF16
#define UTF16_MASK_HIGH_SURROGATE     0xD800
#define UTF16_MASK_LOW_SURROGATE      0xDC00
#define UTF16_CODE_POINT_SUBSTRACTION 0x10000
#define UTF16_CODE_POINT_SUBSTRACTION_LE 0x00000100 //The same as the one above but with bytes swapped

////////////////// BIT MANIPULATION MACROS
#define TEN_LOWER_BITS   0x3FF

#define SIX_LOWER_BITS   0x3F
#define FIVE_LOWER_BITS  0x1F
#define FOUR_LOWER_BITS  0x0F
#define THREE_LOWER_BITS 0x07

#endif /*UNICODE_MARCROS*/

#endif /*UNICODE_COMMON*/
