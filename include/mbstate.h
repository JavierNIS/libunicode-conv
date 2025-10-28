#ifndef _MBSTATE_
#define _MBSTATE_
#include "bitmask.h"
#include "uc_bitmask.h"
#include <stdint.h> //for least types

#ifndef UTF8_CHAR
#define UTF8_CHAR
typedef uint_least8_t charUTF8_t; //utf8 char
#endif /*UTF8_CHAR*/

#ifndef UTF16_CHAR
#define UTF16_CHAR
typedef uint_least16_t charUTF16_t; //utf16 char
#endif /*UTF16_CHAR*/

#ifndef UTF32_CHAR
#define UTF32_CHAR
typedef uint_least32_t charUTF32_t; //utf32 char
#endif /*UTF32_CHAR*/

#ifndef WIDE_CHAR
#define WIDE_CHAR

#ifdef __linux__
typedef uint_least32_t widechar_t; //OS based char type
#endif /*__linux__*/

#ifdef _WINDOWS_
typedef uint_least16_t widechar_t;
#endif /*_WIN32*/
#endif /*WIDE_CHAR*/

#ifndef MB_CHAR_SIZE
#define MB_CHAR_SIZE
typedef uint8_t mbsize_t;
#endif


/* Using a bitmask so it can easily be scaled

#define OK 1 //just a rename in case something fails
#define BAD 0
*/
#include <uc_bitmask.h>

typedef struct{
  void* context;
  uc_bitmask32_t _flags;
} conversionInfo_t;

//init the struct
static inline void InitializeConversion(conversionInfo_t* conver, int defaultBigEndian){
  conver->context = 0;
  Bitmask32Clear(&conver->_flags);
  if(defaultBigEndian)
    Bitmask32Set(&conver->_flags, UC_BM_CONVERSION_BIG_ENDIAN);

}

//error related functions
static inline void SetError(conversionInfo_t* conver, void* badmb){
  conver->context = badmb;
  Bitmask32Set(&conver->_flags, UC_BM_ERROR_OCURRED);
}

static inline void SetMalformedStringError(conversionInfo_t *conver, void *badmb){
  Bitmask32Set(&conver->_flags, UC_BM_MALFORMED_STRINGS);
  SetError(conver, badmb);
}

static inline void ClearError(conversionInfo_t* conver){
  conver->context = 0;
  Bitmask32Unset(&conver->_flags, UC_BM_ERROR_OCURRED);
}

static inline int ConversionHasError(const conversionInfo_t *conver){
  return Bitmask32AnySet(conver->_flags, UC_BM_ERROR_OCURRED);
}

//endianness related functions
static inline void SetEndianness(conversionInfo_t* conver, const uint8_t endianness){
  if(endianness == BIG_ENDIAN)
    Bitmask32Set(&conver->_flags, UC_BM_CONVERSION_BIG_ENDIAN);
  else 
    Bitmask32Unset(&conver->_flags, UC_BM_CONVERSION_BIG_ENDIAN);
}

static inline void SetAutodetectEndianness(conversionInfo_t *conver, const uint8_t autodetect){
  if(autodetect)
    Bitmask32Set(&conver->_flags, UC_BM_AUTODETECT_ENDIANNESS);
  else
    Bitmask32Unset(&conver->_flags, UC_BM_AUTODETECT_ENDIANNESS);
}

static inline int ConversionWithLittleEndian(const conversionInfo_t * conver){
  return !Bitmask32AnySet(conver->_flags, UC_BM_CONVERSION_BIG_ENDIAN);
}


#endif /*_MBSTATE*/
