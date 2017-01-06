#pragma once
#include "../../Inc/VisualStudioVersion.h"



//注意：下面的处理是为了用链接发行版的MFC库


#ifndef CAD_TIP
#define CAD_TIP

#pragma message ("    **********************  CAD无关公用函数库  **********************")
#endif

//////////////////////////////////////////////////////////////////////////
#if defined(_DEBUG) && !defined(HP_FULL_DEBUG)
#define _DEBUG_WAS_DEFINED_IN_OTHER
#undef _DEBUG
//#pragma message ("    ***注意： 在 WAFPubMFCdll 中使用 MFC 发行版库文件")
#endif
///////////////////////////////////////////

///////////////////////////////////////////
//1.恢复调试宏并去除临时宏的定义
#ifdef _DEBUG_WAS_DEFINED_IN_OTHER
#define _DEBUG
#undef _DEBUG_WAS_DEFINED_IN_OTHER
#endif
//////////////////////////////////////////////////////////////////////////
#if defined(_JT3D_UPDIR_)
#define _JT3D_PATHSTR__1 "..\\lib\\"
#elif defined(_JT3D_UPDIR_1)
#define _JT3D_PATHSTR__1 "..\\..\\..\\lib\\"
#elif defined(_JT3D_UPDIR_2)
#define _JT3D_PATHSTR__1 "..\\..\\..\\Jt3DPub_Dx9\\Jt3DSz\\lib\\"
#else
#define _JT3D_PATHSTR__1 "..\\..\\..\\..\\Jt3DPub_Dx9\\Jt3DSz\\lib\\"
#endif // _UNICODE
//////////////////////////////////////////////////////////////////////////
#undef WAFPUBMFCDLL_API
#ifdef _WAFPUBMFCDLL_EXPORT
#define WAFPUBMFCDLL_API _declspec(dllexport)
#else
#define WAFPUBMFCDLL_API _declspec(dllimport)	
#   if _MSC_VER < 1300	//6.0
#define _JT3D_PATHSTR__2 "vc60\\"
#   elif _MSC_VER == 1300	// 7.0
#define _JT3D_PATHSTR__2 "vc70\\"
#   elif _MSC_VER == 1400	// 8.0
#ifdef _WIN64
#define _JT3D_PATHSTR__2 "vc80x64\\"
#else
#define _JT3D_PATHSTR__2 "vc80\\"
#endif
#   elif	_MSC_VER == 1500// 9.0
#ifdef _WIN64
#define _JT3D_PATHSTR__2 "vc90x64\\"
#else
#define _JT3D_PATHSTR__2 "vc90\\"
#endif
#   else// 10.0
#ifdef _WIN64
#define _JT3D_PATHSTR__2 "vc100x64\\"
#else
#define _JT3D_PATHSTR__2 "vc100\\"
#endif
#   endif
//////////////////////////////////////////////////////////////////////////
#ifdef _WIN64
#pragma comment(lib,_JT3D_PATHSTR__1 _JT3D_PATHSTR__2 "WAFPubMFCdll" _HPLIB_VISUALSTUDIO_VERSION _HPLIB_PROJECT_CONFIGURATION "x64.lib")
#pragma message ("Automatically linking WAFPubMFCdll" _HPLIB_VISUALSTUDIO_VERSION _HPLIB_PROJECT_CONFIGURATION "x64.lib")
#else
#pragma comment(lib,_JT3D_PATHSTR__1 _JT3D_PATHSTR__2 "WAFPubMFCdll" _HPLIB_VISUALSTUDIO_VERSION _HPLIB_PROJECT_CONFIGURATION ".lib")
#pragma message ("Automatically linking WAFPubMFCdll" _HPLIB_VISUALSTUDIO_VERSION _HPLIB_PROJECT_CONFIGURATION ".lib")
#endif
#undef _JT3D_PATHSTR__2
//
#endif
//////////////////////////////////////////////////////////////////////////

#include "PropTree.h"
#include "PropTreeList.h"
#include "PropTreeInfo.h"

#include "PropTreeItem.h"
#include "PropTreeItemStatic.h"
#include "PropTreeItemEdit.h"
#include "PropTreeItemCombo.h"
#include "PropTreeItemColor.h"

// Jt3DBaseDialog
#include "Jt3DGeUiDialog/Jt3DGeUiDialog.h"

// Button
#include "Button/BtnST.h"
#include "Button/CJt3DUiPickButton.h"

// UltimateGrid
#include "UltimateGrid\UGCtrl.h"
#include "UltimateGrid\UGStrOp.h"
#include "UltimateGrid\UgGridBase.h"

#include "SuperEdit/SuperEdit.h"

// mfc相关公用函数
#include "MFCGlobleFun/MFCGlobleFunInc.h"
