#ifndef _ENDIANNESS_UNICODE
#define _ENDIANNESS_UNICODE

#include "chars.h"

#define LITTLE_ENDIAN 0
#define BIG_ENDIAN 1
#define SAME_ENDIANNESS_AS_CPU 2


static inline int CheckEndianness(void){
  int integer = 1;
  char *byte = (char *)&integer;
  
  //To see if it is little endian, we check if the lowest value bit
  //comes in the first byte, in that case we know that it is little endian
  return *byte == 1 ? LITTLE_ENDIAN : BIG_ENDIAN;
}

static inline void
SwapEndiannessU32(charUTF32_t* src){
  charUTF32_t auxiliar = *src;
  *src = (((auxiliar >> 24) & 0x000000ff) | //3th byte to 0th
      ((auxiliar << 8) & 0x00ff0000) | //1st byte to 2th
      ((auxiliar >> 8) & 0x0000ff00) | //2th byte to 1st
      ((auxiliar << 24) & 0xff000000)); //0th byte to 3th
}

static inline void
SwapEndiannessU16(charUTF16_t* src){
  *src = (*src << 8 | *src >> 8);
}


#endif
