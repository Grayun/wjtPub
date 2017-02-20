#include "gui.h"
#include "../graphics/surface.h"
namespace air
{
	CButton::CButton(unsigned int id, float x, float y, float width, float height, string up_tex, string down_tex, string over_tex) : CGuiObject(id, x, y, width, height)
	{
		m_up_tex = up_tex;
		m_down_tex = down_tex;
		m_over_tex = over_tex;
		m_state = B_UP;
	}
	void CButton::draw()
	{
		if (B_UP == m_state)
			g_p_surface->draw_sprite(m_up_tex.c_str(), m_x, m_y);
		else if (B_DOWN == m_state)
			g_p_surface->draw_sprite(m_down_tex.c_str(), m_x, m_y);
		else if (B_OVER == m_state)
			g_p_surface->draw_sprite(m_over_tex.c_str(), m_x, m_y);
	}
	bool CButton::on_msg(const msg& e)
	{
	    if (e.type == MOUSE_MOVE)
        {
            m_state = B_OVER;
            return true;
        }
        if (e.type == MOUSE_L_DOWN)
        {
            m_state = B_DOWN;
            return true;
        }
        if (e.type == MOUSE_L_UP)
        {
            m_state = B_UP;
            return true;
        }
        return false;
	}
	bool CButton::is_over(float x, float y)
	{
		if (x >= m_x && x <= m_x + m_width)
		{
			if (y >= m_y && y <= m_y + m_height)
			{
				if (m_state != B_DOWN)
					m_state = B_OVER;
				return true;
			}
		}
		m_state = B_UP;
		return false;
	}
}
