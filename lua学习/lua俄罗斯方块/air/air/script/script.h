/*
	脚本引擎类，Lua实现
*/
#ifndef script_h_
#define script_h_

#include "lua/lua.hpp"

namespace air
{
	class CLog;
	class CScript
	{
	private:
		lua_State* m_L;
	public:
		CScript();
		~CScript();
		/*
			载入脚本
			p_file：	脚本文件
		*/
		void load(const char* p_file);
		/*
			调用脚本函数
			p_fun_name：	脚本函数名
			arg_num：		参数数量
		*/
		void call(const char* p_fun_name);
		/*
			带参数调用脚本函数
			p_fun_name：	脚本函数名
			p_params：		脚本函数参数
			调用实例：	call("fun_name", "ids", 10, 20.0, "abc")
		*/
		void call(const char* p_fun_name, const char* p_params, ...);
		// 调用脚本函数
		// p_func：脚本函数名
		// in_num：输入参数个数
		// p_in：输入参数数组
		// out_num：输出参数个数
		// p_out：输出参数数组
		void call(const char* p_func, int in_num, float* p_in, int out_num, float* p_out);
	private:
		void _register_func();
		void _err(const char* p_msg, ...);
		CScript(const CScript&);
		CScript& operator=(const CScript&);
	};
	extern CScript* g_p_script;
}

#endif
