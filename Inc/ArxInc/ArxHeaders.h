#pragma once


#pragma warning(disable: 4996 4819)

#if _MSC_VER == 1200	// vc6
#include "E:\CvsWork\3rdparty\R15ObjectARX 2002\inc\ArxMigrtion.h"
	
#endif

#if _MSC_VER == 1300   // vc7
	#include "E:\CvsWork\3rdparty\R16ObjectARX 2004\inc\arxHeaders.h"
	
#endif

#if _MSC_VER == 1400   // vc8
	#ifdef _WIN64
		#include "E:\CvsWork\3rdparty\R17ObjectARX 2008_64bit\inc\arxHeaders.h"
	
	#else
		#include "E:\CvsWork\3rdparty\R17ObjectARX 2008_32bit\inc\arxHeaders.h"
		
	#endif
#endif

#if _MSC_VER == 1500   // vc9
	#include "E:\CvsWork\3rdparty\R18ObjectARX 2010\inc\arxHeaders.h"
	
#endif

#if _MSC_VER == 1600   // vc10
	
	#ifdef _WIN64
		#include "E:\CvsWork\3rdparty\R19ObjectARX 2013\inc-x64\arxHeaders.h"
	#else
		#include "E:\CvsWork\3rdparty\R19ObjectARX 2013\inc-win32\arxHeaders.h"
	#endif
#endif