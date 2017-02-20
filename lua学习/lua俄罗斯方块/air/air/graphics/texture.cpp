#include "../tool/common.h"
#include "texture.h"
#include "surface.h"
#include "image.h"
#include <fstream>
#include <sstream>
using std::ifstream;
using std::stringstream;

namespace air
{
	CTextureManager* g_p_tex_mgr = NULL;
	CTextureManager::CTextureManager()	{ _free(); }
	CTextureManager::~CTextureManager()	{ _free(); }

	bool CTextureManager::load_bmp(const char* p_file, const char* p_name, unsigned char r, unsigned char g, unsigned char b)
	{
		CBmp bmp;
		if (!bmp.load(p_file))
			return false;
		bmp.convert_to_rgba(r, g, b);

		Texture tex;
		glGenTextures(1, &tex.id);
		glBindTexture(GL_TEXTURE_2D, tex.id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	// 如果使用 GL_LINEAR，会导致透明色周围出现毛刺
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bmp.width(), bmp.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp.rgba_data());

		tex.width = bmp.width();
		tex.height = bmp.height();
		string name(p_name);
		if (m_texs.find(name) != m_texs.end())
			return false;
		m_texs[name] = tex;
		return true;
	}

	bool CTextureManager::load_tga(const char* p_file, const char* p_name)
	{
		return true;
	}

	bool CTextureManager::load_from_config(const char* p_config)
	{
		ifstream reader;
		reader.open(p_config);
		if (!reader.is_open())
			return false;

		char buff[1024];
		string name, path, trans_r, trans_g, trans_b;
		while (!reader.eof())
		{
			reader.getline(buff, 1024, '\n');
			buff[1024 - 1] = NULL;

			if (buff[0] == '\n' || buff[0] == '\r' || buff[0] == '#')
				continue;

			stringstream ss(buff);
			ss >> name >> path >> trans_r >> trans_g >> trans_b;
			if (!trans_r.empty() && !trans_g.empty() && !trans_b.empty())
			{
				if (!load_bmp(path.c_str(), name.c_str(), sti(trans_r), sti(trans_g), sti(trans_b)))
					continue;
			}
			else
			{
				if (!load_bmp(path.c_str(), name.c_str()))
					continue;
			}
		}
		reader.close();
		return true;
	}

	unsigned int CTextureManager::get_id(const char* p_name) const
	{
		string name(p_name);
		TexMap::const_iterator it = m_texs.find(name);
		if (it != m_texs.end())
			return (it->second).id;
		return 0;
	}

	unsigned int CTextureManager::get_width(const char* p_name) const
	{
		string name(p_name);
		TexMap::const_iterator it = m_texs.find(name);
		if (it != m_texs.end())
			return (it->second).width;
		return 0;
	}

	unsigned int CTextureManager::get_height(const char* p_name) const
	{
		string name(p_name);
		TexMap::const_iterator it = m_texs.find(name);
		if (it != m_texs.end())
			return (it->second).height;
		return 0;
	}

	const CTextureManager::Texture* CTextureManager::get_texture(const char *p_name) const
	{
		string name(p_name);
		TexMap::const_iterator it = m_texs.find(name);
		if (it != m_texs.end())
			return &(it->second);
		return NULL;
	}

	bool CTextureManager::raster_origin_center(const char* p_name, float sub_x, float sub_y, float sub_width, float sub_height) const
	{
		TexMap::const_iterator it = m_texs.find(p_name);
		if (it == m_texs.end())
			return false;

		int w = it->second.width;
		int h = it->second.height;
		glBindTexture(GL_TEXTURE_2D, it->second.id);
		glBegin(GL_QUADS);
			if (sub_x == 0.0f && sub_y == 0.0f && sub_width == 0.0f && sub_height == 0.0f)
			{
				glTexCoord2f(0.0f, 0.0f);	glVertex2i(-w / 2, -h / 2);	// 左下角
				glTexCoord2f(1.0f, 0.0f);	glVertex2i(w / 2, -h / 2);	// 右下角
				glTexCoord2f(1.0f, 1.0f);	glVertex2i(w / 2, h / 2);	// 右上角
				glTexCoord2f(0.0f, 1.0f);	glVertex2i(-w / 2, h / 2);	// 左上角
			}
			else
			{
				float start_x = sub_x / w;
				float start_y = sub_y / h;
				float end_x = (sub_x + sub_width) / w;
				float end_y = (sub_y + sub_height) / h;
				glTexCoord2f(start_x, start_y);	glVertex2f(-sub_width / 2, -sub_height / 2);
				glTexCoord2f(end_x, start_y);	glVertex2f(sub_width / 2, -sub_height / 2);
				glTexCoord2f(end_x, end_y);		glVertex2f(sub_width / 2, sub_height / 2);
				glTexCoord2f(start_x, end_y);	glVertex2f(-sub_width / 2, sub_height / 2);
			}
		glEnd();
		return true;
	}

	bool CTextureManager::raster_origin_leftbottom(const char* p_name, float sub_x, float sub_y, float sub_width, float sub_height) const
	{
		TexMap::const_iterator it = m_texs.find(p_name);
		if (it == m_texs.end())
			return false;

		int w = it->second.width;
		int h = it->second.height;
		glBindTexture(GL_TEXTURE_2D, it->second.id);
		glBegin(GL_QUADS);
			if (sub_x == 0.0f && sub_y == 0.0f && sub_width == 0.0f && sub_height == 0.0f)
			{
				glTexCoord2f(0.0f, 0.0f);	glVertex2i(0, 0);	// 左下角
				glTexCoord2f(1.0f, 0.0f);	glVertex2i(w, 0);	// 右下角
				glTexCoord2f(1.0f, 1.0f);	glVertex2i(w, h);	// 右上角
				glTexCoord2f(0.0f, 1.0f);	glVertex2i(0, h);	// 左上角
			}
			else
			{
				// 计算纹理坐标
				float start_x = sub_x / w;
				float start_y = sub_y / h;
				float end_x = (sub_x + sub_width) / w;
				float end_y = (sub_y + sub_height) / h;

				glTexCoord2f(start_x, start_y);	glVertex2f(0, 0);
				glTexCoord2f(end_x, start_y);	glVertex2f(sub_width, 0);
				glTexCoord2f(end_x, end_y);		glVertex2f(sub_width, sub_height);
				glTexCoord2f(start_x, end_y);	glVertex2f(0, sub_height);
			}
		glEnd();
		return true;
	}

	void CTextureManager::_free()
	{
		for (TexMap::iterator it = m_texs.begin(); it != m_texs.end(); it++)
			glDeleteTextures(1, &it->second.id);
		m_texs.clear();
	}

	//////////////////////////////////////////////////////////////////////////
	CImageFont::CImageFont()
	{
		memset(&m_font, 0, sizeof(Font));
		memset(&m_ascii_to_list, 0, sizeof(m_ascii_to_list));
	}
	CImageFont::~CImageFont()
	{
		_free();
	}
	bool CImageFont::create(const char* p_tex_name)
	{
		const CTextureManager::Texture* p_tex = g_p_tex_mgr->get_texture(p_tex_name);
		if (p_tex == NULL)
			return false;
		m_font.width = p_tex->width / 26;
		m_font.height = p_tex->height / 2;
		m_font.id = glGenLists(37);		// 26个字母，10个数字，1个空白
		if (m_font.id == 0)
			return false;
		float x_unit = 1.0f / p_tex->width;
		m_font.tex_id = p_tex->id;
		for (int i = 0; i < 10; i++)	// 0 ~ 9
		{
			glNewList(m_font.id + i, GL_COMPILE);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0);
			glBegin(GL_QUADS);
				glTexCoord2f(m_font.width * i * x_unit, 0.0f);
				glVertex2i(0, 0);
				glTexCoord2f(m_font.width * (i + 1) * x_unit, 0.0f);
				glVertex2i(m_font.width, 0);
				glTexCoord2f(m_font.width * (i + 1) * x_unit, 0.5f);
				glVertex2i(m_font.width, m_font.height);
				glTexCoord2f(m_font.width * i * x_unit, 0.5f);
				glVertex2i(0, m_font.height);
			glEnd();
			glDisable(GL_ALPHA_TEST);
			glEndList();
		}
		for (int i = 0; i < 26; i++)	// A ~ Z
		{
			glNewList(m_font.id + i + 10, GL_COMPILE);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0);
			glBegin(GL_QUADS);
				glTexCoord2f(m_font.width * i * x_unit, 0.5f);
				glVertex2i(0, 0);
				glTexCoord2f(m_font.width * (i + 1) * x_unit, 0.5f);
				glVertex2i(m_font.width, 0);
				glTexCoord2f(m_font.width * (i + 1) * x_unit, 1.0f);
				glVertex2i(m_font.width, m_font.height);
				glTexCoord2f(m_font.width * i * x_unit, 1.0f);
				glVertex2i(0, m_font.height);
			glEnd();
			glDisable(GL_ALPHA_TEST);
			glEndList();
		}
		// 空白
		glNewList(m_font.id + 36, GL_COMPILE);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0);
		glBegin(GL_QUADS);
			glTexCoord2f(m_font.width * 10 * x_unit, 0.0f);
			glVertex2i(0, 0);
			glTexCoord2f(m_font.width * 11 * x_unit, 0.0f);
			glVertex2i(m_font.width, 0);
			glTexCoord2f(m_font.width * 11 * x_unit, 0.5f);
			glVertex2i(m_font.width, m_font.height);
			glTexCoord2f(m_font.width * 10 * x_unit, 0.5f);
			glVertex2i(0, m_font.height);
		glEnd();
		glDisable(GL_ALPHA_TEST);
		glEndList();
		_init_char_to_list(m_font.id);
		return true;
	}
	void CImageFont::print(float x, float y, string msg)
	{
		glPushMatrix();
		glLoadIdentity();
		glBindTexture(GL_TEXTURE_2D, m_font.tex_id);
		glTranslatef(x, y, 0.0f);
		size_t size = msg.size();
		for (size_t i = 0; i < size; i++)
		{
			_print(msg.c_str()[i]);
			glTranslatef((float)m_font.width, 0.0f, 0.0f);
		}
		glPopMatrix();
	}
	void CImageFont::_init_char_to_list(unsigned int start)
	{
		unsigned int list = 0;
		for (int ascii = '0'; ascii <= '9'; ascii++)	// 0 ~ 9
		{
			m_ascii_to_list[ascii] = start + list;
			list++;
		}
		for (int ascii = 'A'; ascii <= 'Z'; ascii++)	// A ~ Z
		{
			m_ascii_to_list[ascii] = start + list;
			list++;
		}
		m_ascii_to_list[' '] = start + list;		// ' '
	}
	void CImageFont::_print(char c)
	{
		if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z'))
			glCallList(m_ascii_to_list[c]);
	}
	void CImageFont::_free()
	{
		glDeleteLists(m_font.id, 37);
	}
}
