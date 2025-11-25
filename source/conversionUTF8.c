#include "UTF8.h"
#include "UTF16.h"

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

////////////////////////////////////////////////////////////////////////////////

size_t 
UTF8stringToUTF16(const charUTF8_t *src, charUTF16_t *dest, 
    conversionInfo_t *conver, const size_t utf16unitsAvailable){
  size_t utf16unitsEncoded = 0;
  if(src == 0 || dest == 0 || utf16unitsAvailable == 0)
    return utf16unitsEncoded;

  charUTF16_t *outputUTF16 = dest;
  charUTF16_t bufferUTF16[2];
  const charUTF8_t *inputUTF8 = src;
  mbsize_t inputBytesConsumed, outputUnits;
  
  //utf32unitsEncoded is increased by one to take into account that the string will be null terminated
  while(utf16unitsEncoded + 1 < utf16unitsAvailable && *inputUTF8 != '\0'){
    inputBytesConsumed = UTF8toUTF16(inputUTF8, bufferUTF16, conver, 2);
    
    //We need to check if an error happened during conversion, if nothing went wrong
    //we can check if the new character fits
    if(ConversionHasError(conver)) break;
    //No error happened, but we still need to check if we can fit the character in dest
    outputUnits = CharLength16(bufferUTF16, conver);
    if(outputUnits && utf16unitsEncoded + outputUnits < utf16unitsAvailable){
      memcpy(outputUTF16, bufferUTF16, outputUnits*sizeof(charUTF16_t));
      utf16unitsEncoded += outputUnits;
      outputUTF16 += outputUnits;
      inputUTF8 += inputBytesConsumed;
    }
    //No error happened, but the character can't fit in dest, we bail out and end the string
    else{
      //this path will generate the TOO_SHORT_STRING error in conversion
      //at the moment this isn't treated
      break;
    }

  }
  memset(outputUTF16, 0, sizeof(charUTF16_t));
  ++utf16unitsEncoded;
  return utf16unitsEncoded;
}

size_t 
UTF8stringToUTF32(const charUTF8_t *src, charUTF32_t *dest, 
    conversionInfo_t *conver, const size_t utf32unitsAvailable){
  size_t utf32unitsEncoded = 0;
  if(src == 0 || dest == 0 || utf32unitsAvailable == 0)
    return utf32unitsEncoded;

  charUTF32_t *outputUTF32 = dest;
  const charUTF8_t *inputUTF8 = src;
  mbsize_t inputBytesConsumed;
  
  //utf32unitsEncoded is increased by one to take into account that the string will be null terminated
  while(utf32unitsEncoded + 1 < utf32unitsAvailable && *inputUTF8 != '\0'){
    inputBytesConsumed = UTF8toUTF32(inputUTF8, outputUTF32, conver);

    //if an error took place during conversion, we bail out
    if(ConversionHasError(conver)) break;
    //If the conversion was successful, we update the position of the pointers
    ++outputUTF32;
    ++utf32unitsEncoded;
    inputUTF8+=inputBytesConsumed;
  }
  if(utf32unitsEncoded+1 == utf32unitsAvailable && *inputUTF8 != '\0'){
    //TOO_SHORT_STRING error in conversion
  }
  ++utf32unitsEncoded;
  memset(outputUTF32, 0, sizeof(charUTF32_t));
  return utf32unitsEncoded;

}

size_t 
UTF8stringToWIDE(const charUTF8_t *src, widechar_t *dest, 
    conversionInfo_t *conver, const size_t numberOfBytesAvailable){
#ifdef _WINDOWS_
  return UTF8stringToUTF16(src, dest, conver, numberOfBytesAvailable);
#elif defined __linux__
  return UTF8stringToUTF32(src, dest, conver, numberOfBytesAvailable);
#endif
}
