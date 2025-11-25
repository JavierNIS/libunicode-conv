#include "UTF16.h"
#include "UTF8.h"

mbsize_t 
UTF16toUTF8(const charUTF16_t* src, charUTF8_t* dest, 
    conversionInfo_t* conver, const mbsize_t utf8bytes){
  //Get the size of the code point in numbers of 16 bytes needed
  mbsize_t u16_cp_size = CharLength16(src, conver);

  if(!utf8bytes || (u16_cp_size == 2 && utf8bytes < 4))
    u16_cp_size = 0;

  switch(u16_cp_size){
    //If the code point is smaller than 0xFFFF (can be stored in 16 bits)
    case 1:
      //Check if the code point can be stored in 7 bits (0xxx xxxx)
      if (*src < UTF8_MASK_ONE_BYTE){
        *dest = *src;
      //If it requires more than 7 bits, but less than 11 bits (110x xxxx 10xx xxxx)
      }else if (*src < 0x800 && utf8bytes >= 2){
        dest[0] = (UTF8_MASK_TWO_BYTES | ((*src >> 6) & FIVE_LOWER_BITS));
        dest[1] = (UTF8_MASK_ONE_BYTE | (*src & SIX_LOWER_BITS));
      //If it requires more than 11 bits (1110 xxxx 10xx xxxx 10xx xxxx, at a maximum of 16 bits)
      }else if(utf8bytes >= 3){
        dest[0] = (UTF8_MASK_THREE_BYTES | ((*src >> 12) & FOUR_LOWER_BITS));
        dest[1] = (UTF8_MASK_ONE_BYTE | ((*src >> 6) & SIX_LOWER_BITS));
        dest[2] = (UTF8_MASK_ONE_BYTE | (*src & SIX_LOWER_BITS));
      }else 
        SetError(conver, (void*)src);
      break;

    case 2:
      //Don't mind the {}, it is to avoid a C warning
      {}
      //The surrogate pairs get stored in an auxiliar pair
      charUTF16_t srcBE[2] = {src[0], src[1]};
      //All operations are done with big_endian in mind, so, if the 
      //code point was store in little endian, we convert the auxiliar pair to big endian
      if(ConversionWithLittleEndian(conver))
        SwapEndiannessU16(srcBE);
     
      //UTF-8 doesn't care about endianness, but it goes from the most relevant
      //bit to the least relevant bit
      uint32_t code_point = ((srcBE[0] & TEN_LOWER_BITS) << 10) + 
        (srcBE[1] & TEN_LOWER_BITS) + UTF16_CODE_POINT_SUBSTRACTION;
      dest[0] = (UTF8_MASK_FOUR_BYTES | ((code_point >> 18) & THREE_LOWER_BITS));
      dest[1] = (UTF8_MASK_ONE_BYTE | ((code_point >> 12) & SIX_LOWER_BITS));
      dest[2] = (UTF8_MASK_ONE_BYTE | ((code_point >> 6) & SIX_LOWER_BITS));
      dest[3] = (UTF8_MASK_ONE_BYTE | (code_point & SIX_LOWER_BITS));
      break;
    default: //If CharLength16 returned an error (length < 1), set the error
      SetError(conver, (void*)src);
  }
  return u16_cp_size;
}

mbsize_t 
UTF16toUTF32(const charUTF16_t* src, charUTF32_t* dest, conversionInfo_t* conver){

  mbsize_t u16_cp_size = CharLength16(src, conver);
  switch (u16_cp_size) {
    case 1:
      *dest = *src;
      if(ConversionWithLittleEndian(conver))
        SwapEndiannessU32(dest);
      break;
    case 2:
      {}
      if(!ConversionWithLittleEndian(conver)){
        *dest = 
          ((src[0] & TEN_LOWER_BITS) << 10) +
          (src[1] & TEN_LOWER_BITS) + UTF16_CODE_POINT_SUBSTRACTION;
      }else{
        charUTF16_t srcBE[2] = {src[1], src[0]};
        *dest = 
          ((srcBE[0] & TEN_LOWER_BITS) << 10) +
          (srcBE[1] & TEN_LOWER_BITS) + UTF16_CODE_POINT_SUBSTRACTION;
        SwapEndiannessU32(dest);
      }
      break;
    default:
      SetError(conver,(void*)src);
      break;
    }
  return u16_cp_size;
}

#ifdef __linux__
mbsize_t 
UTF16toWIDE(const charUTF16_t* src, widechar_t* dest, conversionInfo_t* conver, const mbsize_t max){
  return (UTF16toUTF32(src, dest, conver));
}

size_t 
UTF16stringToWIDE(const charUTF16_t *src, widechar_t *dest, 
    conversionInfo_t* conver, const size_t numberOfUnitsToEncode){
  return (UTF16stringToUTF32(src, dest, conver, numberOfUnitsToEncode));
}
#endif /*__linux__*/

size_t 
UTF16stringToUTF8(const charUTF16_t *src, charUTF8_t *dest, 
    conversionInfo_t* conver, const size_t utf8bytesAvailable){
  size_t bytesEncoded = 0;
  if(src == 0 || dest == 0 || utf8bytesAvailable == 0)
    return bytesEncoded;

  charUTF8_t *outputUTF8 = dest;
  charUTF8_t bufferUTF8[4];
  const charUTF16_t *inputUTF16 = src;
  mbsize_t inputUnitsConsumed, outputBytes;

  while(bytesEncoded + 1 < utf8bytesAvailable && *inputUTF16 != 0){
    inputUnitsConsumed = UTF16toUTF8(inputUTF16, bufferUTF8, conver, 4);

    if(ConversionHasError(conver)) break;

    outputBytes = CharLength(bufferUTF8, conver);
    if(outputBytes && bytesEncoded + outputBytes < utf8bytesAvailable){
      memcpy(outputUTF8, bufferUTF8, outputBytes);
      bytesEncoded += outputBytes;
      outputUTF8 += outputBytes;
      inputUTF16 += inputUnitsConsumed;
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
UTF16stringToUTF32(const charUTF16_t *src, charUTF32_t *dest, 
    conversionInfo_t* conver, const size_t utf32unitsAvailable){
  size_t utf32unitsEncoded = 0;
  if(src == 0 || dest == 0 || utf32unitsAvailable == 0)
    return utf32unitsEncoded;

  charUTF32_t *outputUTF32 = dest;
  const charUTF16_t *inputUTF16 = src;
  mbsize_t inputUnitsConsumed;
  
  //utf32unitsEncoded is increased by one to take into account that the string will be null terminated
  while(utf32unitsEncoded + 1 < utf32unitsAvailable && *inputUTF16 != 0){
    inputUnitsConsumed = UTF16toUTF32(inputUTF16, outputUTF32, conver);

    //if an error took place during conversion, we bail out
    if(ConversionHasError(conver)) break;
    //If the conversion was successful, we update the position of the pointers
    ++outputUTF32;
    ++utf32unitsEncoded;
    inputUTF16+=inputUnitsConsumed;
  }
  if(utf32unitsEncoded+1 == utf32unitsAvailable && *inputUTF16 != 0){
    //TOO_SHORT_STRING error in conversion
  }
  ++utf32unitsEncoded;
  memset(outputUTF32, 0, sizeof(charUTF32_t));
  return utf32unitsEncoded;
}
