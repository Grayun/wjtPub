#include "gui.h"
#include "../tool/common.h"
#include "../input/input.h"
#include <fstream>
#include <sstream>
using std::ifstream;
using std::stringstream;

namespace air
{
	CGuiSystem::CGuiSystem()
	{

	}
	CGuiSystem::~CGuiSystem()
	{
		for (GuiList::iterator it = m_gui_object_list.begin(); it != m_gui_object_list.end(); it++)
		{
			delete (*it);
			(*it) = NULL;
		}
		m_gui_object_list.clear();
	}
	void CGuiSystem::add(CGuiObject* p_obj)
	{
		if (p_obj == NULL)
			return;
		m_gui_object_list.push_back(p_obj);
	}
	bool CGuiSystem::load_from_config(const char* p_config)
	{
		ifstream reader;
		reader.open(p_config);
		if (!reader.is_open())
			return false;

		char buff[1024];
		string type;
		while (!reader.eof())
		{
			reader.getline(buff, 1024, '\n');
			buff[1024 - 1] = NULL;
			if (buff[0] == '\n' || buff[0] == '\r' || buff[0] == '#')
				continue;
			stringstream ss(buff);
			ss >> type;

			// gui 分类初始化
			if (type == "button")
			{
				string id, x, y, width, height, up, down, over;
				ss >> id >> x >> y >> width >> height >> up >> down >> over;
				CButton* p_obj = new CButton(sti(id), stf(x), stf(y), stf(width), stf(height), up, down, over);
				m_gui_object_list.push_back(p_obj);
			}
			else if (type == "label")
			{
                string id, x, y, text, pen, r, g, b;
				ss >> id >> x >> y >> text >> pen >> r >> g >> b;
				CLabel* p_obj = new CLabel(sti(id), stf(x), stf(y), text, pen, color(sti(r), sti(g), sti(b)));
				m_gui_object_list.push_back(p_obj);
			}
			else if (type == "edit")
            {
                string id, x, y, w, h, pen;
                ss >> id >> x >> y >> w >> h >> pen;
                CEdit* p_obj = new CEdit(sti(id), stf(x), stf(y), stf(w), stf(h), pen);
                m_gui_object_list.push_back(p_obj);
            }
		}
		reader.close();
		return true;
	}
	void CGuiSystem::draw()
	{
		for (GuiList::iterator it = m_gui_object_list.begin(); it != m_gui_object_list.end(); it++)
            (*it)->draw();
	}
	unsigned int CGuiSystem::process_msg()
	{
        CGuiObject::msg e;
        int k = 0;
		for (GuiList::iterator it = m_gui_object_list.begin(); it != m_gui_object_list.end(); it++)
		{
		    // 构造键盘消息
		    memset(&e, 0, sizeof(e));
		    k = g_p_input->get_key_down();
		    if (k != 0 && (*it)->is_focus())
                e.type = CGuiObject::KEY_DOWN;
		    k = g_p_input->get_key_up();
		    if (k != 0 && (*it)->is_focus())
                e.type = CGuiObject::KEY_UP;
		    if (KEY_DOWN(VK_SHIFT))
                e.flags |= CGuiObject::SHIFT;
            e.key = k;
		    if (e.type != CGuiObject::NIL && (*it)->on_msg(e))
                return (*it)->get_id();
		    // 构造鼠标消息
		    memset(&e, 0, sizeof(e));
		    e.x = g_p_input->get_mouse_x();
		    e.y = g_p_input->get_mouse_y();
		    if ((*it)->is_over(e.x, e.y))
		    {
		        if (MOUSE_DOWN(LEFT_BUTTON))
		        {
		            all_lost();
		            e.type = CGuiObject::MOUSE_L_DOWN;
		        }
		        else if (MOUSE_UP(LEFT_BUTTON))
		        {
		            e.type = CGuiObject::MOUSE_L_UP;
		        }
		        else
		        {
		            e.type = CGuiObject::MOUSE_MOVE;
		        }
		        if ((*it)->on_msg(e))
                    return (*it)->get_id();
		    }
		}
		return NO_MSG;
	}
	void CGuiSystem::all_lost()
	{
	    for (GuiList::const_iterator it = m_gui_object_list.begin(); it != m_gui_object_list.end(); it++)
            (*it)->lost();
	}
	CGuiObject* CGuiSystem::get(unsigned int id)
	{
	    for (GuiList::const_iterator it = m_gui_object_list.begin(); it != m_gui_object_list.end(); it++)
        {
            if (id == (*it)->get_id())
                return *it;
        }
        return NULL;
	}
}
