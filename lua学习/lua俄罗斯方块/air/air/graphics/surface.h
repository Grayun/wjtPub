#ifndef surface_h_
#define surface_h_

#include "openGL.h"

namespace air
{
	class COpenGL
	{
	public:
		// 2D 模式： 屏幕坐标系为笛卡尔坐标系第二象限，左下角为原点
		// 3D 模式： 右手坐标系
		enum Mode	{ _2D, _3D };
	private:
		HGLRC m_hrc;
		int	  m_width, m_height;
		Mode  m_mode;
		void _init();
	public:
		/*
			构建 OpenGL 渲染系统
			hdc：			设备上下文
			width，height：	窗口尺寸
		*/
		COpenGL(HDC hdc, int width, int height);
		~COpenGL();
		/*
			窗口尺寸改变
			new_width，new_height：	新的窗口尺寸
		*/
		void resize(int new_width, int new_height);
		/*
			每帧调用，清屏和错误检测
		*/
		void frame() const;
		/*
			设置渲染模式
		*/
		void twoD();
		void threeD();
		/*
			交换缓存
			hdc：		进行交换的设备上下文
		*/
		void swap_buff(HDC hdc) const;
		/*
			返回渲染模式
		*/
		Mode get_mode() const;
		/*
			得到视口高
		*/
		int get_height() const;
	private:
		COpenGL();
		COpenGL(const COpenGL&);
		COpenGL& operator =(const COpenGL&);
	};
	extern COpenGL* g_p_openGL;

	//////////////////////////////////////////////////////////////////////////
	class CSurface
	{
	public:
		CSurface();
		~CSurface();
		/*
			绘制线段
			start_x，start_y：	线段起点
			end_x，end_y：		线段终点
			color：				颜色
			size：				线段粗细
		*/
		bool draw_line(float start_x, float start_y, float end_x, float end_y, color c, float size = 1.0f);
		/*
			绘制矩形
			x，y：			矩形坐下角坐标
			width，height：	矩形宽和高
			color：			颜色
			fill：			是否填充
		*/
		bool draw_rect(float x, float y, float width, float height, color c, bool fill = true);
		/*
			绘制圆
			x，y：		圆心坐标
			radius：	半径
			color：		颜色
			fill：		是否填充
		*/
		bool draw_circle(float x, float y, float radius, color c, bool fill = true);
		/*
			绘制纹理
			p_name：				纹理索引名
			x，y：					屏幕坐标
			sub_x，sub_y：			纹理起点坐标
			sub_width，sub_height：	纹理尺寸
		*/
		bool draw_texture(const char* p_name, float x, float y, float sub_x = 0.0f, float sub_y  = 0.0f, float sub_width = 0.0f, float sub_height = 0.0f);
		/*
			绘制子画面，带有 alpha 测试
			p_name：				纹理索引名
			x，y：					渲染屏幕位置
			sub_x，sub_y：			纹理起点坐标
			sub_width，sub_height：	纹理尺寸
		*/
		bool draw_sprite(const char* p_name, float x, float y, float sub_x = 0.0f, float sub_y = 0.0f, float sub_width = 0.0f, float sub_height = 0.0f);
	private:
		CSurface(const CSurface&);
		CSurface& operator=(const CSurface&);
	};
	extern CSurface* g_p_surface;
}

#endif
