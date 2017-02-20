#ifndef input_h_
#define input_h_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <map>
#include <string>
using std::map;
using std::string;

namespace air
{
	enum { LEFT_BUTTON = 0, RIGHT_BUTTON = 1, MIDDLE_BUTTON = 2 };

	#define KEY_DOWN(x)		g_p_input->key_down(x)
	#define KEY_UP(x)		g_p_input->key_up(x)
	#define MOUSE_DOWN(x)	g_p_input->mouse_down(x)
	#define MOUSE_UP(x)		g_p_input->mouse_up(x)

	class CInput
	{
    private:
        enum   { KEY_NUM = 256, MOUSE_NUM = 3 };
		HWND    m_wnd;
		float	      m_x, m_y, m_wheel;
		short	  m_mouse[MOUSE_NUM];
		short	  m_last_mouse[MOUSE_NUM];
		unsigned char	m_key[KEY_NUM];
		unsigned char	m_last_key[KEY_NUM];

		typedef map<string, unsigned int>   KeyNameMap;
		KeyNameMap m_name_to_key;

	public:
		CInput(HWND wnd);
		~CInput();
		//	更新设备状态
		void update();
		//	查询输入设备状态
		//	key：	键盘
		//	button：鼠标
		bool key_down(unsigned int key) const;
		bool key_up(unsigned int key) const;
		bool mouse_down(unsigned int button) const;
		bool mouse_up(unsigned int button) const;
		float get_mouse_x() const;
		float get_mouse_y() const;
		float get_mouse_wheel() const;
		//	根据名字查询输入状态
		//	name：	名字
		bool key_down_by_name(string name) const;
		bool key_up_by_name(string name) const;
        // 得到某一时刻按下的某个按键
        // 返回值：键值
		int get_key_down() const;
		int get_key_up() const;
	private:
		void _bulid_key_name_table();
		CInput();
		CInput(const CInput &r);
		CInput& operator=(const CInput &r);
	};
	extern CInput* g_p_input;
}

#endif
