#include "gui.h"
#include "../graphics/pen.h"
namespace air
{
    CLabel::CLabel(unsigned int id, float x, float y, string text, string pen, color c) : CGuiObject(id, x, y, 0.0f, 0.0f)
    {
        m_text = text;
        m_pen = pen;
        m_text_color = c;
    }
	void CLabel::draw()
	{
		if (!m_pen.empty() && !m_text.empty())
			g_p_pen_mgr->print(m_pen, m_x, m_y, m_text_color, m_text.c_str());
	}
	bool CLabel::on_msg(const msg& e)   { return false; }
	bool CLabel::is_over(float x, float y)	{ return false; }
}
