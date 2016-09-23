#pragma once

#ifdef _MSC_VER
#define HAVE_VISUAL_STUDIO 1
#ifdef __cplusplus_winrt
#define HAVE_WINRT 1
#else
#define HAVE_WINRT 0
#endif
#define INCLUDE_PCH_H 0
#endif

// submodules help:
// http://johnleach.co.uk/words/323/git-submodules-in-n-easy-steps