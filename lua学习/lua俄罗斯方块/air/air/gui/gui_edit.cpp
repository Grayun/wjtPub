#include "gui.h"
#include "../graphics/pen.h"
#include "../graphics/surface.h"
#include "../input/input.h"
#include <stdio.h>
namespace air
{
    CEdit::CEdit(unsigned int id, float x, float y, float width, float height, string pen) : CGuiObject(id, x, y, width, height)
    {
        m_pen = pen;
    }
    CEdit::~CEdit()
    {
    }
    void CEdit::draw()
	{
	    if (m_focus)
            g_p_surface->draw_rect(m_x, m_y, m_width, m_height, color(255,0,0), false);
        else
            g_p_surface->draw_rect(m_x, m_y, m_width, m_height, color(128,128,128), false);
		if (!m_pen.empty() && !m_result.empty())
			g_p_pen_mgr->print(m_pen, m_x + 2.0f, m_y + 7.0f, color(255,255,255), m_result.c_str());
	}
	bool CEdit::on_msg(const msg& e)
	{
	    if (e.type == MOUSE_L_DOWN)
	    {
	        m_focus = true;
	        return true;
	    }
	    if (e.type == KEY_UP)
	    {
	        if (!is_full())
            {
                if (e.key >= 'A' && e.key <= 'Z')
                {
                    char c;
                    if (e.flags & SHIFT)
                        c = (char)e.key;
                    else
                        c = (char)e.key + 32;
                    m_result.append(1, c);
                    return true;
                }
                if (e.key >= '0' && e.key <= '9')
                {
                    char c;
                    if (e.flags & SHIFT)
                    {
                        if (e.key == '8')   c = '*';
                        else if (e.key == '9')  c = '(';
                        else if (e.key == '0')  c = ')';
                    }
                    else
                    {
                        c = (char)e.key;
                    }
                    m_result.append(1, c);
                    return true;
                }
                if (e.key == 189)
                {
                    m_result.append(1, '-');
                    return true;
                }
                if (e.key == 187 && e.flags & SHIFT)
                {
                    m_result.append(1, '+');
                    return true;
                }
                if (e.key == 190)
                {
                    m_result.append(1, '.');
                    return true;
                }
                if (e.key == 191)
                {
                    m_result.append(1, '/');
                    return true;
                }
            }
            if (e.key == VK_BACK && !m_result.empty())
            {
                m_result = m_result.erase(m_result.size() - 1, 1);
                return true;
            }
            if (e.key == VK_RETURN)
            {
                m_focus = false;
                return true;
            }
	    }
	    return false;
	}
	bool CEdit::is_over(float x, float y)
	{
        if (x >= m_x && x <= m_x + m_width)
			if (y >= m_y && y <= m_y + m_height)
				return true;
		return false;
	}
	string CEdit::content() const
	{
	    return m_result;
	}
	bool CEdit::is_full() const
	{
	    if (g_p_pen_mgr->get_str_width(m_pen, m_result) + 4.0f < m_width)
            return false;
        return true;
	}
}
