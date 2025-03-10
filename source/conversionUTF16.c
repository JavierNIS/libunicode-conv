#include "UTF16.h"

mbsize_t 
UTF16toUTF8(const charUTF16_t* src, charUTF8_t* dest, conversionInfo_t* conver, const mbsize_t max){
  if(src == 0 || dest == 0 || conver == BAD)
    return 0;
  
  mbsize_t u16_cp_size = CharLength16(src, conver);
  if(u16_cp_size > max)
    u16_cp_size = 0;
  switch(u16_cp_size){
    case 1:
      if (*src < UTF8_MASK_ONE_BYTE){
        *dest = *src;
      }else if (*src < 0x800){
        dest[0] = (UTF8_MASK_TWO_BYTES | ((*src >> 6) & FIVE_LOWER_BITS));
        dest[1] = (UTF8_MASK_ONE_BYTE | (*src & SIX_LOWER_BITS));
      }else{
        dest[0] = (UTF8_MASK_THREE_BYTES | ((*src >> 12) & FOUR_LOWER_BITS));
        dest[1] = (UTF8_MASK_ONE_BYTE | ((*src >> 6) & SIX_LOWER_BITS));
        dest[2] = (UTF8_MASK_ONE_BYTE | (*src & SIX_LOWER_BITS));
      }
      break;
    case 2:
      {}
      charUTF16_t srcBE[2] = {src[0], src[1]};
      if(conver->_endianness == LITTLE_ENDIAN)
        SwapEndiannessU16(srcBE);
      
      /*uint8_t conversion_auxiliar = ((srcBE[0] & 0x03C0)>>6) + 1;
      dest[0] = (0xF0 | (conversion_auxiliar & 0x1C));
      dest[1] = (UTF8_MASK_ONE_BYTE | ((conversion_auxiliar & 0x03) << 4));
      dest[1] = (dest[1] | (srcBE[0] & 0x3C));
      dest[2] = (UTF8_MASK_ONE_BYTE | ((srcBE[0] & 0x3) << 4));
      dest[2] = (dest[2] | ((srcBE[1] & 0x03C0) >> 6));
      dest[3] = (UTF8_MASK_ONE_BYTE | (srcBE[1] & SIX_LOWER_BITS));*/

      uint32_t code_point = ((srcBE[0] & TEN_LOWER_BITS) << 10) + 
        (srcBE[1] & TEN_LOWER_BITS) + UTF16_CODE_POINT_SUBSTRACTION;
      dest[0] = (0xF0 | ((code_point >> 18) & 0x07));
      dest[1] = (UTF8_MASK_ONE_BYTE | ((code_point >> 12) & SIX_LOWER_BITS));
      dest[2] = (UTF8_MASK_ONE_BYTE | ((code_point >> 6) & SIX_LOWER_BITS));
      dest[3] = (UTF8_MASK_ONE_BYTE | (code_point & SIX_LOWER_BITS));
      break;
    default:
      SetError(conver, (void*)src);
  }
  return u16_cp_size;
}

//TESTED
mbsize_t 
UTF16toUTF32(const charUTF16_t* src, charUTF32_t* dest, conversionInfo_t* conver, const mbsize_t max){
  if(src == 0 || dest == 0 || conver->_state == BAD)
    return 0;
  mbsize_t u16_cp_size = CharLength16(src, conver);
  if(u16_cp_size > max)
    u16_cp_size = 0;
  switch (u16_cp_size) {
    case 1:
      *dest = *src;
      break;
    case 2:
      {}
      charUTF32_t destBE = 
        ((src[0] & TEN_LOWER_BITS) << 10) +
         (src[1] & TEN_LOWER_BITS) + UTF16_CODE_POINT_SUBSTRACTION;
      if(conver->_endianness == LITTLE_ENDIAN)
        SwapEndiannessU32(&destBE);
      *dest = destBE;
      }
  return u16_cp_size;
}

#ifdef __linux__
mbsize_t 
UTF16toWIDE(const charUTF16_t* src, widechar_t* dest, conversionInfo_t* conver, const mbsize_t max){
  return (UTF16toUTF32(src, dest, conver, max));
}
#endif /*__linux__*/
