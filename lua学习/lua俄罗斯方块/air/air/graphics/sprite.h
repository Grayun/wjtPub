#ifndef sprite_h_
#define sprite_h_

#include <string>
using std::string;

namespace air
{
	class CSprite
	{
    protected:
		string m_image;
		float	 m_x, m_y, m_z;
		float	 m_w, m_h;
		float	 m_angle;
		unsigned char	m_alpha;
        struct aabb
        {
            float left, right, bottom, top;
        };
        aabb m_aabb;
	public:
        CSprite();
		virtual ~CSprite();
		// 初始化精灵
		// image：	纹理名
		// x，y：	位置
		// w，h：	尺寸
		void init(string image, float x, float y, float w, float h);
		// 移动精灵
		// x，y：	移动量
		void move(float x, float y);
		// 直接将精灵放到指定位置
		// x，y：	位置
		void put(float x, float y);
		// 以中心为原点旋转精灵
		// angle：	角度
		void rotate(float angle);
		// 改变透明度
		// alpha：	alpha值改变量
		void fade(int alpha);
		// 绘制精灵
		virtual void draw();
		// 访问器
		float pos_x() const;
		float pos_y() const;
		float width() const;
		float height() const;
		// 初始化碰撞矩形
		// left，right，bottom，top：碰撞盒的边界
		void init_aabb(float left, float right, float bottom, float top);
		// 访问碰撞矩形
		aabb box() const
		{
            return m_aabb;
		}
		// 碰撞检测
		// spr： 进行检测的精灵
		// 返回值： 是否发生碰撞
		bool is_collide(const CSprite& spr) const;
		void debug_draw_aabb() const;
	private:
		CSprite(const CSprite&);
		CSprite& operator=(const CSprite&);
	};
	//////////////////////////////////////////////////////////////////////////
	class CTimer;
	class CAniSprite : public CSprite
	{
	public:
		enum Sequence { X, Y };  // 动画帧以什么方式存放
	private:
		CTimer* m_p_pace_controller;              // 动画速度控制
		unsigned int m_interval_ms;			    // 帧间隔
		unsigned int m_cur_x, m_cur_y;		    // 当前帧
		unsigned int m_total_x, m_total_y;	    // 总列数，总行数
		unsigned int m_frame_w, m_frame_h;	// 帧宽，帧高
		Sequence m_seq;						        // 帧序列
	public:
		CAniSprite();
		~CAniSprite();
		// 初始化动画参数
		// ms：              每帧间隔毫秒
		// total_x，total_y：总列数，总行数
		// seq：             帧排列方式
		void init_ani(unsigned int ms, unsigned int total_x, unsigned int total_y, Sequence seq);
		// 设置要播放的动画帧序列
		// n：播放第几个帧序列
		void set_sequence(unsigned int idx);
		// 绘制精灵
		virtual void draw();
	private:
        void _draw(unsigned int x, unsigned int y);
		CAniSprite(const CAniSprite&);
		CAniSprite& operator=(const CAniSprite&);
	};
}

#endif
