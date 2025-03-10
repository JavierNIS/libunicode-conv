#ifndef UTF16_CONVERSION
#define UTF16_CONVERSION

#include "unicode_common.h"

static inline mbsize_t 
CharLength16(const charUTF16_t* src, conversionInfo_t* conver){
  if(src == 0 || conver->_state == BAD)
    return 0;
  //It is assumed that character is not composed by surrogate pairs
  //In that case, the valid code points are U+0000 to U+D7FF and U+E000 to U+FFFF 
  if(*src < UTF16_MASK_HIGH_SURROGATE || 
      *src > UTF16_MASK_LOW_SURROGATE + TEN_LOWER_BITS) return 1; 

  /* If the previous statement didn't hold true, the character will be interpreted
   * as a surrogate pair. First the endianness is checked and then the pairs are 
   * interpreted.
   */
  charUTF16_t srcBE[2] = {src[0], src[1]};
  if(conver->_endianness == LITTLE_ENDIAN)
    SwapEndiannessU16(srcBE);

  if(UTF16_MASK_HIGH_SURROGATE <= *src && 
      *src <= UTF16_MASK_HIGH_SURROGATE + TEN_LOWER_BITS)
    if(UTF16_MASK_LOW_SURROGATE <= *(src+1) &&
        *(src+1) <= UTF16_MASK_LOW_SURROGATE + TEN_LOWER_BITS) return 2; 

  SetError(conver, (void*)src); 
  return 0;
}

mbsize_t 
UTF16toUTF8(const charUTF16_t* src, charUTF8_t* dest, conversionInfo_t* conver, const mbsize_t max);

mbsize_t 
UTF16toUTF32(const charUTF16_t* src, charUTF32_t* dest, conversionInfo_t* conver, const mbsize_t max);

#ifdef __linux__
mbsize_t 
UTF16toWIDE(const charUTF16_t* src, widechar_t* dest, conversionInfo_t* conver, const mbsize_t max);
#endif /*__linux__*/

#endif /*UTF16_CONVERSION*/
