#include "UTF8.h"
#include "UTF16.h"
#include "UTF32.h"
#include <stdlib.h>
#include <stdio.h>

#define NUMBER_OF_TESTS 6

enum{
  UTF8_UTF16 = 0,
  UTF8_UTF32,
  UTF16_UTF8,
  UTF16_UTF32,
  UTF32_UTF8,
  UTF32_UTF16
};

void testStrings(const charUTF8_t *string, const size_t codepoints, 
    const size_t numberutf16, const size_t numberOfBytes, conversionInfo_t * conver){
  charUTF16_t *utf16string = (charUTF16_t*)malloc(sizeof(charUTF16_t)*numberutf16);
  charUTF32_t *utf32string = (charUTF32_t*)malloc(sizeof(charUTF32_t)*codepoints);
  charUTF8_t *utf8string = (charUTF8_t*)malloc(sizeof(charUTF8_t)*numberOfBytes);

  char *testStrings[NUMBER_OF_TESTS] = {
    "==================================\nConverting from UTF-8 to UTF-16...\n==================================\n\n",
    "==================================\nConverting from UTF-8 to UTF-32...\n==================================\n\n",
    "==================================\nConverting from UTF-16 to UTF-8...\n==================================\n\n",
    "==================================\nConverting from UTF-16 to UTF-32...\n==================================\n\n",
    "==================================\nConverting from UTF-32 to UTF-8...\n==================================\n\n",
    "==================================\nConverting from UTF-32 to UTF-16...\n==================================\n\n"
  };
  
  size_t outputEncoded, numTest;
  mbsize_t has_error=0;

  for(int test_i = 0; test_i < NUMBER_OF_TESTS; test_i++){
    printf("%s", testStrings[test_i]);

    switch(test_i){
      case UTF8_UTF16:
        outputEncoded = UTF8stringToUTF16(string, utf16string, conver, numberutf16);
        break;
      case UTF8_UTF32:
        outputEncoded = UTF8stringToUTF32(string, utf32string, conver, codepoints);
        break;
      case UTF16_UTF8:
        outputEncoded = UTF16stringToUTF8(utf16string, utf8string, conver, numberOfBytes);
        break;
      case UTF16_UTF32:
        outputEncoded = UTF16stringToUTF32(utf16string, utf32string, conver, codepoints);
        break;
      case UTF32_UTF8:
        outputEncoded = UTF32stringToUTF8(utf32string, utf8string, conver, numberOfBytes);
        break;
      case UTF32_UTF16:
        outputEncoded = UTF32stringToUTF16(utf32string, utf16string, conver, numberutf16);
        break;
    }
    if(ConversionHasError(conver)){
      printf("Current conversion had an error detected by helper struct\n");
      has_error++;
    }
    switch(test_i){
      case UTF16_UTF8:
      case UTF32_UTF8:
        numTest = numberOfBytes;
        break;
      case UTF8_UTF16:
      case UTF32_UTF16:
        numTest = numberutf16;
        break;
      case UTF8_UTF32:
      case UTF16_UTF32:
        numTest = codepoints;
        break;
    }
    has_error = outputEncoded == numTest ? has_error : 1+has_error;
    if(has_error){
      if(has_error == 2){
        printf("Error detected in output received too.\n");
      }
      else
        printf("Error detected in output received.\n");
      printf("Output gotten: %zu\t", outputEncoded); 
      printf("Output expected: %zu", numTest); 
      exit(-1);
    }
    
    switch(test_i){
      case UTF16_UTF8:
      case UTF32_UTF8:
        printf("Resulting utf-8 string: %s", utf8string);
        break;
      case UTF8_UTF16:
      case UTF32_UTF16:
        printf("Resulting utf-16 string (hexadecimal): ");
        for(int i = 0; i < numberutf16; i++){
          printf("%x-", utf16string[i]);
        }
        break;
      case UTF8_UTF32:
      case UTF16_UTF32:
        printf("Resulting utf-32 string (decimal): ");
        for(int i = 0; i < codepoints; i++){
          printf("%d-", utf32string[i]);
        }
        break;
    }
    
    printf("\n\n\n");
  }

  free(utf8string);
  free(utf16string);
  free(utf32string);
}



int main(int argc, char **argv){
  if(argc != 2){
    printf("Missing a string to test\n");
    exit(-1);
  }
  printf("string: %s\n", argv[1]);

  conversionInfo_t conver;
  InitializeConversion(&conver, BIG_ENDIAN);

  const charUTF8_t *strPT = (const charUTF8_t*)argv[1];
  mbsize_t utf8bytes;
  size_t numberOfCodepoints=1, numberOfBytes=1, utf16unitsNeeded=1;
  while(*strPT && !ConversionHasError(&conver)){
    utf8bytes = CharLength(strPT, &conver);
    strPT += utf8bytes;
    numberOfBytes += utf8bytes;
    if(utf8bytes == 4)
      utf16unitsNeeded += 2;
    else 
      ++utf16unitsNeeded;
    ++numberOfCodepoints;
  }

  if(ConversionHasError(&conver)){
    printf("there was an error counting characters\n");
    exit(-1);
  } 
  printf("number of characters: %zu\n", numberOfCodepoints);

  printf("Running conversions\n\n");
  strPT = (const charUTF8_t*)argv[1];
  testStrings(strPT, numberOfCodepoints, utf16unitsNeeded, numberOfBytes, &conver);

  exit(0);
}
