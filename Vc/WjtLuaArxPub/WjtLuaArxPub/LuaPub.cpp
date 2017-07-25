#include "StdAfx.h"
#include "LuaPub.h"

CLuaPub::CLuaPub(void)
{
	L.Open();
	LoadArxLua();
}

CLuaPub::~CLuaPub(void)
{
}

CString CLuaPub::charToTCHAR(const char* str)
{
#if _MSC_VER >= 1400
	;
#else
	return CString(str);
#endif 
	//计算pChar所指向的多字节字符串相当于多少个宽字节
	DWORD num=MultiByteToWideChar(CP_ACP, 0, str, -1,NULL,0);

	wchar_t* pWCHAR = (wchar_t*)malloc(num * sizeof(wchar_t));

	if (pWCHAR==NULL)
	{
		return CStringW();
	}

	memset(pWCHAR,0,num * sizeof(wchar_t));

	//多字节转换为宽字节
	MultiByteToWideChar(CP_ACP, 0, str, -1, pWCHAR, num);

	CStringW strRec(pWCHAR);
	if (pWCHAR != NULL)
	{
		free(pWCHAR);
	}
	return strRec;
}

CStringA CLuaPub::TCHARTochar(const TCHAR* str)
{
#if _MSC_VER >= 1400
	;
#else
return CStringA(str);
#endif 

	//计算需要多少个字节才能表示对应的多字节字符串
	DWORD num = WideCharToMultiByte(CP_ACP,0,str,-1,NULL,0,NULL,0);

	//开辟空间
	char *pChar = NULL;
	pChar = (char*)malloc(num * sizeof(char));

	if (pChar==NULL)
	{
		return CStringA();
	}

	memset(pChar,0,num * sizeof(char));

	//将宽字节字符串转换为多字节字符串
	WideCharToMultiByte(CP_ACP, 0, str, -1, pChar, num, NULL, 0);

	CStringA strRec(pChar);
	if (pChar != NULL)
	{
		free(pChar);
	}
	return strRec;
}

lua_State* CLuaPub::LuaPtr()
{
	static CLuaPub g_single;
	return g_single.L.Object();
}

void CLuaPub::DoLua(const CString& str)
{
	lua_tinker::dofile(CLuaPub::LuaPtr(), TCHARTochar(str));
}

void CLuaPub::LoadArxLua()
{
	lua_State* PL = CLuaPub::LuaPtr();

	lua_tinker::def(PL, "charToTCHAR", charToTCHAR);
	lua_tinker::def(PL, "TCHARTochar", TCHARTochar);

	lua_tinker::def(PL, "PrintArx", PrintArx);
	lua_tinker::def(PL, "PrintArxLn", PrintArxLn);

	Register_Point();

}

void CLuaPub::PrintArx(const char* str)
{
	USES_CONVERSION;
	acutPrintf(_T("%s"),A2CT(str));
}

void CLuaPub::PrintArxLn(const char* str)
{
	USES_CONVERSION;
	acutPrintf(_T("\n%s"),A2CT(str));
}

void CLuaPub::Register_Point()
{
	lua_State* PL = CLuaPub::LuaPtr();

	lua_tinker::set(PL, "CgeTol", &AcGeContext::gTol);

	// 注册点类
	lua_tinker::class_add<AcGePoint3d>(PL, "AcGePoint3d");
	lua_tinker::class_con<AcGePoint3d>(PL, lua_tinker::constructor<AcGePoint3d>);
	lua_tinker::class_con<AcGePoint3d>(PL, lua_tinker::constructor<AcGePoint3d, double, double, double>);
	// lua_tinker::class_con<AcGePoint3d>(PL, lua_tinker::constructor<AcGePoint3d, double, double, double>);
	lua_tinker::class_mem<AcGePoint3d>(PL, "x", &AcGePoint3d::x);
	lua_tinker::class_mem<AcGePoint3d>(PL, "y", &AcGePoint3d::y);
	lua_tinker::class_mem<AcGePoint3d>(PL, "z", &AcGePoint3d::z);
	lua_tinker::class_mem<AcGePoint3d>(PL, "z", &AcGePoint3d::z);

	// lua_tinker::class_def<AcGePoint3d>(PL, "kOrigin", AcGePoint3d::kOrigin);

	lua_tinker::class_def<AcGePoint3d>(PL, "distanceTo", &AcGePoint3d::distanceTo);
	lua_tinker::class_def<AcGePoint3d>(PL, "isEqualTo", &AcGePoint3d::isEqualTo);
	// lua_tinker::class_mem<stuPortData>(L, "m_dAngle", &stuPortData::m_dAngle);
	//
}