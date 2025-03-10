#ifndef UTF32_CONVERSION
#define UTF32_CONVERSION

#include "unicode_common.h"

static inline mbsize_t
UTF32mbLength(const charUTF32_t* src){
  mbsize_t u8_cp_length = 0;
  if(*src < UTF8_MASK_ONE_BYTE)
    u8_cp_length = 1;
  else if(*src < 0x800)
    u8_cp_length = 2;
  else if(*src < UTF16_CODE_POINT_SUBSTRACTION)
    u8_cp_length = 3;
  else
    u8_cp_length = 4;
  return u8_cp_length;
}

static inline mbsize_t
UTF32bytesToUTF16(const charUTF32_t* src){
  return *src <= 0xFFFF ? 1 : 2;
}

mbsize_t 
UTF32toUTF8(const charUTF32_t* src, charUTF8_t* dest, conversionInfo_t* conver, const mbsize_t max);

mbsize_t 
UTF32toUTF16(const charUTF32_t* src, charUTF16_t* dest, conversionInfo_t* conver, const mbsize_t max);

#ifdef _WIN32
mbsize_t 
UTF32toWIDE(const charUTF32_t*, widechar_t*, conversionInfo_t*, const mbsize_t&);
#endif /*_WIN32*/

#endif /*UTF32_CONVERSION*/
