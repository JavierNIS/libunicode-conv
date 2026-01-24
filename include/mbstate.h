#ifndef _MBSTATE_
#define _MBSTATE_
#include "uc_bitmask.h"
#include "endianness.h"

/* Using a bitmask so it can easily be scaled

#define OK 1 //just a rename in case something fails
#define BAD 0
*/

typedef struct{
  void* context;
  uc_bitmask32_t _flags;
} conversionInfo_t;

//init the struct
static inline void InitializeConversion(conversionInfo_t* conver, int endianness){
  conver->context = 0;
  Bitmask32Clear(&conver->_flags);
  //Get the local endianness
  if(CheckEndianness()){
    Bitmask32Set(&conver->_flags, UC_BM_CPU_ENDIANNESS_BIG_ENDIAN);
  }

  //Checks what is the desired endianness
  if(endianness == BIG_ENDIAN)
    Bitmask32Set(&conver->_flags, UC_BM_CONVERSION_BIG_ENDIAN);
  else if(endianness == SAME_ENDIANNESS_AS_CPU){
    if(Bitmask32AnySet(conver->_flags, UC_BM_CPU_ENDIANNESS_BIG_ENDIAN))
      Bitmask32Set(&conver->_flags, UC_BM_CONVERSION_BIG_ENDIAN);
  }

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
  Bitmask32Unset(&conver->_flags, 0x7);
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

static inline void SetToLocalEndianness(conversionInfo_t *conver){
  if(Bitmask32AnySet(conver->_flags, UC_BM_CPU_ENDIANNESS_BIG_ENDIAN))
    Bitmask32Set(&conver->_flags, UC_BM_CONVERSION_BIG_ENDIAN);
  else
    Bitmask32Unset(&conver->_flags, UC_BM_CONVERSION_BIG_ENDIAN);
}

static inline int ConversionWithLittleEndian(const conversionInfo_t * conver){
  return !Bitmask32AnySet(conver->_flags, UC_BM_CONVERSION_BIG_ENDIAN);
}

#endif /*_MBSTATE*/
