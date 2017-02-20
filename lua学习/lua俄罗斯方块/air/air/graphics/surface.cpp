#include "surface.h"
#include "texture.h"
#include <stdio.h>
#include <math.h>

namespace air
{
	COpenGL* g_p_openGL = NULL;
	COpenGL::COpenGL(HDC hdc, int width, int height)
	{
		m_hrc = NULL;
		if (!(m_hrc = wglCreateContext(hdc)))
			PostQuitMessage(0);
		if (!wglMakeCurrent(hdc, m_hrc))
			PostQuitMessage(0);
		m_width = width;
		m_height = height;
		m_mode = _3D;
		_init();
	}
	COpenGL::~COpenGL()
	{
		if (m_hrc)
		{
			if (!wglMakeCurrent(NULL, NULL))
				MessageBoxA(NULL, "OpenGL wglMakeCurrent() fail...", "error", NULL);
			if (!wglDeleteContext(m_hrc))
				MessageBoxA(NULL, "OpenGL wglDeleteContext() fail...", "error", NULL);
		}
		m_hrc = NULL;
	}
	void COpenGL::resize(int new_width, int new_height)
	{
		if (0 == new_height)
			new_height = 1;
		if (0 == new_width)
			new_width = 1;
		m_width = new_width;
		m_height = new_height;
		glViewport(0, 0, m_width, m_height);
	}
	void COpenGL::frame() const
	{
		// 清屏
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 错误处理
		GLenum err;
		err = glGetError();
		if (err == GL_NO_ERROR)
			return;
		char buff[1024];
		sprintf(buff, "OpenGL frame() catch an error! error code:%d", err);
		MessageBoxA(NULL, buff, "error", NULL);
		PostQuitMessage(0);
	}
	void COpenGL::twoD()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, m_width, 0, m_height, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		m_mode = _2D;
	}
	void COpenGL::threeD()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, (double)m_width / (double)m_height, 1.0f, 1000.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		m_mode = _3D;
	}
	void COpenGL::swap_buff(HDC hdc) const
	{
        SwapBuffers(hdc);
	}
	COpenGL::Mode COpenGL::get_mode() const
	{
		return m_mode;
	}
	int COpenGL::get_height() const
	{
		return m_height;
	}
	void COpenGL::_init()
	{
		glShadeModel(GL_SMOOTH);				// 启用阴影平滑
		glClearDepth(1.0f);						// 深度缓存
		glEnable(GL_DEPTH_TEST);				// 启用深度测试
		glDepthFunc(GL_LEQUAL);					// 深度测试类型
		glEnable(GL_TEXTURE_2D);				// 开启纹理
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);	// 背景颜色

		// 效果最佳
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
		glHint(GL_FOG_HINT, GL_NICEST);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glEnable(GL_POLYGON_SMOOTH);			// 多边形抗锯齿
		glViewport(0, 0, m_width, m_height);
	}

	//////////////////////////////////////////////////////////////////////////
	CSurface* g_p_surface = NULL;
	CSurface::CSurface()	{}
	CSurface::~CSurface()	{}
	bool CSurface::draw_line(float start_x, float start_y, float end_x, float end_y, color c, float size)
	{
		if (g_p_openGL->get_mode() == COpenGL::_3D)
			return false;
		glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LINE_BIT);
		glDisable(GL_TEXTURE_2D);
		glLineWidth(size);
		glBegin(GL_LINES);
			glColor4ub(c.r, c.g, c.b, c.a);
			glVertex2f(start_x, start_y);
			glVertex2f(end_x, end_y);
		glEnd();
		glPopAttrib();
		return true;
	}
	bool CSurface::draw_rect(float x, float y, float width, float height, color c, bool fill)
	{
		if (g_p_openGL->get_mode() == COpenGL::_3D)
			return false;
		glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4ub(c.r, c.g, c.b, c.a);
		if (fill)
		{
			glRectf(x, y, x + width, y + height);
		}
		else
		{
			glBegin(GL_LINE_LOOP);
				glVertex2f(x, y);
				glVertex2f(x + width, y);
				glVertex2f(x + width, y + height);
				glVertex2f(x, y + height);
			glEnd();
		}
		glPopAttrib();
		return true;
	}
	const float PI = (float)3.141592654f;
	bool CSurface::draw_circle(float x, float y, float radius, color c, bool fill)
	{
		if (g_p_openGL->get_mode() == COpenGL::_3D)
			return false;
		glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);
		glDisable(GL_TEXTURE_2D);
		glColor4ub(c.r, c.g, c.b, c.a);
		if (fill)
			glBegin(GL_POLYGON);
		else
			glBegin(GL_LINE_STRIP);
				for (float v = 0.0f; v < 60.0f; v++)
				{
					float angle = (v * 2.0f * PI) / 60;
					glVertex2f(x + (cosf(angle) * radius), y + (sinf(angle) * radius));
				}
				if (!fill)
					glVertex2f(x + radius, y);
		glEnd();
		glPopAttrib();
		return true;
	}
	bool CSurface::draw_texture(const char* p_name, float x, float y, float sub_x, float sub_y, float sub_width, float sub_height)
	{
		if (g_p_openGL->get_mode() == COpenGL::_3D)
			return false;
		glPushMatrix();
		glPushAttrib(GL_CURRENT_BIT);
		glColor4ub(255, 255, 255, 255);
		glLoadIdentity();
		glTranslatef(x, y, 0.0f);
		if (!g_p_tex_mgr->raster_origin_leftbottom(p_name, sub_x, sub_y, sub_width, sub_height))
		{
			glPopMatrix();
			glPopAttrib();
			return false;
		}
		glPopMatrix();
		glPopAttrib();
		return true;
	}
	bool CSurface::draw_sprite(const char* p_name, float x, float y, float sub_x, float sub_y, float sub_width, float sub_height)
	{
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0);		// alpha 值大于 0 的像素才会被绘制
		if (!draw_texture(p_name, x, y, sub_x, sub_y, sub_width, sub_height))
		{
			glDisable(GL_ALPHA_TEST);
			return false;
		}
		glDisable(GL_ALPHA_TEST);
		return true;
	}
}
