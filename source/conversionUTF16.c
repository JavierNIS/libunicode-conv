#include "UTF16.h"
#include "mbstate.h"

mbsize_t 
UTF16toUTF8(const charUTF16_t* src, charUTF8_t* dest, conversionInfo_t* conver, const mbsize_t max){
  //First check if the src and destination are okay, this is redundant and will be removed
  if(src == 0 || dest == 0 || conver == BAD)
    return 0;
  
  //Get the size of the code point in numbers of 16 bytes needed
  mbsize_t u16_cp_size = CharLength16(src, conver);

  switch(u16_cp_size){
    //If the code point is smaller than 0xFFFF (can be stored in 16 bits)
    case 1:
      //Check if the code point can be stored in 7 bits (0xxx xxxx)
      if (*src < UTF8_MASK_ONE_BYTE){
        *dest = *src;
      //If it requires more than 7 bits, but less than 11 bits (110x xxxx 10xx xxxx)
      }else if (*src < 0x800){
        dest[0] = (UTF8_MASK_TWO_BYTES | ((*src >> 6) & FIVE_LOWER_BITS));
        dest[1] = (UTF8_MASK_ONE_BYTE | (*src & SIX_LOWER_BITS));
      //If it requires more than 11 bits (1110 xxxx 10xx xxxx 10xx xxxx, at a maximum of 16 bits)
      }else{
        dest[0] = (UTF8_MASK_THREE_BYTES | ((*src >> 12) & FOUR_LOWER_BITS));
        dest[1] = (UTF8_MASK_ONE_BYTE | ((*src >> 6) & SIX_LOWER_BITS));
        dest[2] = (UTF8_MASK_ONE_BYTE | (*src & SIX_LOWER_BITS));
      }
      break;

    case 2:
      //Don't mind the {}, it is to avoid a C warning
      {}
      //The surrogate pairs get stored in an auxiliar pair
      charUTF16_t srcBE[2] = {src[0], src[1]};
      //All operations are done with big_endian in mind, so, if the 
      //code point was store in little endian, we convert the auxiliar pair to big endian
      if(conver->_endianness == LITTLE_ENDIAN)
        SwapEndiannessU16(srcBE);
     
      //UTF-8 doesn't care about endianness, but it goes from the most relevant
      //bit to the least relevant bit
      uint32_t code_point = ((srcBE[0] & TEN_LOWER_BITS) << 10) + 
        (srcBE[1] & TEN_LOWER_BITS) + UTF16_CODE_POINT_SUBSTRACTION;
      dest[0] = (0xF0 | ((code_point >> 18) & 0x07));
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
UTF16toUTF32(const charUTF16_t* src, charUTF32_t* dest, conversionInfo_t* conver, const mbsize_t max){
  //Again a redundant check, will be removed later on
  if(src == 0 || dest == 0 || conver->_state == BAD)
    return 0;
 
  //Endianness needs to get tested, this looks weird
  mbsize_t u16_cp_size = CharLength16(src, conver);
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
  return (UTF16toUTF32(src, dest, conver, max));
}
#endif /*__linux__*/
