#include "UTF32.h"
#include "UTF16.h"
#include "UTF8.h"
#include "unicode_common.h"
#include "print.h"
#include <stdlib.h>

const static mbsize_t num_mb = 4;
const static mbsize_t num_utf16_surr = 2;
static mbsize_t booleanBigEndian = 1;


void TestALLCodePoints(void){
  conversionInfo_t conver; 
  InitializeConversion(&conver, booleanBigEndian);
  
  for(uint32_t code_point = 0; code_point <= SSP_PLANE[1].end;
      code_point++){
    if(code_point >= 0xD800 && code_point <= 0xDFFF)
      continue;
    charUTF32_t utf32 = (charUTF32_t)code_point;
    charUTF16_t utf16[num_utf16_surr];
    charUTF8_t utf8[num_mb+1];
    charUTF32_t utf32_code_point = utf32;
    //printf("Code point: %u\n", code_point);

    UTF32toUTF8(&utf32, utf8, &conver, num_mb);
    if(ConversionHasError(&conver)){
      printf("Error in conversion from utf32 to utf8\n");
      exit(-1);
    }
    utf8[CharLength(utf8, &conver)] = 0;
    //printf("UTF8: %s\n", utf8);

    UTF32toUTF16(&utf32, utf16, &conver, num_utf16_surr);
    if(ConversionHasError(&conver)){
      printf("Error in conversion from utf32 to utf16\n");
      exit(-1);
    }
    //printf("UTF16: %X %X\n\n UTF16 Bytes: ", utf16[0], utf16[1]);
    write_utf16(stdout, utf16, 2, conver._flags);
    //printf("\n\n");

    //Now we go back to UTF-32.
    UTF8toUTF32(utf8, &utf32, &conver);
    if(ConversionHasError(&conver)){
      printf("Error in conversion from utf8 to utf32\n");
      exit(-1);
    }
    else if (utf32 != utf32_code_point){
      printf("Error in conversion, the code points are not equal (UTF8). ");
      printf("Code point obtained: %d\n", utf32);
      exit(-1);
    }

    UTF16toUTF32(utf16, &utf32, &conver);
    if(ConversionHasError(&conver)){
      printf("Error in conversion from utf16 to utf32\n");
      exit(-1);
    }
    else if (utf32 != utf32_code_point){
      printf("Error in conversion, the code points are not equal (UTF16). ");
      printf("Code point obtained: %d\n", utf32);
      exit(-1);
    }
  }
}


int main(int argc, char *argv[]){
  if(argc == 2 && strcmp("LE", argv[1]) == 0){
    booleanBigEndian = 0;
    printf("Using little endian\n\n");
  }else{
    printf("Using big endian\n\n");
  }
  TestALLCodePoints();
  printf("Test successful\n");
  return 0;
}
