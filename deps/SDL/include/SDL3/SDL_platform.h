/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2022 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

/**
 *  \file SDL_platform.h
 *
 *  Try to get a standard set of platform defines.
 */

#ifndef SDL_platform_h_
#define SDL_platform_h_

#if defined(_AIX)
#undef __AIX__
#define __AIX__     1
#endif
#if defined(__HAIKU__)
#undef __HAIKU__
#define __HAIKU__   1
#endif
#if defined(bsdi) || defined(__bsdi) || defined(__bsdi__)
#undef __BSDI__
#define __BSDI__    1
#endif
#if defined(_arch_dreamcast)
#undef __DREAMCAST__
#define __DREAMCAST__   1
#endif
#if defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(__DragonFly__)
#undef __FREEBSD__
#define __FREEBSD__ 1
#endif
#if defined(hpux) || defined(__hpux) || defined(__hpux__)
#undef __HPUX__
#define __HPUX__    1
#endif
#if defined(sgi) || defined(__sgi) || defined(__sgi__) || defined(_SGI_SOURCE)
#undef __IRIX__
#define __IRIX__    1
#endif
#if (defined(linux) || defined(__linux) || defined(__linux__))
#undef __LINUX__
#define __LINUX__   1
#endif
#if defined(ANDROID) || defined(__ANDROID__)
#undef __ANDROID__
#undef __LINUX__ /* do we need to do this? */
#define __ANDROID__ 1
#endif
#if defined(__NGAGE__)
#undef __NGAGE__
#define __NGAGE__ 1
#endif

#if defined(__APPLE__)
/* lets us know what version of macOS we're compiling on */
#include <AvailabilityMacros.h>
#include <TargetConditionals.h>

/* Fix building with older SDKs that don't define these
   See this for more information:
   https://stackoverflow.com/questions/12132933/preprocessor-macro-for-os-x-targets
*/
#ifndef TARGET_OS_MACCATALYST
#define TARGET_OS_MACCATALYST 0
#endif
#ifndef TARGET_OS_IOS
#define TARGET_OS_IOS 0
#endif
#ifndef TARGET_OS_IPHONE
#define TARGET_OS_IPHONE 0
#endif
#ifndef TARGET_OS_TV
#define TARGET_OS_TV 0
#endif
#ifndef TARGET_OS_SIMULATOR
#define TARGET_OS_SIMULATOR 0
#endif

#if TARGET_OS_TV
#undef __TVOS__
#define __TVOS__ 1
#endif
#if TARGET_OS_IPHONE
#undef __IOS__
#define __IOS__ 1
#else
#undef __MACOS__
#define __MACOS__  1
#if MAC_OS_X_VERSION_MIN_REQUIRED < 1070
# error SDL for macOS only supports deploying on 10.7 and above.
#endif /* MAC_OS_X_VERSION_MIN_REQUIRED < 1070 */
#endif /* TARGET_OS_IPHONE */
#endif /* defined(__APPLE__) */

#if defined(__NetBSD__)
#undef __NETBSD__
#define __NETBSD__  1
#endif
#if defined(__OpenBSD__)
#undef __OPENBSD__
#define __OPENBSD__ 1
#endif
#if defined(__OS2__) || defined(__EMX__)
#undef __OS2__
#define __OS2__     1
#endif
#if defined(osf) || defined(__osf) || defined(__osf__) || defined(_OSF_SOURCE)
#undef __OSF__
#define __OSF__     1
#endif
#if defined(__QNXNTO__)
#undef __QNXNTO__
#define __QNXNTO__  1
#endif
#if defined(riscos) || defined(__riscos) || defined(__riscos__)
#undef __RISCOS__
#define __RISCOS__  1
#endif
#if defined(__sun) && defined(__SVR4)
#undef __SOLARIS__
#define __SOLARIS__ 1
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
/* Try to find out if we're compiling for WinRT, GDK or non-WinRT/GDK */
#if defined(_MSC_VER) && defined(__has_include)
#if __has_include(<winapifamily.h>)
#define HAVE_WINAPIFAMILY_H 1
#else
#define HAVE_WINAPIFAMILY_H 0
#endif

/* If _USING_V110_SDK71_ is defined it means we are using the Windows XP toolset. */
#elif defined(_MSC_VER) && (_MSC_VER >= 1700 && !_USING_V110_SDK71_)    /* _MSC_VER == 1700 for Visual Studio 2012 */
#define HAVE_WINAPIFAMILY_H 1
#else
#define HAVE_WINAPIFAMILY_H 0
#endif

#if HAVE_WINAPIFAMILY_H
#include <winapifamily.h>
#define WINAPI_FAMILY_WINRT (!WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP))
#else
#define WINAPI_FAMILY_WINRT 0
#endif /* HAVE_WINAPIFAMILY_H */

#if WINAPI_FAMILY_WINRT
#undef __WINRT__
#define __WINRT__ 1
#elif defined(_GAMING_DESKTOP) /* GDK project configuration always defines _GAMING_XXX */
#undef __WINGDK__
#define __WINGDK__ 1
#elif defined(_GAMING_XBOX_XBOXONE)
#undef __XBOXONE__
#define __XBOXONE__ 1
#elif defined(_GAMING_XBOX_SCARLETT)
#undef __XBOXSERIES__
#define __XBOXSERIES__ 1
#else
#undef __WINDOWS__
#define __WINDOWS__ 1
#endif
#endif /* defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__) */

#if defined(__WINDOWS__)
#undef __WIN32__
#define __WIN32__ 1
#endif
/* This is to support generic "any GDK" separate from a platform-specific GDK */
#if defined(__WINGDK__) || defined(__XBOXONE__) || defined(__XBOXSERIES__)
#undef __GDK__
#define __GDK__ 1
#endif
#if defined(__PSP__)
#undef __PSP__
#define __PSP__ 1
#endif
#if defined(PS2)
#define __PS2__ 1
#endif

#if defined(__vita__)
#define __VITA__ 1
#endif

#if defined(__3DS__)
#undef __3DS__
#define __3DS__ 1
#endif

#include <SDL3/SDL_begin_code.h>
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get the name of the platform.
 *
 * Here are the names returned for some (but not all) supported platforms:
 *
 * - "Windows"
 * - "macOS"
 * - "Linux"
 * - "iOS"
 * - "Android"
 *
 * \returns the name of the platform. If the correct platform name is not
 *          available, returns a string beginning with the text "Unknown".
 *
 * \since This function is available since SDL 3.0.0.
 */
extern DECLSPEC const char * SDLCALL SDL_GetPlatform (void);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#include <SDL3/SDL_close_code.h>

#endif /* SDL_platform_h_ */
