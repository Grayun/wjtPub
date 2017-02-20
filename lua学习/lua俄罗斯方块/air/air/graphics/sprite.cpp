#include "sprite.h"
#include "../graphics/openGL.h"
#include "../graphics/texture.h"
#include "../tool/timer.h"
#include "../tool/common.h"
#include "../graphics/surface.h"

namespace air
{
	CSprite::CSprite()
	{
        m_x = m_y = m_z = 0.0f;
        m_w = m_h = 0.0f;
        m_angle = 0.0f;
        m_alpha = 255;
        m_aabb.left = m_aabb.right = m_aabb.bottom = m_aabb.top = 0.0f;
	}
	CSprite::~CSprite()
	{

	}
	void CSprite::init(string image, float x, float y, float w, float h)
	{
        m_image = image;
		m_x = x;
		m_y = y;
		m_w = w;
		m_h = h;
	}
	void CSprite::move(float x, float y)
	{
		m_x += x;
		m_y += y;
		if (m_aabb.left == 0.0f && m_aabb.right == 0.0f && m_aabb.top == 0.0f && m_aabb.bottom == 0.0f)
            return;
		m_aabb.left += x;
		m_aabb.right += x;
		m_aabb.bottom += y;
		m_aabb.top += y;
	}
	void CSprite::put(float x, float y)
	{
	    m_x = x;
	    m_y = y;
        if (m_aabb.left == 0.0f && m_aabb.right == 0.0f && m_aabb.top == 0.0f && m_aabb.bottom == 0.0f)
            return;
        float aabb_w = m_aabb.right - m_aabb.left;
        float aabb_h = m_aabb.top - m_aabb.bottom;
        m_aabb.left = x;
		m_aabb.right = x + aabb_w;
		m_aabb.bottom = y;
		m_aabb.top = y + aabb_h;
	}
	void CSprite::rotate(float angle)
	{
		m_angle += angle;
	}
	void CSprite::fade(int alpha)
	{
		int t = m_alpha + alpha;
		if (t > 255)
			m_alpha = 255;
		else if (t < 0)
			m_alpha = 0;
		else
			m_alpha += alpha;
	}
	void CSprite::draw()
	{
		const CTextureManager::Texture* p_tex = g_p_tex_mgr->get_texture(m_image.c_str());
		if (p_tex == NULL)
			return;
		float half_x = (float)(p_tex->width >> 1);
		float half_y = (float)(p_tex->height >> 1);
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(half_x, half_y, 0.0f);
		glTranslatef(m_x, m_y, 0.0f);
		glRotatef(m_angle, 0.0f, 0.0f, 1.0f);
		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4ub(255, 255, 255, m_alpha);
		g_p_tex_mgr->raster_origin_center(m_image.c_str());
		glPopAttrib();
		glPopMatrix();
	}
	float CSprite::pos_x() const
	{
	    return m_x;
	}
	float CSprite::pos_y() const
	{
	    return m_y;
	}
	float CSprite::width() const
	{
	    return m_w;
	}
	float CSprite::height() const
	{
	    return m_h;
	}
	void CSprite::init_aabb(float left, float right, float bottom, float top)
	{
	    m_aabb.left = left;
	    m_aabb.right = right;
	    m_aabb.bottom = bottom;
	    m_aabb.top = top;
	}
//	aabb CSprite::box() const;
//	{
//	    return m_aabb;
//	}
	bool CSprite::is_collide(const CSprite& spr) const
	{
	    aabb other = spr.box();
        if (m_aabb.right < other.left || m_aabb.left > other.right || m_aabb.bottom > other.top || m_aabb.top < other.bottom)
            return false;
        else
            return true;
	}
	void CSprite::debug_draw_aabb() const
	{
	    g_p_surface->draw_rect(m_aabb.left, m_aabb.bottom, m_aabb.right - m_aabb.left, m_aabb.top - m_aabb.bottom, color(255,0,0), false);
	}
	//////////////////////////////////////////////////////////////////////////
	CAniSprite::CAniSprite()
	{
	    m_p_pace_controller = new CTimer();
		m_cur_x = m_cur_y = 0;
		m_total_x = m_total_y = 0;
		m_frame_w = m_frame_h = 0;
		m_seq = Y;
	}
	CAniSprite::~CAniSprite()
	{
		delete m_p_pace_controller;
	}
	void CAniSprite::init_ani(unsigned int ms, unsigned int total_x, unsigned int total_y, Sequence seq)
	{
		const CTextureManager::Texture* p_tex = g_p_tex_mgr->get_texture(m_image.c_str());
		if (p_tex == NULL)
			return;
		m_interval_ms = ms;
		m_total_x = total_x;
		m_total_y = total_y;
		m_frame_w = p_tex->width / total_x;
		m_frame_h = p_tex->height / total_y;
		m_seq = seq;
		m_p_pace_controller->reset();
	}
	void CAniSprite::set_sequence(unsigned int idx)
	{
		if (m_seq == X)
			m_cur_y = idx;
		else if (m_seq == Y)
			m_cur_x = idx;
	}
	void CAniSprite::draw()
	{
		double pass = m_p_pace_controller->pass_ms();
		if (pass > m_interval_ms)
		{
			if (m_seq == X)
				m_cur_x = (m_cur_x++) % m_total_x;
			else if (m_seq == Y)
				m_cur_y = (m_cur_y++) % m_total_y;
			m_p_pace_controller->reset();
		}
		_draw(m_cur_x, m_cur_y);
	}
	void CAniSprite::_draw(unsigned int x, unsigned int y)
	{
		const CTextureManager::Texture* p_tex = g_p_tex_mgr->get_texture(m_image.c_str());
		if (p_tex == NULL)
			return;
		float half_x = (float)(m_frame_w >> 1);
		float half_y = (float)(m_frame_h >> 1);
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(half_x, half_y, 0.0f);
		glTranslatef(m_x, m_y, 0.0f);
		glRotatef(m_angle, 0.0f, 0.0f, 1.0f);
		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4ub(255, 255, 255, m_alpha);
		g_p_tex_mgr->raster_origin_center(m_image.c_str(), x * m_frame_w, y * m_frame_h, m_frame_w, m_frame_h);
		glPopAttrib();
		glPopMatrix();
	}
}
