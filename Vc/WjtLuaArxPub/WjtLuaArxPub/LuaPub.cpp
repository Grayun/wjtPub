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

lua_State* CLuaPub::LuaPtr()
{
	static CLuaPub g_single;
	return g_single.L.Object();
}

void CLuaPub::DoLua(const CString& str)
{
	USES_CONVERSION;
	lua_tinker::dofile(CLuaPub::LuaPtr(), T2CA(str));
}

void CLuaPub::LoadArxLua()
{
	lua_State* PL = CLuaPub::LuaPtr();

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


	// 注册点类,这样处理太简单，可以通过加载lib来实现。
	lua_tinker::class_add<AcGePoint3d>(PL, "AcGePoint3d");
	lua_tinker::class_con<AcGePoint3d>(PL, lua_tinker::constructor<AcGePoint3d>);
	lua_tinker::class_con<AcGePoint3d>(PL, lua_tinker::constructor<AcGePoint3d, double, double, double>);
	// lua_tinker::class_con<AcGePoint3d>(PL, lua_tinker::constructor<AcGePoint3d, double, double, double>);
	lua_tinker::class_mem<AcGePoint3d>(PL, "x", &AcGePoint3d::x);
	lua_tinker::class_mem<AcGePoint3d>(PL, "y", &AcGePoint3d::y);
	lua_tinker::class_mem<AcGePoint3d>(PL, "z", &AcGePoint3d::z);
	lua_tinker::class_mem<AcGePoint3d>(PL, "z", &AcGePoint3d::z);

	lua_tinker::class_def<AcGePoint3d>(PL, "distanceTo", &AcGePoint3d::distanceTo);
	lua_tinker::class_def<AcGePoint3d>(PL, "isEqualTo", &AcGePoint3d::isEqualTo);
	// lua_tinker::class_mem<stuPortData>(L, "m_dAngle", &stuPortData::m_dAngle);
	//
}