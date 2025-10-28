#ifndef UTF8_CONVERSION
#define UTF8_CONVERSION

#include "unicode_common.h"

static inline mbsize_t 
CharLength(const charUTF8_t* src, conversionInfo_t* conver){
  if(src == 0 || conver == 0 
      || ConversionHasError(conver))
    return 0;

  if((*src & UTF8_MASK_ONE_BYTE) == 0x0) //Code point of one byte
    return 1;
  charUTF8_t srcAux = *src & UTF8_MASK_FOUR_BYTES;
  if(srcAux == UTF8_MASK_TWO_BYTES || srcAux == 0xD0) //Two bytes
    return 2;
  else if(srcAux == UTF8_MASK_THREE_BYTES) //Three bytes
    return 3;
  else if(srcAux == UTF8_MASK_FOUR_BYTES) //Four bytes
    return 4;
  else{
    SetError(conver, (void*)src);
    return 0;
  }
}

mbsize_t 
UTF8toUTF16(const charUTF8_t* src, charUTF16_t* dest, conversionInfo_t* conver, const mbsize_t max);

mbsize_t 
UTF8toUTF32(const charUTF8_t* src, charUTF32_t* dest, conversionInfo_t* conver, const mbsize_t max);

mbsize_t 
UTF8toWIDE(const charUTF8_t* src, widechar_t* dest, conversionInfo_t* conver, const mbsize_t max);

#endif /*UTF8_CONVERSION*/
