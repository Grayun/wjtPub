#include "../air.h"	  // 扩展接口时需要大部分部件
#include "script.h"
#include <stdarg.h>
#include <string>
using std::string;

namespace air
{
	CScript* g_p_script = NULL;
	CScript::CScript()
	{
		m_L = lua_open();
		luaL_openlibs(m_L);	// 打开所有的lua标准库
		_register_func();	// 注册功能函数
	}
	CScript::~CScript()
	{
		lua_close(m_L);
		m_L = NULL;
	}
	void CScript::load(const char* p_file)
	{
		if (luaL_loadfile(m_L, p_file) || lua_pcall(m_L, 0, 0, 0))
			_err("CScript::init error:%s", lua_tostring(m_L, -1));
	}
	void CScript::call(const char* p_fun_name)
	{
		char name[1024];
		sprintf(name, p_fun_name);

		lua_getglobal(m_L, name);
		if (lua_isnil(m_L, -1) == 1)	// 检查方法是否为 nil
			return;
		if (lua_pcall(m_L, 0, 0, 0) != 0)
			_err("CScript::call error:%s", lua_tostring(m_L, -1));
	}
	void CScript::call(const char* p_fun_name, const char* p_params, ...)
	{
		va_list vl;
		va_start(vl, p_params);
		lua_getglobal(m_L, p_fun_name);
		if (lua_isnil(m_L, -1) == 1)	// 检查方法是否为 nil
			return;
		int arg_num = 0;
		while (*p_params)
		{
			switch (*p_params++)
			{
			case 'd':
				lua_pushnumber(m_L, va_arg(vl, double));
				break;
			case 'i':
				lua_pushnumber(m_L, va_arg(vl, int));
				break;
			case 's':
				lua_pushstring(m_L, va_arg(vl, char*));
				break;
			default:
				_err("CScript::call error:invalid param type option!");
				break;
			}
			luaL_checkstack(m_L, 1, "CScript::call error:too many params!");
			arg_num++;
		}
		va_end(vl);
		if (lua_pcall(m_L, arg_num, 0, 0) != 0)
			_err("CScript::call error:%s", lua_tostring(m_L, -1));
	}
	void CScript::call(const char* p_func, int in_num, float* p_in, int out_num, float* p_out)
    {
        if (p_func == NULL || in_num > 5 || p_in == NULL || out_num > 5 || p_out == NULL)
            return;
        lua_getglobal(m_L, p_func);
		if (lua_isnil(m_L, -1) == 1)
			return;
        for (int in = 0; in < in_num; in++)
            lua_pushnumber(m_L, (float)p_in[in]);
        if (lua_pcall(m_L, in_num, out_num, 0) != 0)
        {
            _err("CScript::call error:%s", lua_tostring(m_L, -1));
            return;
        }
        int idx = -out_num;
        for (int out = 0; out < out_num; out++)
        {
            if (!lua_isnumber(m_L, idx))
                continue;
            p_out[out] = (float)lua_tonumber(m_L, idx);
            idx++;
        }
    }
	// lua api ////////////////////////////////////////////////////////////////////////
	static int L_draw_line(lua_State* L)
	{
		float start_x = (float)lua_tonumber(L, 1);
		float start_y = (float)lua_tonumber(L, 2);
		float end_x = (float)lua_tonumber(L, 3);
		float end_y = (float)lua_tonumber(L, 4);
		unsigned char r = (unsigned char)lua_tonumber(L, 5);
		unsigned char g = (unsigned char)lua_tonumber(L, 6);
		unsigned char b = (unsigned char)lua_tonumber(L, 7);
		float size = (float)lua_tonumber(L, 8);
		g_p_surface->draw_line(start_x, start_y, end_x, end_y, color(r, g, b), size);
		return 0;
	}
	static int L_draw_rect(lua_State* L)
	{
		float x = (float)lua_tonumber(L, 1);
		float y = (float)lua_tonumber(L, 2);
		float width = (float)lua_tonumber(L, 3);
		float height = (float)lua_tonumber(L, 4);
		unsigned char r = (unsigned char)lua_tonumber(L, 5);
		unsigned char g = (unsigned char)lua_tonumber(L, 6);
		unsigned char b = (unsigned char)lua_tonumber(L, 7);
		int ifill = lua_toboolean(L, 8);
		bool fill = false;
		if (ifill)
			fill = true;
		g_p_surface->draw_rect(x, y, width, height, color(r, g, b), fill);
		return 0;
	}
	static int L_draw_circle(lua_State* L)
	{
		float x = (float)lua_tonumber(L, 1);
		float y = (float)lua_tonumber(L, 2);
		float radius = (float)lua_tonumber(L, 3);
		unsigned char r = (unsigned char)lua_tonumber(L, 4);
		unsigned char g = (unsigned char)lua_tonumber(L, 5);
		unsigned char b = (unsigned char)lua_tonumber(L, 6);
		int ifill = lua_toboolean(L, 7);
		bool fill = false;
		if (ifill)
			fill = true;
		g_p_surface->draw_circle(x, y, radius, color(r, g, b), fill);
		return 0;
	}
	static int L_draw_sprite(lua_State* L)
	{
		const char* p_name = lua_tostring(L, 1);
		float x = (float)lua_tonumber(L, 2);
		float y = (float)lua_tonumber(L, 3);
		float tex_x = (float)lua_tonumber(L, 4);
		float tex_y = (float)lua_tonumber(L, 5);
		float tex_width = (float)lua_tonumber(L, 6);
		float tex_height = (float)lua_tonumber(L, 7);
		g_p_surface->draw_sprite(p_name, x, y, tex_x, tex_y, tex_width, tex_height);
		return 0;
	}
	static int L_print(lua_State* L)
	{
		const char* p_name = lua_tostring(L, 1);
		float x = (float)lua_tonumber(L, 2);
		float y = (float)lua_tonumber(L, 3);
		unsigned char r = (unsigned char)lua_tonumber(L, 4);
		unsigned char g = (unsigned char)lua_tonumber(L, 5);
		unsigned char b = (unsigned char)lua_tonumber(L, 6);
		const char* p_str = lua_tostring(L, 7);
		g_p_pen_mgr->print(string(p_name), x, y, color(r, g, b), p_str);
		return 0;
	}
	static int L_key_down_by_name(lua_State* L)
	{
		const char* p_key = lua_tostring(L, 1);
		if (g_p_input->key_down_by_name(string(p_key)))
			lua_pushboolean(L, true);
		else
			lua_pushboolean(L, false);
		return 1;
	}
	static int L_key_up_by_name(lua_State* L)
	{
		const char* p_key = lua_tostring(L, 1);
		if (g_p_input->key_up_by_name(string(p_key)))
			lua_pushboolean(L, true);
		else
			lua_pushboolean(L, false);
		return 1;
	}
	static int L_play_music(lua_State* L)
	{
		const char* p_name = lua_tostring(L, 1);
		g_p_music->play(string(p_name));
		return 0;
	}
	static int L_pause_music(lua_State* L)
	{
		const char* p_name = lua_tostring(L, 1);
		g_p_music->pause(string(p_name));
		return 0;
	}
	static int L_proceed_music(lua_State* L)
	{
		const char* p_name = lua_tostring(L, 1);
		g_p_music->proceed(string(p_name));
		return 0;
	}
	static int L_debug(lua_State* L)
	{
		const char* p = lua_tostring(L, 1);
		printf(p);
		printf("\n");
		return 0;
	}
	static int L_quit(lua_State* L)
	{
		PostQuitMessage(0);
		return 0;
	}
	// lua api ////////////////////////////////////////////////////////////////////////

	void CScript::_register_func()
	{
		luaL_Reg lib[] =
		{
			{ "surface_draw_line", L_draw_line },
			{ "surface_draw_rect", L_draw_rect },
			{ "surface_draw_circle", L_draw_circle },
			{ "surface_draw_sprite", L_draw_sprite },
			{ "pen_print", L_print },
			{ "key_down", L_key_down_by_name },
			{ "key_up", L_key_up_by_name },
			{ "music_play", L_play_music },
			{ "music_pause", L_pause_music },
			{ "music_proceed", L_proceed_music },
			{ "msg", L_debug },
			{ "quit", L_quit },
			{ NULL, NULL },
		};
		luaL_register(m_L, "air", lib);
	}
	void CScript::_err(const char* p_msg, ...)
	{
	    char buf[256];
        va_list args;
        va_start(args, p_msg);
        vsprintf(buf, p_msg, args);
        MessageBoxA(NULL, buf, "err", NULL);
        va_end(args);
	}
}
