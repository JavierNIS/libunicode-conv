#ifndef PLATFORM_SPECIFICS
#define PLATFORM_SPECIFICS

#ifndef _WINDOWS_
#if defined(_WIN32) ||    \
  defined(_WIN64) ||      \
  defined(__MINGW32__) || \
  defined(__MINGW64__) || \
  defined(_MYSYS_) ||     \
  defined(__CYGWIN__) ||  \
  defined(__WINDOWS__)
#define _WINDOWS_
#endif
#endif /*_WINDOWS_*/

#endif /*PLATFORM_SPECIFICS*/