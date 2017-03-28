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
	lua_tinker::def(CLuaPub::LuaPtr(), "PrintArx", PrintArx);
}

void CLuaPub::PrintArx(const char* str)
{
	USES_CONVERSION;
	acutPrintf(_T("%s"),A2CT(str));
}
