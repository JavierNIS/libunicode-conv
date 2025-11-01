#include "UTF32.h"
#include "UTF8.h"
#include "UTF16.h"

mbsize_t 
UTF32toUTF8(const charUTF32_t* src, charUTF8_t* dest, 
    conversionInfo_t* conver, const mbsize_t utf8bytes){
  if(ConversionHasError(conver))
    return 0;

  charUTF32_t srcBE = *src;
  if(ConversionWithLittleEndian(conver))
    SwapEndiannessU32(&srcBE);

  mbsize_t u8_cp_length = UTF32mbLength(&srcBE);
  if(u8_cp_length > utf8bytes)
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
UTF32toUTF16(const charUTF32_t* src, charUTF16_t* dest, 
    conversionInfo_t* conver, const mbsize_t utf16bytes){
  if(ConversionHasError(conver))
    return 0;

  charUTF32_t srcBE = *src;
  if(ConversionWithLittleEndian(conver))
    SwapEndiannessU32(&srcBE);

  mbsize_t u16_cp_length = UTF32bytesToUTF16(&srcBE);
  if(u16_cp_length > utf16bytes)
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
      if(ConversionWithLittleEndian(conver))
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

size_t
UTF32stringToWIDE(const charUTF32_t* src, widechar_t* dest, conversionInfo_t* conver, const size_t max){
  return (UTF32stringToUTF16(src, dest, conver, max));
}
#endif /*_WINDOWS_*/

size_t 
UTF32stringToUTF8(const charUTF32_t *src, charUTF8_t *dest, 
    conversionInfo_t *conver, const size_t utf8bytesAvailable){
  size_t bytesEncoded = 0;
  if(src == 0 || dest == 0 || utf8bytesAvailable == 0)
    return bytesEncoded;

  const charUTF32_t *inputUTF32 = src;
  charUTF8_t buffer[4];
  charUTF8_t *outputUTF8 = dest;
  mbsize_t outputBytes;

  while(bytesEncoded + 1 < utf8bytesAvailable && *inputUTF32 != 0){
    outputBytes = UTF32toUTF8(inputUTF32, buffer, conver, 4);

    if(ConversionHasError(conver)) break;

    outputBytes = CharLength(buffer, conver);
    if(outputBytes && bytesEncoded + outputBytes < utf8bytesAvailable){
      memcpy(outputUTF8, buffer, outputBytes);
      bytesEncoded += outputBytes;
      outputUTF8 += outputBytes;
      ++inputUTF32;
    }else{
      //TOO_SHORT_STRING error
      break;
    }
  }
  *outputUTF8 = '\0';
  ++bytesEncoded;
  return bytesEncoded;
}

size_t 
UTF32stringToUTF16(const charUTF32_t *src, charUTF16_t *dest, 
    conversionInfo_t *conver, const size_t utf16unitsAvailable){
  size_t utf16unitsEncoded = 0;
  if(src == 0 || dest == 0 || utf16unitsAvailable == 0)
    return utf16unitsEncoded;

  const charUTF32_t *inputUTF32 = src;
  charUTF16_t buffer[2];
  charUTF16_t *outputUTF16 = dest;
  mbsize_t outputUnits;

  while(utf16unitsEncoded + 1 < utf16unitsAvailable && *inputUTF32 != 0){
    outputUnits = UTF32toUTF16(inputUTF32, buffer, conver, 2);

    if(ConversionHasError(conver)) break;

    outputUnits = CharLength16(buffer, conver);
    if(outputUnits && utf16unitsEncoded + outputUnits < utf16unitsAvailable){
      memcpy(outputUTF16, buffer, outputUnits*sizeof(charUTF16_t));
      utf16unitsEncoded += outputUnits;
      outputUTF16 += outputUnits;
      ++inputUTF32;
    }else{
      //TOO_SHORT_STRING error
      break;
    }
  }
  memset(outputUTF16, 0, sizeof(charUTF16_t));
  ++utf16unitsEncoded;
  return utf16unitsEncoded;
}
