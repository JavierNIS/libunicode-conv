#include "mbstate.h"

void Initialize(conversionInfo_t* conver){
  conver->_mbchar = 0;
  conver->_state = OK;
  conver->_endianness = BIG_ENDIAN;
}

void SetError(conversionInfo_t* conver, void* badmb){
  conver->_mbchar=badmb;
  conver->_state=BAD;
}

void SetEndiannes(conversionInfo_t* conver, const uint8_t endiannes){
  if(endiannes == BIG_ENDIAN ||
      endiannes == LITTLE_ENDIAN)
    conver->_endianness=endiannes;
}

void Reset(conversionInfo_t* conver){
  conver->_state=OK;
}
