#ifndef _MBSTATE_
#define _MBSTATE_
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
#define BIG_ENDIAN 1 //Now endianness can be distinguished, big endian is the 
                     //default
#define LITTLE_ENDIAN 0 
#include <bitmask.h>

typedef struct{
  void* _mbchar;
  uc_flags8_t _flags;
} conversionInfo_t;

static inline void InitializeConversion(conversionInfo_t* conver){
  conver->_mbchar = 0;
  clearFlagsByte(&conver->_flags);
  setFlagByte(&conver->_flags, NO_FAILURE_OCURRED | USING_BIG_ENDIAN);
}

static inline void SetError(conversionInfo_t* conver, void* badmb){
  conver->_mbchar = badmb;
  unsetFlagByte(&conver->_flags, NO_FAILURE_OCURRED);
}

static inline void SetEndianness(conversionInfo_t* conver, const uint8_t endianness){
  if(endianness == BIG_ENDIAN)
    setFlagByte(&conver->_flags, USING_BIG_ENDIAN);
  else if(endianness == LITTLE_ENDIAN)
    unsetFlagByte(&conver->_flags, USING_BIG_ENDIAN);
}

static inline void ResetConversion(conversionInfo_t* conver){
  setFlagByte(&conver->_flags, NO_FAILURE_OCURRED);
}


#endif /*_MBSTATE*/
