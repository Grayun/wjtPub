#ifndef texture_h_
#define texture_h_

#include <string>
#include <map>
using std::string;
using std::map;

namespace air
{
	class CTextureManager
	{
	public:
		struct Texture					// 纹理结构体	
		{
			unsigned int id;			// 纹理索引
			unsigned int width, height;	// 纹理尺寸
		};

	private:
		typedef map<string, Texture>	TexMap;
		TexMap m_texs;

		void _free();

	public:
		CTextureManager();
		~CTextureManager();

		/*
			载入指定透明色的 BMP 文件作为纹理
			p_file：	图像文件路径
			p_name：	纹理索引名
			r，g，b：	透明色，默认为紫色 RGB(128,0,128)
		*/
		bool load_bmp(const char* p_file, const char* p_name, unsigned char r = 128, unsigned char g = 0, unsigned char b = 128);

		/*
			载入 TGA 文件作为纹理
			p_file：	图像文件路径
			p_name：	纹理索引名
		*/
		bool load_tga(const char* p_file, const char* p_name);

		/*
			从配置文件中载入纹理
			p_config：	配置文件名
		*/
		bool load_from_config(const char* p_config);

		/*
			得到纹理的信息
			p_name：	纹理索引名
		*/
		unsigned int get_id(const char* p_name) const;
		unsigned int get_width(const char* p_name) const;
		unsigned int get_height(const char* p_name) const;
		const Texture* get_texture(const char* p_name) const;

		/*
			光栅化纹理，局部坐标原点为纹理中心（方便以纹理为中心旋转）
			p_name：				索引名
			sub_x，sub_y：			起点坐标
			sub_width，sub_height：	尺寸
		*/
		bool raster_origin_center(const char* p_name, float sub_x = 0.0f, float sub_y  = 0.0f, float sub_width = 0.0f, float sub_height = 0.0f) const;

		/*
			光栅化纹理，局部坐标原点为纹理左下角
			p_name：				索引名
			sub_x，sub_y：			起点坐标
			sub_width，sub_height：	尺寸
		*/
		bool raster_origin_leftbottom(const char* p_name, float sub_x = 0.0f, float sub_y  = 0.0f, float sub_width = 0.0f, float sub_height = 0.0f) const;

	private:
		CTextureManager(const CTextureManager&);
		CTextureManager& operator =(const CTextureManager&);
	};
	extern CTextureManager* g_p_tex_mgr;

	//////////////////////////////////////////////////////////////////////////
	// 图像字体
	class CImageFont
	{
	private:
		struct Font
		{
			unsigned int id;		// 显示列表ID
			unsigned int width;		// 字符宽
			unsigned int height;	// 字符高
			unsigned int tex_id;	// 纹理ID
		};
		Font m_font;
		unsigned int m_ascii_to_list[128];

		void _init_char_to_list(unsigned int start);
		void _print(char c);
		void _free();

	public:
		CImageFont();
		~CImageFont();

		/*
			创建一种图像字体，从纹理（外部位图）中创建字体
			p_tex_name：纹理名
		*/
		bool create(const char* p_tex_name);

		/*
			显示信息
			x，y：	位置
			msg：	信息
		*/
		void print(float x, float y, string msg);

	private:
		CImageFont(const CImageFont&);
		CImageFont& operator=(const CImageFont&);
	};
}

#endif