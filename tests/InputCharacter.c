#include "UTF16.h"
#include "UTF8.h"
#include "mbstate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[]) {
  int size=0;
  charUTF8_t character[5]={0};
  conversionInfo_t conver;
  
  if(argc == 2 && strcmp("LE", argv[1]) == 0){
    InitializeConversion(&conver, LITTLE_ENDIAN);
    printf("Using little endian\n\n");
  }else{
    printf("Using big endian\n\n");
    InitializeConversion(&conver, BIG_ENDIAN);
  }

  printf("Introduce the number of bytes:\n");
  do{
    scanf("%d", &size);
  }while (size <= 0 || size > 4);


  printf("Input the character:\n");
  scanf("%4s", character);

  int real_size = CharLength(character, &conver);

  printf("For the character: %s\n", character);
  printf("The real size is: %d\n\n", real_size);

  charUTF16_t* utf16character = 0;
  if(real_size == 4)
    utf16character = (charUTF16_t*)malloc(2*sizeof(charUTF16_t));
  else
    utf16character = (charUTF16_t*)malloc(sizeof(charUTF16_t));

  printf("Converting into UTF16...\n\n");

  if(UTF8toUTF16(character, utf16character, &conver, size) != real_size){
    printf("There was a problem trying to convert to UTF-16\n");
    return -1;
  }
  printf("In hexadecimal: ");
  if(real_size == 4)
    printf("%x %x", utf16character[0], utf16character[1]);
  else
    printf("%x", *utf16character);
  printf("\nThe size of the character in UTF-16 is: %hhu\n", CharLength16(utf16character, &conver));
  printf("\n\n");

  charUTF32_t utf32caracter;

  printf("Converting into UTF-32...\n\n");

  if(UTF8toUTF32(character, &utf32caracter, &conver, size) != real_size){
    printf("There was a problem trying to convert to UTF-32\n");
    return -1;
  }

  printf("In hexadecimal: %x\n", utf32caracter);
  printf("character: %d\n\n", utf32caracter);

  free(utf16character);
  return 0;
}
