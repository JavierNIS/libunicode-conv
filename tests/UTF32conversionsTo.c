#include "UTF32.h"
#include "UTF16.h"
#include "UTF8.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const static mbsize_t num_mb = 4;
const static mbsize_t num_utf16_surr = 2;
static mbsize_t booleanBigEndian = 1;


void TestALLCodePoints(void){
  conversionInfo_t conver; 
  InitializeConversion(&conver);
  if(!booleanBigEndian)
    unsetFlagByte(&conver._flags, USING_BIG_ENDIAN);

  for(uint32_t code_point = 0; code_point <= SSP_PLANE[1].end;
      code_point++){
    if(code_point >= 0xD800 && code_point <= 0xDFFF)
      continue;
    charUTF32_t utf32;
    charUTF16_t utf16[num_utf16_surr];
    charUTF8_t utf8[num_mb];
    utf32 = (charUTF32_t)code_point;
//    printf("Code point: %u\n", utf32);
    UTF32toUTF8(&utf32, utf8, &conver, num_mb);
    if(areFlagsUnsetByte(conver._flags, NO_FAILURE_OCURRED)){
      //printf("Error in conversion from utf32 to utf8\n");
      exit(-1);
    }
    //printf("UTF8: %s\n", utf8);
    UTF32toUTF16(&utf32, utf16, &conver, num_utf16_surr);
    if(areFlagsUnsetByte(conver._flags, NO_FAILURE_OCURRED)){
//      printf("Error in conversion from utf32 to utf16\n");
      exit(-1);
    }
//    printf("UTF16: %X %X\n\n", utf16[0], utf16[1%num_utf16_surr]);
    UTF8toUTF32(utf8, &utf32, &conver, num_mb);
    if(areFlagsUnsetByte(conver._flags, NO_FAILURE_OCURRED)){
//      printf("Error in conversion from utf8 to utf32\n");
      exit(-1);
    }
    else if (utf32 != code_point){
//      printf("Error in conversion, the code points are not equal (UTF8).");
//      printf("Code point obtained: %d\n", code_point);
      exit(-1);
    }
    UTF16toUTF32(utf16, &utf32, &conver, num_utf16_surr);
    if(areFlagsUnsetByte(conver._flags, NO_FAILURE_OCURRED)){
//      printf("Error in conversion from utf16 to utf32\n");
      exit(-1);
    }
    else if (utf32 != code_point){
//      printf("Error in conversion, the code points are not equal (UTF16).");
//      printf("Code point obtained: %d\n", utf32);
      exit(-1);
    }
  }
}


int main(int argc, char *argv[]){
  if(argc == 2 && strcmp("LE", argv[1]) == 0){
    booleanBigEndian = 0;
    printf("Using little endian\n\n");
  }
  TestALLCodePoints();
  printf("Test successful\n");
  return 0;
}
