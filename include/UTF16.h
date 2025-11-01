#ifndef UTF16_CONVERSION
#define UTF16_CONVERSION

#include "unicode_common.h"

static inline mbsize_t 
CharLength16(const charUTF16_t* src, conversionInfo_t* conver){
  if(src == 0 || conver == 0 
      || ConversionHasError(conver))
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
  if(ConversionWithLittleEndian(conver))
    SwapEndiannessU16(srcBE);

  if(UTF16_MASK_HIGH_SURROGATE <= srcBE[0] && 
      srcBE[0] <= UTF16_MASK_HIGH_SURROGATE + TEN_LOWER_BITS)
    if(UTF16_MASK_LOW_SURROGATE <= srcBE[1] &&
        srcBE[1] <= UTF16_MASK_LOW_SURROGATE + TEN_LOWER_BITS) return 2; 

  return 0;
}

mbsize_t 
UTF16toUTF8(const charUTF16_t* src, charUTF8_t* dest, 
    conversionInfo_t* conver, const mbsize_t utf8bytes);

mbsize_t 
UTF16toUTF32(const charUTF16_t* src, charUTF32_t* dest, conversionInfo_t* conver);

#ifdef __linux__
mbsize_t 
UTF16toWIDE(const charUTF16_t* src, widechar_t* dest, 
    conversionInfo_t* conver, const mbsize_t max);

size_t 
UTF16stringToWIDE(const charUTF16_t *src, widechar_t *dest, 
    conversionInfo_t* conver, const size_t numberOfUnitsToEncode);
#endif /*__linux__*/

size_t 
UTF16stringToUTF8(const charUTF16_t *src, charUTF8_t *dest, 
    conversionInfo_t* conver, const size_t utf8bytesAvailable);

size_t 
UTF16stringToUTF32(const charUTF16_t *src, charUTF32_t *dest, 
    conversionInfo_t* conver, const size_t utf32unitsAvailable);

#endif /*UTF16_CONVERSION*/
