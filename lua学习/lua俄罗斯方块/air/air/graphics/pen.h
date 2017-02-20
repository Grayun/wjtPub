#ifndef pen_h_
#define pen_h_

#include "openGL.h"
#include <map>
#include <string>
using std::map;
using std::string;

namespace air
{
	class CPenManager
	{
	private:
		struct Pen
		{
			unsigned int list_id;
			HFONT		 hfont;
		};
		typedef map<string, Pen>	PenMap;
		PenMap m_pens;
		void _free();	// 释放内存
		void _draw(unsigned int id, float x, float y, unsigned char r, unsigned char g, unsigned char b, string msg);	// 绘制文本
	public:
		CPenManager();
		~CPenManager();
		/*
			创建一种字体
			name：	字体索引名
			size：	字体尺寸
			style：	字体风格名称
			space：	字体平均间隔
			weight：字体粗细
		*/
		bool create(string name, int size, string style, int space = 0, int weight = FW_BOLD);
		/*
			从配置中创建画笔
			config：	配置文件
		*/
		bool create_from_config(string config);
		/*
			绘制文本
			name：	字体索引名
			x，y：	位置
			c：	    颜色
			msg：	内容
		*/
		bool print(string name, float x, float y, color c, string msg);
		/*
			删除字体
			name：	字体索引名
		*/
		void destroy(string name);
		/*
			获取字体信息
			name：	字体索引名
		*/
		unsigned int get_char_height(string name);
		// 得到指定字符串宽度
		// name：字体索引名
		// msg：指定字符串
		unsigned int get_str_width(string name, string msg);
	private:
		CPenManager(const CPenManager&);
		CPenManager& operator=(const CPenManager&);
	};
	extern CPenManager* g_p_pen_mgr;
}

#endif
