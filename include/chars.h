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

#include "platform_integration.h"

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
