#ifndef system_h_
#define system_h_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
using std::string;

namespace air
{
	typedef void (*callback)();
	class CTimer;
	class CSystem
	{
	private:
		HWND m_hwnd;
		HDC m_hdc;
		bool m_paused;
		callback m_p_init_func;
		callback m_p_frame_func;
		callback m_p_draw_func;
		callback m_p_render_func;
		unsigned int m_fixed_delta;
		unsigned int m_win_width, m_win_height;
		string m_title;
	public:
	    // 构造窗口
	    // w，h：宽，高
	    // t：标题
		CSystem(unsigned int w, unsigned int h, const char* t);
		~CSystem();
        // 设置回调函数
        // option：	设置选项
        // p_func：	回调函数
		enum Func { INIT, FRAME, DRAW, RENDER };
		void set_callback(Func option, callback p_func);
        // 主循环
		void run();
        // 处理标准windows消息
		LRESULT msg_proc(UINT msg, WPARAM wparam, LPARAM lparam);
        // 得到DC
		HDC getDC() const;
	public:
	    void _create_window();
	    void _release();
	    void _err();
	    CSystem();
		CSystem(const CSystem&);
		CSystem& operator=(const CSystem&);
	};
	extern CSystem* g_p_system;
}

#endif
