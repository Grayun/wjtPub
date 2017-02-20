#ifndef phy2d_h_
#define phy2d_h_
#include "../math/math2d.h"
namespace air
{
    namespace phy
    {
        using air::math::vec2;
        // CPoint /////////////////////////////////////////////////////
        class CPoint
        {
        private:
            vec2 m_position;   // 位置
            vec2 m_velocity;   // 速度
            float m_mass;   // 质量
            float m_inv_mass;   // 1/质量
            vec2 m_acc_force;   // 作用力之和
        public:
            CPoint();
            ~CPoint();
            //	初始化位置，速度，质量
            //	pos：	位置
            //	vel：	速度
            //	mass：	质量
            void init(const vec2& pos, const vec2& vel, float mass);
            //	添加一个作用于物体的外力
            //	f	外力
            void add_force(const vec2& f);
            //	更新
            //	dt：  控制系数
            void run(float dt);
            //	访问器，设置器
            vec2& position();
            vec2& velocity();
            float mass();
        private:
            CPoint(const CPoint&);
            CPoint& operator=(const CPoint&);
        };
        // CSpring /////////////////////////////////////////////////////
		class CSpring
		{
		private:
			CPoint* m_pt1;
			CPoint* m_pt2;
			float m_k;
			float m_reset_length;
			float m_friction;
		public:
			// 构造方法
			// a，b：质点
			// k：弹性系数
			// rl：原始长度
			// fr：摩擦力
			CSpring(CPoint* a, CPoint* b, float k, float rl, float fr);
			// 计算力
			void calculate_force();
		private:
			CSpring();
			CSpring(const CSpring&);
			CSpring& operator=(const CSpring&);
		};
		// force /////////////////////////////////////////////////////
		class IForceGen
		{
        public:
            IForceGen()   {};
            virtual void run(CPoint* p) = 0;   // p 代表受力作用的质点
        private:
            IForceGen(const IForceGen&);
            IForceGen& operator=(const IForceGen&);
		};
		class CDrag : public IForceGen   // 摩擦力
		{
        private:
            float m_k1, m_k2;
        public:
            CDrag(float k1, float k2);
            virtual void run(CPoint* p);
		};
		class CGravity : public IForceGen   // 重力
		{
        private:
            vec2 m_g;
        public:
            CGravity(const vec2& g);
            virtual void run(CPoint* p);
		};
		/*class CSpring : public IForceGen   // 弹力
		{
        private:
            CPoint* m_p_other;
            float m_constant;
            float m_rest_length;
        public:
            CSpring(const CPoint* p_other, float constant, float rest_length);
            virtual void run(CPoint* p);
		};*/
		class CAnchoredSpring : public IForceGen   // 锚点弹力
		{
        private:
            vec2 m_anchor;
            float m_constant, m_rest_length;
        public:
            CAnchoredSpring(const vec2& anchor, float constant, float rest_length);
            virtual void run(CPoint* p);
            vec2 anchor() const;
		};
		// collide line /////////////////////////////////////////////////////
		class CLine
		{
        public:
            vec2 m_s, m_e;   // 起点，终点
            vec2 m_nor;   // 法向量
            float m_l;   // 长度
        public:
            CLine();
            ~CLine();
            // 设置碰撞线段
            // start，end：起点，终点
            void set(const vec2& start, const vec2& end);
            // 反射向量
            // v：速度
            // 返回值：反射后的速度
            vec2 rebound(const vec2& v);
            // 点到线段的最近距离
            // v：点
            // 返回值：距离
            float mindist(const vec2& v);
        private:
            CLine(const CLine&);
            CLine& operator=(const CLine&);
		};
    }
}
#endif
