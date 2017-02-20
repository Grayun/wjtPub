#include "../tool/common.h"
#include "../system/system.h"
#include "pen.h"
#include "surface.h"
#include "texture.h"
#include <fstream>
#include <sstream>
using std::ifstream;
using std::stringstream;

namespace air
{
	CPenManager* g_p_pen_mgr = NULL;
	CPenManager::CPenManager()
	{
		_free();
	}
	CPenManager::~CPenManager()
	{
		_free();
	}
	bool CPenManager::create(string name, int size, string style, int space, int weight)
	{
		Pen pen;
		pen.hfont = CreateFontA(size, space, 0, 0, weight, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS,
								CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, style.c_str());
		if (pen.hfont == NULL)
			return false;

		// 创建显示列表
		pen.list_id = glGenLists(96);
		if (pen.list_id == 0)
			return false;
		HFONT old;
		HDC hdc = g_p_system->getDC();
		old = (HFONT)SelectObject(hdc, pen.hfont);
		wglUseFontBitmaps(hdc, 32, 96, pen.list_id);
		SelectObject(hdc, old);

		if (m_pens.find(name) != m_pens.end())
			return false;
		m_pens[name] = pen;
		return true;
	}
	bool CPenManager::create_from_config(string config)
	{
		ifstream reader;
		reader.open(config.c_str());
		if (!reader.is_open())
			return false;

		char buff[1024];
		string name, size, style, space, weight;
		while (!reader.eof())
		{
			reader.getline(buff, 1024, '\n');
			buff[1024 - 1] = NULL;

			if (buff[0] == '\n' || buff[0] == '\r' || buff[0] == '#')
				continue;

			stringstream ss(buff);
			ss >> name >> size >> style >> space >> weight;
			if (!create(name, sti(size), style, sti(space), sti(weight)))
				continue;
		}
		reader.close();
		return true;
	}
	bool CPenManager::print(string name, float x, float y, color c, string msg)
	{
		if (g_p_openGL->get_mode() != COpenGL::_2D)
			return false;
		PenMap::iterator it = m_pens.find(name);
		if (it == m_pens.end())
			return false;
		_draw(it->second.list_id, x, y, c.r, c.g, c.b, msg);
		return true;
	}
	void CPenManager::destroy(string name)
	{
		// wait to implement
	}
	unsigned int CPenManager::get_char_height(string name)
	{
		//PenMap::iterator it = m_pens.find(name);
		//if (it == m_pens.end())
			return 0;
		//HFONT old;
		//old = (HFONT)SelectObject(m_hdc, it->second.hfont);
		//SIZE size;
		//string sentence("yqq");
		//GetTextExtentPoint32A(m_hdc, sentence.c_str(), (int)sentence.size(), &size);
		//SelectObject(m_hdc, old);
		//return size.cy;
	}
	unsigned int CPenManager::get_str_width(string name, string msg)
	{
		PenMap::iterator it = m_pens.find(name);
		if (it == m_pens.end())
			return 0;
        HDC hdc = g_p_system->getDC();
		HFONT old = (HFONT)SelectObject(hdc, it->second.hfont);
		SIZE size;
		GetTextExtentPoint32A(hdc, msg.c_str(), (int)msg.size(), &size);
		SelectObject(hdc, old);
		return size.cx;
	}
	void CPenManager::_free()
	{
		for (PenMap::iterator it = m_pens.begin(); it != m_pens.end(); it++)
		{
			glDeleteLists(it->second.list_id, 96);
			DeleteObject(it->second.hfont);
		}
		m_pens.clear();
	}
	void CPenManager::_draw(unsigned int id, float x, float y, unsigned char r, unsigned char g, unsigned char b, string msg)
	{
	    glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LIST_BIT);
	    glDisable(GL_TEXTURE_2D);
		glColor3ub(r, g, b);
		glRasterPos2f(x, y);
		glListBase(id - 32);
		glCallLists((int)strlen(msg.c_str()), GL_UNSIGNED_BYTE, msg.c_str());
		glPopAttrib();
	}
}
