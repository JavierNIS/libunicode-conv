#include "UTF8.h"
#include "print.h"
#include <stdlib.h>

charUTF16_t *getUTF16_space_and_endianness(const charUTF8_t* src, conversionInfo_t *conv, FILE *out){
  mbsize_t utf8_size=0;
  size_t size_utf16=0;
  const charUTF8_t *ptr = src;
  //set to 1, this is to include the terminating character
  charUTF8_t last_c=1;

  while(last_c){
    utf8_size = CharLength(ptr,conv);
    if(utf8_size){
      if(utf8_size==4)
        size_utf16+=2;
      else size_utf16+=1;
    }else{
      printf("An error occurred while processing the string\n");
      exit(EXIT_FAILURE);
    }
    last_c=*ptr;
    ptr+=utf8_size;
  }
  charUTF16_t *dest = (charUTF16_t*)malloc(sizeof(charUTF16_t)*size_utf16);
  size_utf16 = UTF8stringToUTF16(src, dest, conv, size_utf16);
  if(!ConversionHasError(conv)){
    write_bom(out, Bitmask32AnySet(conv->_flags, UC_BM_CONVERSION_BIG_ENDIAN));
    write_utf16(out, dest, size_utf16-1, conv->_flags);
  }
  return dest;
}


int main(int argc, char *argv[]){
  if(argc < 3){
    printf("You need to pass at least a string and text_file\n");
    printf("Help: ./program string [--BE|--LE] output.txt\n");
    return EXIT_FAILURE;
  }
  FILE *output_file;
  const charUTF8_t *utf8_string = (const charUTF8_t*)argv[1];
  size_t current_arg=2;
  mbsize_t endianness = LITTLE_ENDIAN;

  while(current_arg < argc){
    if(current_arg == 2){
      if(strcmp("--BE", argv[2]) == 0)
        endianness = BIG_ENDIAN;
      else if(strcmp("--LE", argv[2]) == 0)
        endianness = LITTLE_ENDIAN;
      else{
        output_file = fopen(argv[2], "wb");
        if(!output_file){
          printf("Couldn't open file: %s\n", argv[2]);
          return EXIT_FAILURE;
        }
      }
    }else if(current_arg == 3 && !output_file){
      output_file = fopen(argv[3], "wb");
      if(!output_file){
        printf("Couldn't open file: %s\n", argv[3]);
        return EXIT_FAILURE;
      }
    }else{
      printf("Too many arguments, second argument is endianness or the file, endinness can't come later on\n");
      return EXIT_FAILURE;
    }
    ++current_arg;
  }
  if(!output_file){
    printf("Couldn't create the file conv16.out \n");
    return EXIT_FAILURE;
  }
  conversionInfo_t conv;
  InitializeConversion(&conv, endianness);
  
  printf("Converting %s\n", utf8_string);
  charUTF16_t *utf16_string = getUTF16_space_and_endianness(utf8_string, &conv, output_file);


  free(utf16_string);
  fclose(output_file);
  return EXIT_SUCCESS;
}
