#ifndef UNICODE_COMMON
#define UNICODE_COMMON

#include "platform_integration.h"
#include "mbstate.h"
#include "codepoints.h"

static inline void
SwapEndiannessU32(charUTF32_t* src){
  charUTF32_t auxiliar = *src;
  *src = (((auxiliar >> 24) & 0x000000ff) | //3th byte to 0th
      ((auxiliar << 8) & 0x00ff0000) | //1th byte to 2th
      ((auxiliar >> 8) & 0x0000ff00) | //2th byte to 1th
      ((auxiliar << 24) & 0xff000000)); //0th byte to 3th
}

static inline void
SwapEndiannessU16(charUTF16_t* src){
  charUTF16_t auxiliar = src[0];
  src[0] = src[1];
  src[1] = auxiliar;
}

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

////////////////// BIT MANIPULATION MACROS
#define TEN_LOWER_BITS   0x3FF

#define SIX_LOWER_BITS   0x3F
#define FIVE_LOWER_BITS  0x1F
#define FOUR_LOWER_BITS  0x0F
#define THREE_LOWER_BITS 0x07

#endif /*UNICODE_MARCROS*/

#endif /*UNICODE_COMMON*/
