#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace air
{
	CBmp::CBmp()
	{
		m_p_rgb = NULL;
		m_p_rgba = NULL;
		m_w = 0;
		m_h = 0;
	}
	CBmp::~CBmp()
	{
		unload();
	}
	#define WIDTHBYTES(bits) (((bits)+31)/32*4)
	bool CBmp::load(const char* p_file)
	{
		FILE* fp = NULL;
		fp = fopen(p_file, "rb");
		if (fp == NULL)
			return false;
		head _head;
		info _info;
		fread(&_head, 1, sizeof(head), fp);
		fread(&_info, 1, sizeof(info), fp);
		if (_head.id != 19778 || _info.compress != 0 || _info.bit_per_pixel != 24)
			return false;
		m_w = _info.pixel_width;
		m_h = _info.pixel_height;
		u32 l_width = WIDTHBYTES(m_w * _info.bit_per_pixel);
		u32 color_data_size = m_h * l_width;
		u8* p_color_data = (u8*)malloc(color_data_size);
		memset(p_color_data, 0, color_data_size);
		fread(p_color_data, 1, color_data_size, fp);

		rgb* p_rgb = NULL;
		p_rgb = (rgb*)malloc(m_w * m_h * sizeof(rgb));
		memset(p_rgb, 0, m_w * m_h * sizeof(rgb));

		u32 idx = 0;
		for (u32 y = 0; y < m_h; y++)
		{
			for (u32 x = 0; x < m_w; x++)
			{
				u32 k = y * l_width + x * 3;
				p_rgb[idx].r = p_color_data[k + 2];
				p_rgb[idx].g = p_color_data[k + 1];
				p_rgb[idx].b = p_color_data[k];
				idx++;
			}
		}

		m_p_rgb = (u8*)p_rgb;
		free(p_color_data);
		fclose(fp);
		return true;
	}
	void CBmp::convert_to_rgba(u8 r, u8 g, u8 b)
	{
		m_p_rgba = (u8*)malloc(m_w * m_h * 4 * sizeof(u8));
		for (u32 y = 0; y < m_h; y++)
		{
			for (u32 x = 0; x < m_w; x++)
			{
				memcpy(&m_p_rgba[(y * m_w + x) * 4], &m_p_rgb[(y * m_w + x) * 3], 3 * sizeof(u8));
				if (m_p_rgb[(y * m_w + x) * 3 + 0] == r && m_p_rgb[(y * m_w + x) * 3 + 1] == g && m_p_rgb[(y * m_w + x) * 3 + 2] == b)
					m_p_rgba[(y * m_w + x) * 4 + 3] = 0;
				else
					m_p_rgba[(y * m_w + x) * 4 + 3] = 255;
			}
		}
	}
	const u8* CBmp::rgb_data() const
	{
		return m_p_rgb;
	}
	const u8* CBmp::rgba_data() const
	{
		return m_p_rgba;
	}
	u32 CBmp::width() const
	{
		return m_w;
	}
	u32 CBmp::height() const
	{
		return m_h;
	}
	void CBmp::unload()
	{
		free(m_p_rgb);
		m_p_rgb = NULL;
		free(m_p_rgba);
		m_p_rgba = NULL;
	}
	//////////////////////////////////////////////////////////////////////////
	CTga::CTga()
	{
		m_w = 0;
		m_h = 0;
		m_p_rgba = NULL;
	}
	CTga::~CTga()
	{
		unload();
	}
	bool CTga::load(const char* p_file)
	{
		FILE* pf = NULL;
		pf = fopen(p_file, "rb");
		if (pf == NULL)
			return false;
		head _head;
		fread(&_head, 1, sizeof(head), pf);
		if (_head.image_type != 2 || _head.bit_per_pixel != 32)
			return false;
		m_w = _head.image_width;
		m_h = _head.image_height;
		u32 bytes_per_pixel = _head.bit_per_pixel / 8;
		u32 bytes_image = _head.image_width * _head.image_height * bytes_per_pixel;
		m_p_rgba = (u8*)malloc(bytes_image);
		fread(m_p_rgba, 1, bytes_image, pf);
		for (u32 i = 0; i < bytes_image; i += bytes_per_pixel)
		{
			SWAP(m_p_rgba[i], m_p_rgba[i + 2]);
		}
		fclose(pf);
		return true;
	}
	void CTga::unload()
	{
		free(m_p_rgba);
		m_p_rgba = NULL;
	}
	const u8* CTga::data() const
	{
		return m_p_rgba;
	}
	u32 CTga::width() const
	{
		return m_w;
	}
	u32 CTga::height() const
	{
		return m_h;
	}
}
