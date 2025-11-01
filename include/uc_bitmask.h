#ifndef _BITMASK_UNICODE
#define _BITMASK_UNICODE

#include <bitmask.h>

typedef bitmask32_t uc_bitmask32_t;

enum {
  UC_BM_ERROR_OCURRED = BITMASK_BIT(0), //Gives a general error, allows for quick checks
  UC_BM_MALFORMED_STRINGS = BITMASK_BIT(1), //once strong string validation is implemented this 
                                            //code error might appear for malformed strings
  UC_BM_DEST_TOO_SHORT = BITMASK_BIT(2), //This error might appear when converting strings,
                                         //but itself is an error that might happen per character
  UC_BM_CONVERSION_BIG_ENDIAN = BITMASK_BIT(11), //Set the endianness which is used in conversions
  UC_BM_AUTODETECT_ENDIANNESS = BITMASK_BIT(22) //If enabled, will lookup endianness
                                                //at the start of strings, modifying the default
                                                //if a BOM is detected
};

#define LITTLE_ENDIAN 0
#define BIG_ENDIAN 1

#define SET_BIT 1
#define UNSET_BIT 0

#endif
