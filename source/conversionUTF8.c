#include "UTF8.h"

////////////////////////////////////////////////////////////////////////////////

mbsize_t 
UTF8toUTF16(const charUTF8_t* src, charUTF16_t* dest, 
    conversionInfo_t* conver, const mbsize_t maxutf16bytes){
  mbsize_t u8_cp_size = CharLength(src, conver);
  //not a valid utf8 character
  //if no utf16bytes left for conversion
  if(!maxutf16bytes || maxutf16bytes + 3 == u8_cp_size)
    u8_cp_size=0; 

  switch (u8_cp_size){
    case 0:
      SetError(conver, (void*)src);
      break;
    case 1:
      *dest = *src;
      break;
    case 2:
      *dest = ((src[1] & SIX_LOWER_BITS) |
          (src[0] & FIVE_LOWER_BITS) << 6);
      break;
    case 3:
      *dest = ((src[2] & SIX_LOWER_BITS) | 
          (src[1] & SIX_LOWER_BITS) << 6 | 
          (src[0] & FOUR_LOWER_BITS) << 12);
      break;
    case 4:
      {}
      charUTF32_t code_point =
        ((src[0] & THREE_LOWER_BITS) << 18) + ((src[1] & SIX_LOWER_BITS) << 12) +
        ((src[2] & SIX_LOWER_BITS) << 6) + (src[3] & SIX_LOWER_BITS) -
        UTF16_CODE_POINT_SUBSTRACTION;

      dest[0] = (UTF16_MASK_HIGH_SURROGATE | ((code_point >> 10)));
      dest[1] = (UTF16_MASK_LOW_SURROGATE | ((code_point & TEN_LOWER_BITS)));
      if(ConversionWithLittleEndian(conver))
        SwapEndiannessU16(dest);
      break;
  }
  return u8_cp_size;
}

////////////////////////////////////////////////////////////////////////////////

mbsize_t 
UTF8toUTF32(const charUTF8_t* src, charUTF32_t* dest, conversionInfo_t* conver){
  mbsize_t u8_cp_size = CharLength(src, conver);
  
  switch (u8_cp_size){
    default:
      SetError(conver, (void*)src);
      break;
    case 1:
        *dest = *src;
      break;
    case 2:
        *dest = ((src[1] & SIX_LOWER_BITS) |
            (src[0] & FIVE_LOWER_BITS) << 6);
      break;
    case 3:
        *dest = ((src[2] & SIX_LOWER_BITS) |
            (src[1] & SIX_LOWER_BITS) << 6 |
            (src[0] & FOUR_LOWER_BITS) << 12);
      break;
    case 4:
        *dest = ((src[3] & SIX_LOWER_BITS) |
            (src[2] & SIX_LOWER_BITS) << 6 |
            (src[1] & SIX_LOWER_BITS) << 12 |
            (src[0] & THREE_LOWER_BITS) << 18);
      break;
  }

  if(ConversionWithLittleEndian(conver))
    SwapEndiannessU32(dest);
  return u8_cp_size;
}
////////////////////////////////////////////////////////////////////////////////

mbsize_t 
UTF8toWIDE(const charUTF8_t* src, widechar_t* dest, conversionInfo_t* conver, const mbsize_t maxutf16bytes){
#ifdef _WINDOWS_
  return UTF8toUTF16(src, dest, conver, maxutf16bytes);
#elif defined __linux__
  return UTF8toUTF32(src, dest, conver);
#endif
}
