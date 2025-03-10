#include "codepoints.h"

// Plane 0 (Basic multilingual plane)
const cpRange_t BMP_PLANE[] = {
  //Latin, non-latin european, african, asian, CJK characters, indonesian
  //oceanic, american, notational, symbols...
  {0x0000, 0xD7FF},
  //CJK characters, non-latin european, middle eastern, east and southwest asian
  {0xF900, 0xFFFF}

  //From 0xd800 to 0xdfff the code points are reserved for utf-16
  //From 0xe000 to 0xf8ff the code points are for private use
};

//Plane 1 (Supplementary multilingual plane)
const cpRange_t SMP_PLANE[] = {
  {0x10000, 0x101CF},
  {0x10280, 0x103DF},
  {0x10400, 0x107BF},
  {0x10800, 0x108AF},
  {0x108E0, 0x1093F},
  {0x10980, 0x10A9F},
  {0x10AC0, 0x10BAF},
  {0x10C00, 0x10C4F},
  {0x10C80, 0x10D8F},
  {0x10E60, 0x1124F},
  {0x11280, 0x114DF},
  {0x11580, 0x1174F},
  {0x11800, 0x1184F},
  {0x118A0, 0x1195F},
  {0x119A0, 0x11B5F},
  {0x11BC0, 0x11CBF},
  {0x11D00, 0x11DAF},
  {0x11EE0, 0x11F5F},
  {0x11FB0, 0x1254F},
  {0x12F90, 0x1467F},
  {0x16100, 0x1613F},
  {0x16800, 0x16B8F},
  {0x16D40, 0x16D7F},
  {0x16E40, 0x16F9F},
  {0x16FE0, 0x18D7F},
  {0x1AFF0, 0x1B2FF},
  {0x1BC00, 0x1BCAF},
  {0x1CC00, 0x1CEBF},
  {0x1CF00, 0x1CFCF},
  {0x1D000, 0x1D24F},
  {0x1D2C0, 0x1D37F},
  {0x1D400, 0x1DAAF},
  {0x1DF00, 0x1E08F},
  {0x1E100, 0x1E14F},
  {0x1E290, 0x1E2FF},
  {0x1E4D0, 0x1E4FF},
  {0x1E5D0, 0x1E5FF},
  {0x1E7E0, 0x1E8DF},
  {0x1E900, 0x1E95F},
  {0x1EC70, 0x1ECBF},
  {0x1ED00, 0x1ED4F},
  {0x1EE00, 0x1EEFF},
  {0x1F000, 0x1FBFF}
};

//Plane 2 (Supplementary Ideographic Plane)
const cpRange_t SIP_PLANE[] = {
  //CJK Unified Ideographs B
  {0x20000, 0x2A6DF},
  //CJK Unified Ideographs C, D, E, F, I
  {0x2A700, 0x2EE5F},
  //CJK Unified Ideographs Supplement
  {0x2F800, 0x2FA1F}
  //The rest of code points are not allocated
};

//Plane 3 (Tertiary Ideographic Plane)
const cpRange_t TIP_PLANE[] = {
  //CJK Unified Ideographs G, H
  {0x30000, 0x323AF}
  //The rest of code points are not allocated
};

//Plane 4 to D havo no code points allocated

//Plane E (Supplementary Special-purpose Plane)
const cpRange_t SSP_PLANE[] = {
  //Tags
  {0xE0000, 0xE007F},
  //Variation Selectors Supplement
  {0xE0100, 0xE01EF}
  //The rest of code points are not allocated
};

const uint32_t MAX_CODE_POINT = 0x10FFFF;
