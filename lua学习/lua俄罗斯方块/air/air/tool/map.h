// 2D矩形地图
#ifndef map_h_
#define map_h_
#include <string>
using std::string;
namespace air
{
	class CMap
	{
	private:
		string m_tex;				// 纹理索引名
		unsigned int* m_p_map;		// 图块图像
		unsigned int* m_p_flag;		// 图块属性
		unsigned int m_tile_size;	// 正方形图块
		unsigned int m_tile_num_x;	// X 方向图块数
		unsigned int m_tile_num_y;	// Y 方向图块数
		struct View
		{
			unsigned int left, right, bottom, top;
			unsigned int width, hight;
		};
		View m_view;				// 视口
		unsigned int m_draw_num_x;	// X 方向要绘制的图块数
		unsigned int m_draw_num_y;	// Y 方向要绘制的图块数
	public:
		CMap();
		~CMap();
		// 根据配置初始化地图
        // p_config：	配置文件
        // view_x：	视口宽
        // view_y：	视口高
		bool init(const char* p_config, unsigned int view_w, unsigned int view_h);
		// 移动视口
        // x，y：	移动量
		void scroll(int x, int y);
		// 绘制地图
		void draw();
	private:
		CMap(const CMap&);
		CMap& operator=(const CMap&);
	};
}

#endif
