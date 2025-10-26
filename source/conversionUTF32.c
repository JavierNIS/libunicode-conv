#include "UTF32.h"

mbsize_t 
UTF32toUTF8(const charUTF32_t* src, charUTF8_t* dest, conversionInfo_t* conver, const mbsize_t max){
  if(dest == 0 || areFlagsUnsetByte(conver->_flags, NO_FAILURE_OCURRED))
    return 0;

  charUTF32_t srcBE = *src;
  if(areFlagsUnsetByte(conver->_flags, USING_BIG_ENDIAN))
    SwapEndiannessU32(&srcBE);

  mbsize_t u8_cp_length = UTF32mbLength(&srcBE);
  if(u8_cp_length > max)
    u8_cp_length = 0;


  switch (u8_cp_length) {
    case 1:
      *dest = srcBE; 
      break;
    case 2:
      dest[0] = (UTF8_MASK_TWO_BYTES | ((srcBE >> 6) & FIVE_LOWER_BITS));
      dest[1] = (UTF8_MASK_ONE_BYTE | (srcBE & SIX_LOWER_BITS));
      break;
    case 3:
      dest[0] = (UTF8_MASK_THREE_BYTES | ((srcBE >> 12) & FOUR_LOWER_BITS));
      dest[1] = (UTF8_MASK_ONE_BYTE | ((srcBE >> 6) & SIX_LOWER_BITS));
      dest[2] = (UTF8_MASK_ONE_BYTE | (srcBE & SIX_LOWER_BITS));
      break;
    case 4:
      dest[0] = (UTF8_MASK_FOUR_BYTES | ((srcBE >> 18) & THREE_LOWER_BITS));
      dest[1] = (UTF8_MASK_ONE_BYTE | ((srcBE >> 12) & SIX_LOWER_BITS));
      dest[2] = (UTF8_MASK_ONE_BYTE | ((srcBE >> 6) & SIX_LOWER_BITS));
      dest[3] = (UTF8_MASK_ONE_BYTE | (srcBE & SIX_LOWER_BITS));
      break;
    default:
      SetError(conver, (void*)src);
      break;
  }
  
  return u8_cp_length;
}

mbsize_t 
UTF32toUTF16(const charUTF32_t* src, charUTF16_t* dest, conversionInfo_t* conver, const mbsize_t max){
  if(dest == 0 || areFlagsUnsetByte(conver->_flags, NO_FAILURE_OCURRED))
    return 0;
  charUTF32_t srcBE = *src;
  if(areFlagsUnsetByte(conver->_flags, USING_BIG_ENDIAN))
    SwapEndiannessU32(&srcBE);

  mbsize_t u16_cp_length = UTF32bytesToUTF16(&srcBE);
  if(u16_cp_length > max)
    u16_cp_length = 0;
  switch (u16_cp_length){
    case 1:
      *dest = srcBE;
      break;
    case 2:
      {}
      srcBE-= UTF16_CODE_POINT_SUBSTRACTION;
      dest[0] = ( UTF16_MASK_HIGH_SURROGATE | ((srcBE >> 10) & TEN_LOWER_BITS));
      dest[1] = ( UTF16_MASK_LOW_SURROGATE | (srcBE & TEN_LOWER_BITS));
      if(areFlagsUnsetByte(conver->_flags, USING_BIG_ENDIAN))
        SwapEndiannessU16(dest);
      break;
    default:
      SetError(conver, (void*)src);
      break;
  }
  return u16_cp_length;
}

#ifdef _WINDOWS_
mbsize_t 
UTF32toWIDE(const charUTF32_t* src, widechar_t* dest, conversionInfo_t* conver, const mbsize_t max){
  return (UTF32toUTF16(src, dest, conver, max));
}
#endif /*_WINDOWS_*/
