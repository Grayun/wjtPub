#include "StdAfx.h"
#include "LuaPub.h"


CString CodePageToUnicode(int codePage, const char *src)
{
	CString strRec(_T(""));
	if (!src) return strRec;
	int srcLen = strlen(src);
	if (!srcLen)
	{
		wchar_t *w = new wchar_t[1];
		w[0] = 0;

		strRec = w;
		delete [] w;
		return strRec;
	}

	int requiredSize = MultiByteToWideChar(codePage,
		0,
		src,srcLen,0,0);

	if (!requiredSize)
	{
		return strRec;
	}

	wchar_t *w = new wchar_t[requiredSize+1];
	w[requiredSize] = 0;

	int retval = MultiByteToWideChar(codePage,
		0,
		src,srcLen,w,requiredSize);
	if (!retval)
	{
		delete [] w;
		return strRec;
	}

	strRec = w;
	delete [] w;

	return strRec;
}

CStringA UnicodeToCodePage(int codePage, const wchar_t *src)
{
	CStringA str("");

	if (!src) return str;
	int srcLen = wcslen(src);
	if (!srcLen)
	{
		char *x = new char[1];
		x[0] = '\0';
		str = x;
		delete [] x;
		return str;
	}

	int requiredSize = WideCharToMultiByte(codePage,
		0,
		src,srcLen,0,0,0,0);

	if (!requiredSize)
	{
		return str;
	}

	char *x = new char[requiredSize+1];
	x[requiredSize] = 0;

	int retval = WideCharToMultiByte(codePage,
		0,
		src,srcLen,x,requiredSize,0,0);
	if (!retval)
	{
		delete [] x;
		return str;
	}

	str = x;
	delete [] x;
	

	return str;
}

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

// 传入的字符要是ansi编码的
void CLuaPub::PrintArx(const char* str)
{
	// 传进来的是 utf-8的字符，却以ansi的格式显示的，故而需要将该字符串从uif-8转换为ansi

	USES_CONVERSION;
// 	CStringA strA = UnicodeToCodePage(65001, A2CT(str));
// 	CString strU = CodePageToUnicode(1252, strA);
	acutPrintf(_T("%s"),A2CT(str));
}

void CLuaPub::PrintArxLn(const char* str)
{
	CStringA strA;
	strA.Format("\n%s", str);

	PrintArx(strA);
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

#define MAXUNICODE	0x10FFFF
/*
** Decode one UTF-8 sequence, returning NULL if byte sequence is invalid.
*/
const char *CLuaPub::utf8_decode (const char *o, int *val) {
	static const unsigned int limits[] = {0xFF, 0x7F, 0x7FF, 0xFFFF};
	const unsigned char *s = (const unsigned char *)o;
	unsigned int c = s[0];
	unsigned int res = 0;  /* final result */
	if (c < 0x80)  /* ascii? */
		res = c;
	else {
		int count = 0;  /* to count number of continuation bytes */
		while (c & 0x40) {  /* still have continuation bytes? */
			int cc = s[++count];  /* read next byte */
			if ((cc & 0xC0) != 0x80)  /* not a continuation byte? */
				return NULL;  /* invalid byte sequence */
			res = (res << 6) | (cc & 0x3F);  /* add lower 6 bits from cont. byte */
			c <<= 1;  /* to test next bit */
		}
		res |= ((c & 0x7F) << (count * 5));  /* add first byte */
		if (count > 3 || res > MAXUNICODE || res <= limits[count])
			return NULL;  /* invalid byte sequence */
		s += count;  /* skip continuation bytes read */
	}
	if (val) *val = res;
	return (const char *)s + 1;  /* +1 to include first byte */
}

const char* CLuaPub::ansi_decode(const char* o, int* val)
{
	return 0;
}