#ifndef UNICODE_VALID_CODEPOINTS
#define UNICODE_VALID_CODEPOINTS
#include <stdint.h> //for the ranges 

#ifndef UNICODE_CODEPOINT_RANGE
#define UNICODE_CODEPOINT_RANGE

typedef struct codePointRange{
  uint32_t start;
  uint32_t end;
} cpRange_t;

#endif /*UNICODE_CODEPOINT_RANGE*/

#ifndef UNICODE_PLANES_DEFINITION
#define UNICODE_PLANES_DEFINITION

extern const cpRange_t BMP_PLANE[];

//Plane 1 (Supplementary multilingual plane)
extern const cpRange_t SMP_PLANE[];

//Plane 2 (Supplementary Ideographic Plane)
extern const cpRange_t SIP_PLANE[];

//Plane 3 (Tertiary Ideographic Plane)
extern const cpRange_t TIP_PLANE[];

//Plane 4 to D havo no code points allocated

//Plane E (Supplementary Special-purpose Plane)
extern const cpRange_t SSP_PLANE[];

extern const uint32_t MAX_CODE_POINT;

//Planes F and 10 are for private use

#endif /*UNICODE_PLANES_DEFINITION*/

#endif /*UNICODE_VALID_CODEPOINTS*/
