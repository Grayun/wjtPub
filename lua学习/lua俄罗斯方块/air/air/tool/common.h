/*
	常用小函数
*/
#ifndef common_h_
#define common_h_

#include <string>
#include <sstream>
#include <cstdlib>
using std::string;

namespace air
{
	// 常用工具宏
	#define SAFE_DELETE(x)			{ if (x) { delete (x);		(x) = NULL; } }
	#define SAFE_DELETE_ARRAY(x)	{ if (x) { delete[] (x);	(x) = NULL; } }
	#define SAFE_FREE(x)			{ if (x) { free(x);			(x) = NULL; } }
	// 字符串转化为浮点数
	inline float str_to_float(string num)
	{
		return static_cast<float>(atof(num.c_str()));
	}
	inline float stf(string num)
	{
		return str_to_float(num);
	}
	// 字符串转化为整数
	inline int str_to_int(string num)
	{
		return atoi(num.c_str());
	}
	inline int sti(string num)
	{
		return str_to_int(num);
	}
	// 数据类型转化为字符串
    template<class T>
    std::string tostr(const T& r)
    {
        std::ostringstream oss;
        oss.precision(5);
        oss.setf(std::ios_base::fixed);
        oss << r;
        return oss.str();
    }

	// 数据类型
	typedef unsigned long	u32;
	typedef long			s32;
	typedef unsigned short	u16;
	typedef short			s16;
	typedef unsigned char	u8;
	typedef char			s8;
}

#endif
