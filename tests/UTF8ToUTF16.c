#include "UTF32.h"
#include "UTF16.h"
#include "UTF8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const static mbsize_t num_mb = 4;
const static mbsize_t num_utf16_surr = 2;
static mbsize_t booleanBigEndian = 1;

void TestAllCodePoints(void){
  conversionInfo_t conver; 
  InitializeConversion(&conver, booleanBigEndian);
  for(uint32_t code_point = 0; code_point <= SSP_PLANE[1].end;
      code_point++){
    if(code_point >= 0xD800 && code_point <= 0xDFFF)
      continue;
    charUTF32_t utf32;
    charUTF16_t utf16[num_utf16_surr];
    charUTF8_t utf8[num_mb+1];
    IntegerToUTF32(&utf32, code_point, &conver);
    charUTF32_t endiannessCodepoint = utf32;
    printf("Code point (endianness): %u\n", endiannessCodepoint);
    printf("Code point: %u\n", code_point);

    UTF32toUTF8(&utf32, utf8, &conver, num_mb);
    if(ConversionHasError(&conver)){
      printf("Error in conversion from utf32 to utf8\n");
      exit(-1);
    }
    utf8[CharLength(utf8, &conver)] = 0;
    printf("UTF8 (From UTF32): %s\n", utf8);

    UTF8toUTF16(utf8, utf16, &conver, num_mb);
    if(ConversionHasError(&conver)){
      printf("Error in conversion from utf8 to utf16\n");
      exit(-1);
    }

    UTF16toUTF8(utf16, utf8, &conver, num_mb);
    if(ConversionHasError(&conver)){
      printf("Error in conversion from utf16 to utf8\n");
      exit(-1);
    }
    utf8[CharLength(utf8, &conver)] = 0;
    printf("UTF8 (From UTF16): %s\n", utf8);

    UTF8toUTF32(utf8, &utf32, &conver, num_mb);
    if(ConversionHasError(&conver)){
      printf("Error in conversion from utf16 to utf8\n");
      exit(-1);
    }
    else if(utf32 != endiannessCodepoint){
      printf("Error in conversion. Code points are not equal.\nCode point received: %d\n", utf32);
      printf("Hexadecimal, original: %x\tobtained:%x\n", code_point, utf32);
      exit(-1);
    }
  }
}

int main(int argc, char *argv[]){
  if(argc == 2 && strcmp("LE", argv[1]) == 0){
    booleanBigEndian = 0;
    printf("Using little endian\n\n");
  }
  TestAllCodePoints();
  printf("Test successful\n");
  return 0;
}
