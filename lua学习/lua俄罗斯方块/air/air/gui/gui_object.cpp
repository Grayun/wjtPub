#include "gui.h"
namespace air
{
	CGuiObject::CGuiObject(unsigned int id, float x, float y, float w, float h)
	{
		m_id = id;
		m_x = x;
		m_y = y;
		m_width = w;
		m_height = h;
		m_focus = false;
	}
	CGuiObject::~CGuiObject()
	{

	}
	bool CGuiObject::is_focus() const
	{
	    return m_focus;
	}
	void CGuiObject::lost()
	{
	    m_focus = false;
	}
	unsigned CGuiObject::get_id() const
	{
		return m_id;
	}
}
