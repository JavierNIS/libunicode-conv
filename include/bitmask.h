#ifndef _SIMPLE_BITMASK_
#define _SIMPLE_BITMASK_

#include <stdint.h>

typedef uint32_t uc_flags32_t;

typedef uint8_t uc_flags8_t;

#define FLAG_ONE 1U
#define FLAG_TWO (1U << 1)
#define FLAG_THREE (1U << 2)
#define FLAG_FOUR (1U << 3)
#define FLAG_FIVE (1U << 4)
#define FLAG_SIX (1U << 5)
#define FLAG_SEVEN (1U << 6)
#define FLAG_EIGHT (1U << 7)
#define FLAG_NINE (1U << 8)
#define FLAG_TEN (1U << 9)
#define FLAG_ELEVEN (1U << 10)
#define FLAG_TWELVE (1U << 11)
#define FLAG_THIRTEEN (1U << 12)
#define FLAG_FOURTEEN (1U << 13)
#define FLAG_FIFTEEN (1U << 14)
#define FLAG_SIXTEEN (1U << 15)
#define FLAG_SEVENTEEN (1U << 16)
#define FLAG_EIGHTEEN (1U << 17)
#define FLAG_NINETEEN (1U << 18)
#define FLAG_TWENTY (1U << 19)
#define FLAG_TWENTY_ONE (1U << 20)
#define FLAG_TWENTY_TWO (1U << 21)
#define FLAG_TWENTY_THREE (1U << 22)
#define FLAG_TWENTY_FOUR (1U << 23)
#define FLAG_TWENTY_FIVE (1U << 24)
#define FLAG_TWENTY_SIX (1U << 25)
#define FLAG_TWENTY_SEVEN (1U << 26)
#define FLAG_TWENTY_EIGHT (1U << 27)
#define FLAG_TWENTY_NINE (1U << 28)
#define FLAG_THIRTY (1U << 29)
#define FLAG_THIRTY_ONE (1U << 30)
#define FLAG_THIRTY_TWO (1U << 31)

#define ALL_FLAGS_FALSE 0x0
#define ALL_FLAGS_TRUE 0xFFFFFFFF

static inline void clearFlagsDouble(uc_flags32_t *bitmask){
  *bitmask &= ALL_FLAGS_FALSE;
}

static inline void copyFlagsDouble(uc_flags32_t *originalBitmask, uc_flags32_t copyBitmask){
  *originalBitmask = copyBitmask;
}

static inline void orFlagsDouble(uc_flags32_t *originalBitmask, uc_flags32_t orBitmask){
  *originalBitmask |= orBitmask;
}

static inline void setFlagDouble(uc_flags32_t *originalBitmask, uc_flags32_t flags){
  *originalBitmask |= flags;
}

static inline void unsetFlagDouble(uc_flags32_t *originalBitmask, uc_flags32_t flags){
  //quick way to set to 0 the flags we want to unset in the bitmask
  //if we want to unset the third bit, we set every other bit and then we do a & operation
  *originalBitmask &= (~flags);
}

static inline int areFlagsSet(uc_flags32_t originalBitmask, uc_flags32_t flags){
  return (originalBitmask & flags) == flags;
}

static inline int areFlagsUnset(uc_flags32_t originalBitmask, uc_flags32_t flags){
  return (originalBitmask & flags) != flags;
}



static inline void clearFlagsByte(uc_flags8_t *bitmask){
  *bitmask &= ALL_FLAGS_FALSE;
}

static inline void copyFlagsByte(uc_flags8_t *originalBitmask, uc_flags8_t copyBitmask){
  *originalBitmask = copyBitmask;
}

static inline void orFlagsByte(uc_flags8_t *originalBitmask, uc_flags8_t orBitmask){
  *originalBitmask |= orBitmask;
}

static inline void setFlagByte(uc_flags8_t *originalBitmask, uc_flags8_t flags){
  *originalBitmask |= flags;
}

static inline void unsetFlagByte(uc_flags8_t *originalBitmask, uc_flags8_t flags){
  *originalBitmask &= (~flags);
}

static inline int areFlagsSetByte(uc_flags8_t originalBitmask, uc_flags8_t flags){
  return (originalBitmask & flags) == flags;
}

static inline int areFlagsUnsetByte(uc_flags8_t originalBitmask, uc_flags8_t flags){
  return (originalBitmask & flags) != flags;
}


#endif

#ifndef _BITMASK_UNICODE
#define _BITMASK_UNICODE

#define NO_FAILURE_OCURRED FLAG_ONE
#define USING_BIG_ENDIAN FLAG_TWO
/*
#define  FLAG_THREE
#define  FLAG_FOUR
#define  FLAG_FIVE
#define  FLAG_SIX
#define  FLAG_SEVEN
#define  FLAG_EIGHT
#define  FLAG_NINE
#define  FLAG_TEN
#define  FLAG_ELEVEN
#define  FLAG_TWELVE
#define  FLAG_THIRTEEN
#define  FLAG_FOURTEEN
#define  FLAG_FIFTEEN
#define  FLAG_SIXTEEN
#define  FLAG_SEVENTEEN
#define  FLAG_EIGHTEEN
#define  FLAG_NINETEEN
#define  FLAG_TWENTY
#define  FLAG_TWENTY_ONE
#define  FLAG_TWENTY_TWO
#define  FLAG_TWENTY_THREE
#define  FLAG_TWENTY_FOUR
#define  FLAG_TWENTY_FIVE
#define  FLAG_TWENTY_SIX
#define  FLAG_TWENTY_SEVEN
#define  FLAG_TWENTY_EIGHT
#define  FLAG_TWENTY_NINE
#define  FLAG_THIRTY
#define  FLAG_THIRTY_ONE
#define  FLAG_THIRTY_TWO
*/
#endif
