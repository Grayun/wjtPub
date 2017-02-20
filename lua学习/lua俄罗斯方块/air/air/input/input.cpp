#include "input.h"
namespace air
{
	CInput* g_p_input = NULL;
	CInput::CInput(HWND wnd)
	{
		m_wnd = wnd;
		m_x = m_y = m_wheel = 0.0f;
		memset(&m_mouse, 0, sizeof(m_mouse));
		memset(&m_last_mouse, 0, sizeof(m_last_mouse));
		memset(&m_key, 0, sizeof(m_key));
		memset(&m_last_key, 0, sizeof(m_last_key));
		_bulid_key_name_table();
	}
	CInput::~CInput()
	{
	}
	void CInput::update()
	{
		// 更新键盘状态
		memcpy(m_last_key, m_key, sizeof(m_last_key));
		GetKeyboardState(m_key);

		// 更新鼠标状态和位置
		memcpy(&m_last_mouse, &m_mouse, sizeof(m_last_mouse));
		m_mouse[LEFT_BUTTON] = m_key[VK_LBUTTON] & 0x80;
		m_mouse[RIGHT_BUTTON] = m_key[VK_RBUTTON] & 0x80;
		m_mouse[MIDDLE_BUTTON] = m_key[VK_MBUTTON] & 0x80;
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(m_wnd, &p);
		m_x = (float)p.x;
		RECT rc;
		GetClientRect(m_wnd, &rc);
		m_y = (float)((rc.bottom - rc.top) - p.y);	// 转换为原点为左下角的坐标系

		// how to get wheel's state?
	}
	bool CInput::key_down(unsigned int key) const
	{
		return (m_key[key] & 0x80) != 0;
	}
	bool CInput::key_up(unsigned int key) const
	{
		return (!(m_key[key] & 0x80) && m_key[key] != m_last_key[key]);
	}
	bool CInput::mouse_down(unsigned int button) const
	{
		return  m_mouse[button] != 0;
	}
	bool CInput::mouse_up(unsigned int button) const
	{
		return (m_mouse[button] == 0 && m_mouse[button] != m_last_mouse[button]);
	}
	float CInput::get_mouse_x() const
	{
		return m_x;
	}
	float CInput::get_mouse_y() const
	{
		return m_y;
	}
	float CInput::get_mouse_wheel() const
	{
		return m_wheel;
	}
	bool CInput::key_down_by_name(string name) const
	{
		KeyNameMap::const_iterator it = m_name_to_key.find(name);
		if (it == m_name_to_key.end())
			return false;
		return key_down(it->second);
	}
	bool CInput::key_up_by_name(string name) const
	{
		KeyNameMap::const_iterator it = m_name_to_key.find(name);
		if (it == m_name_to_key.end())
			return false;
		return key_up(it->second);
	}
	int CInput::get_key_down() const
	{
        for (int i = 1; i < KEY_NUM; i++)
        {
            if (key_down(i))
                return i;
        }
		return 0;
	}
	int CInput::get_key_up() const
	{
	    for (int i = 1; i < KEY_NUM; i++)
	    {
	        if (key_up(i))
                return i;
	    }
	    return 0;
	}
	void CInput::_bulid_key_name_table()
	{
		m_name_to_key["enter"] = VK_RETURN;
		m_name_to_key["space"] = VK_SPACE;
		m_name_to_key["up"] = VK_UP;
		m_name_to_key["down"] = VK_DOWN;
		m_name_to_key["left"] = VK_LEFT;
		m_name_to_key["right"] = VK_RIGHT;

		// copy from winuser.h
		// VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
		// 0x40 : unassigned
		// VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
	}
}
