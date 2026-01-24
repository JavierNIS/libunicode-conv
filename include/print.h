#ifndef _PRINT_UNICODE
#define _PRINT_UNICODE
#include "unicode_common.h"

static inline void write_bom(FILE *output, uint32_t output_big_endian){
  if(output_big_endian){
    fputc(0xFE, output);
    fputc(0xFF, output);
  }else{
    fputc(0xFF, output);
    fputc(0xFE, output);
  }
}


static inline size_t write_utf16_native(FILE *file, const charUTF16_t *buf, size_t units){
  return fwrite(buf, sizeof(charUTF16_t), units, file);
}

static inline size_t write_utf16_swapped(FILE *file, const charUTF16_t *buf, size_t units){
  size_t units_written = 0;
  for(size_t i = 0; i < units; i++){
    charUTF16_t u = buf[i];
    SwapEndiannessU16(&u);
    units_written += fwrite(&u, sizeof(charUTF16_t), 1, file);
  }
  return units_written;
}

static inline size_t write_utf32_native(FILE *file, const charUTF32_t *buf, size_t units){
  return fwrite(buf, sizeof(charUTF32_t), units, file);
}

static inline size_t write_utf32_swapped(FILE *file, const charUTF32_t *buf, size_t units){
  size_t units_written = 0;
  for(size_t i = 0; i < units; i++){
    charUTF32_t u = buf[i];
    SwapEndiannessU32(&u);
    units_written += fwrite(&u, sizeof(charUTF32_t), 1, file);
  }
  return units_written;
}

#endif
