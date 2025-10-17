#include "UTF8.h"

//TESTED
//static inline mbsize_t 
//CharLength(const charUTF8_t* src, conversionInfo_t* conver){
//  if(src == 0 || conver->_state == BAD)
//    return 0;
//
//  if((*src & UTF8_MASK_ONE_BYTE) == 0x0) //Code point of one byte
//    return 1;
//  charUTF8_t srcAux = *src & UTF8_MASK_FOUR_BYTES;
//  if(srcAux == UTF8_MASK_TWO_BYTES || srcAux == 0xD0) //Two bytes
//    return 2;
//  else if(srcAux == UTF8_MASK_THREE_BYTES) //Three bytes
//    return 3;
//  else if(srcAux == UTF8_MASK_FOUR_BYTES) //Four bytes
//    return 4;
//  else{
//    SetError(conver, (void*)src);
//    return 0;
//  }
//}

////////////////////////////////////////////////////////////////////////////////

mbsize_t 
UTF8toUTF16(const charUTF8_t* src, charUTF16_t* dest, conversionInfo_t* conver, const mbsize_t max){
  mbsize_t u8_cp_size = CharLength(src, conver);
  if (u8_cp_size > max)
    u8_cp_size = 0;

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
      {}
      charUTF32_t code_point =
        ((src[0] & THREE_LOWER_BITS) << 18) + ((src[1] & SIX_LOWER_BITS) << 12) +
        ((src[2] & SIX_LOWER_BITS) << 6) + (src[3] & SIX_LOWER_BITS) -
        UTF16_CODE_POINT_SUBSTRACTION;

      dest[0] = (UTF16_MASK_HIGH_SURROGATE | ((code_point >> 10)));
      dest[1] = (UTF16_MASK_LOW_SURROGATE | ((code_point & TEN_LOWER_BITS)));
      if(areFlagsUnsetByte(conver->_flags, USING_BIG_ENDIAN))
        SwapEndiannessU16(dest);
      break;
  }
  return u8_cp_size;
}

////////////////////////////////////////////////////////////////////////////////

mbsize_t 
UTF8toUTF32(const charUTF8_t* src, charUTF32_t* dest, conversionInfo_t* conver, const mbsize_t max){
  mbsize_t u8_cp_size = CharLength(src, conver);
  if(u8_cp_size > max)
    u8_cp_size = 0;
  
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

  if(areFlagsUnsetByte(conver->_flags, USING_BIG_ENDIAN))
    SwapEndiannessU32(dest);
  return u8_cp_size;
}
////////////////////////////////////////////////////////////////////////////////

mbsize_t 
UTF8toWIDE(const charUTF8_t* src, widechar_t* dest, conversionInfo_t* conver, const mbsize_t max){
#ifdef _WINDOWS_
  return UTF8toUTF16(src, dest, conver, max);
#elif defined __linux__
  return UTF8toUTF32(src, dest, conver, max);
#endif
}
