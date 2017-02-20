#include "map.h"
#include "../graphics/surface.h"
#include <fstream>
#include <sstream>
using std::ifstream;
using std::stringstream;
namespace air
{
	CMap::CMap()
	{
		m_p_map = NULL;
		m_p_flag = NULL;
		m_tile_size = 0;
		m_tile_num_x = m_tile_num_y = 0;
		memset(&m_view, 0, sizeof(View));
		m_draw_num_x = m_draw_num_y = 0;
	}
	CMap::~CMap()
	{
		delete[] m_p_map;
		m_p_map = NULL;
		delete[] m_p_flag;
		m_p_flag = NULL;
	}
	bool CMap::init(const char* p_config, unsigned int view_w, unsigned int view_h)
	{
		ifstream reader;
		reader.open(p_config);
		if (!reader.is_open())
			return false;
		reader >> m_tex >> m_tile_size >> m_tile_num_x >> m_tile_num_y;
		unsigned int total = m_tile_num_x * m_tile_num_y;
		m_p_map = new unsigned int[total];
		for (unsigned int i = 0; i < total; i++)
		{
			m_p_map[i] = 0;
			reader >> m_p_map[i];
		}
		reader.close();
		m_view.width = view_w;
		m_view.hight = view_h;
		m_view.right = m_view.left + view_w;
		m_view.top = m_view.bottom + view_h;
		m_draw_num_x = view_w / m_tile_size + 1;	// +1是因为当偏移量不为0时，需要多画一个图块
		m_draw_num_y = view_h / m_tile_size + 1;
		return true;
	}
	void CMap::scroll(int x, int y)
	{
		if (m_tile_size == 0)
			return;
		m_view.left += x;
		m_view.bottom += y;
		int edge_x = m_tile_size * m_tile_num_x - m_view.width;
		int edge_y = m_tile_size * m_tile_num_y - m_view.hight;
		if ((int)m_view.left < 0)
			m_view.left = 0;
		else if ((int)m_view.left > edge_x)
			m_view.left = edge_x;
		if ((int)m_view.bottom < 0)
			m_view.bottom = 0;
		else if ((int)m_view.bottom > edge_y)
			m_view.bottom = edge_y;
	}
	void CMap::draw()
	{
		if (m_tile_size == 0)
			return;
		unsigned int scroll_x, scroll_y;
		unsigned int offset_x, offset_y;
		for (unsigned int y = 0; y < m_draw_num_y; y++)
		{
			for (unsigned int x = 0; x < m_draw_num_x; x++)
			{
				scroll_x = x + m_view.left / m_tile_size;
				scroll_y = y + m_view.bottom / m_tile_size;
				offset_x = m_view.left % m_tile_size;
				offset_y = m_view.bottom % m_tile_size;
				if (m_p_map[scroll_y * m_tile_num_x + scroll_x] == 0)
					continue;
				float tex_x = (float)((m_p_map[scroll_y * m_tile_num_x + scroll_x] - 1) * m_tile_size);
				float tex_y = 0.0f;
				// 先转化为int保留符号，再转化为float消除警告
				g_p_surface->draw_texture(m_tex.c_str(), (float)(int)(x * m_tile_size - offset_x), (float)(int)(y * m_tile_size - offset_y), tex_x, tex_y, (float)m_tile_size, (float)m_tile_size);
			}
		}
	}
}
