#ifndef _SIMPLE_BITMASK_
#define _SIMPLE_BITMASK_

#include <stdint.h>

typedef uint32_t bitmask32_t;

typedef uint8_t bitmask8_t;

//quick way to define each bit, instead of painstakingly doing it one by one
#define BITMASK_BIT(n) ((bitmask32_t)1u << (uint32_t)(n))

#define BITMASK32_NONE ((bitmask32_t)0u)
#define BITMASK32_ALL (~(bitmask32_t)0u)



static inline void Bitmask32Clear(bitmask32_t *bitmask){
  *bitmask = BITMASK32_NONE;
}

static inline void Bitmask32Copy(bitmask32_t *originalBitmask, bitmask32_t copyMask){
  *originalBitmask = copyMask;
}

static inline void Bitmask32Or(bitmask32_t *originalBitmask, bitmask32_t orMask){
  *originalBitmask |= orMask;
}

static inline void Bitmask32Set(bitmask32_t *originalBitmask, bitmask32_t setMask){
  *originalBitmask |= setMask;
}

static inline void Bitmask32Unset(bitmask32_t *originalBitmask, bitmask32_t unsetMask){
  //quick way to set to 0 the mask we want to unset in the bitmask
  //if we want to unset the third bit, we set every other bit and then we do a & operation
  *originalBitmask &= (~unsetMask);
}


static inline int Bitmask32AllSet(bitmask32_t originalBitmask, bitmask32_t mask){
  return (originalBitmask & mask) == mask;
}

static inline int Bitmask32AnySet(bitmask32_t originalBitmask, bitmask32_t mask){
  return (originalBitmask & mask) != 0u;
}

static inline int Bitmask32AllUnset(bitmask32_t originalBitmask, bitmask32_t mask){
  return (originalBitmask & mask) == 0u;
}


#define BITMASK8_NONE ((bitmask8_t)0u)
#define BITMASK8_ALL (~(bitmask8_t)0u)


static inline void Bitmask8Clear(bitmask8_t *bitmask){
  *bitmask = BITMASK8_NONE;
}

static inline void Bitmask8Copy(bitmask8_t *originalBitmask, bitmask8_t copyMask){
  *originalBitmask = copyMask;
}

static inline void Bitmask8Or(bitmask8_t *originalBitmask, bitmask8_t orMask){
  *originalBitmask |= orMask;
}

static inline void Bitmask8Set(bitmask8_t *originalBitmask, bitmask8_t setMask){
  *originalBitmask |= setMask;
}

static inline void Bitmask8Unset(bitmask8_t *originalBitmask, bitmask8_t unsetMask){
  *originalBitmask &= (~unsetMask);
}

static inline int Bitmask8AllSet(bitmask8_t originalBitmask, bitmask8_t mask){
  return (originalBitmask & mask) == mask;
}

static inline int Bitmask8AnySet(bitmask8_t originalBitmask, bitmask8_t mask){
  return (originalBitmask & mask) != 0u;
}

static inline int Bitmask8AllUnset(bitmask8_t originalBitmask, bitmask8_t mask){
  return (originalBitmask & mask) == 0u;
}


#endif
