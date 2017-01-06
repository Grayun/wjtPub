#pragma once

//////////////////////////////////////////////////////////////////////////
#if _MSC_VER < 1200
	#define _HPLIB_VISUALSTUDIO_VERSION "50"
#elif _MSC_VER == 1200
	#define _HPLIB_VISUALSTUDIO_VERSION "60"
#elif _MSC_VER == 1300
	#define _HPLIB_VISUALSTUDIO_VERSION "70"
#elif _MSC_VER == 1310
	#define _HPLIB_VISUALSTUDIO_VERSION "71"
#elif _MSC_VER == 1400
	#define _HPLIB_VISUALSTUDIO_VERSION "80"
#elif _MSC_VER == 1500
	#define _HPLIB_VISUALSTUDIO_VERSION "90"
#elif _MSC_VER == 1600
#define _HPLIB_VISUALSTUDIO_VERSION "100"
#else
	#define _HPLIB_VISUALSTUDIO_VERSION ""
#endif
//////////////////////////////////////////////////////////////////////////
#ifdef _WIN64
	#define _HPLIB_PLATFORM "x64"
#else
	#define _HPLIB_PLATFORM ""
#endif
//////////////////////////////////////////////////////////////////////////
#if defined(_DEBUG)

#if defined(HP_FULL_DEBUG)

	#if defined(_UNICODE)
		#define _HPLIB_PROJECT_CONFIGURATION "UDF"
	#else
		#define _HPLIB_PROJECT_CONFIGURATION "DF"
	#endif // _UNICODE
#else
	#if defined(_UNICODE)
	#define _HPLIB_PROJECT_CONFIGURATION "UD"
	#else
	#define _HPLIB_PROJECT_CONFIGURATION "D"
	#endif // _UNICODE
#endif

#else
	#if defined(_UNICODE)
		#define _HPLIB_PROJECT_CONFIGURATION "U"
	#else
		#define _HPLIB_PROJECT_CONFIGURATION ""
	#endif // _UNICODE
#endif // _DEBUG
//////////////////////////////////////////////////////////////////////////