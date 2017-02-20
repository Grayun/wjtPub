#include "phy2d.h"
#include <math.h>
namespace air
{
    namespace phy
    {
        // CPoint /////////////////////////////////////////////////////
        CPoint::CPoint()
        {
            m_position = vec2(0.0f, 0.0f);
            m_velocity = vec2(0.0f, 0.0f);
            m_mass = m_inv_mass = 0.0f;
            m_acc_force = vec2(0.0f, 0.0f);
        }
        CPoint::~CPoint()
        {
        }
        void CPoint::init(const vec2& pos, const vec2& vel, float mass)
        {
            m_position = pos;
            m_velocity = vel;
            m_mass = mass;
            m_inv_mass = 1 / mass;
        }
        void CPoint::add_force(const vec2& f)
        {
            if (f.x == 0.0f && f.y == 0.0f)
                return;
            m_acc_force = m_acc_force + f;
        }
        void CPoint::run(float dt)
        {
            vec2 a = m_acc_force * m_inv_mass * dt;   // a = f / m
            m_velocity = m_velocity + a;   // add velocity
            m_position = m_position + m_velocity * dt;  // update linear position
            m_acc_force = vec2(0.0f, 0.0f);   // reset force
        }
        vec2& CPoint::position()
        {
            return m_position;
        }
        vec2& CPoint::velocity()
        {
            return m_velocity;
        }
        float CPoint::mass()
        {
            return m_mass;
        }
        // CSpring /////////////////////////////////////////////////////
		CSpring::CSpring(CPoint* a, CPoint* b, float k, float rl, float fr)
		{
			m_pt1 = a;
			m_pt2 = b;
			m_k = k;
			m_reset_length = rl;
			m_friction = fr;
		}
		void CSpring::calculate_force()
		{
			vec2 v = m_pt1->position() - m_pt2->position();
			float r = v.length();
			vec2 f;
			if (r != 0.0f)
            {
                float d = fabsf(r - m_reset_length);
                f = f + (v.unitize() * d * -m_reset_length);   // f = -kx
                f = f + (m_pt1->velocity() - m_pt2->velocity()) * -m_friction;   // 摩擦力
                m_pt1->add_force(f);
                m_pt2->add_force(-f);
            }
		}
		// force /////////////////////////////////////////////////////
		CDrag::CDrag(float k1, float k2)
		{
		    m_k1 = k1;
		    m_k2 = k2;
		}
		void CDrag::run(CPoint* p)
		{
		    vec2 f = p->velocity();
		    float coeff = f.length();
		    coeff = m_k1 * coeff + m_k2 * coeff * coeff;
		    f = f.unitize();
		    f = f * -coeff;
		    p->add_force(f);
		}
		CGravity::CGravity(const vec2& g)
		{
		    m_g = g;
		}
		void CGravity::run(CPoint* p)
		{
		    p->add_force(m_g * p->mass());
		}
		CAnchoredSpring::CAnchoredSpring(const vec2& anchor, float constant, float rest_length)
		{
		    m_anchor = anchor;
		    m_constant = constant;
		    m_rest_length = rest_length;
		}
		void CAnchoredSpring::run(CPoint* p)
		{
		    vec2 f = p->position();
		    f = f - m_anchor;
		    float l = f.length();
		    if (l < m_rest_length)
                return;
            l -= m_rest_length;
            l *= m_constant;
            f = f.unitize();
            f = f * -l;
            p->add_force(f);
		}
		vec2 CAnchoredSpring::anchor() const
		{
		    return m_anchor;
		}
		// collide line /////////////////////////////////////////////////////
		CLine::CLine()   { m_l = 0.0f; }
		CLine::~CLine()   {}
		void CLine::set(const vec2& start, const vec2& end)
		{
		    m_s = start;
		    m_e = end;
		    vec2 dir = end - start;
		    m_nor = vec2(-dir.y, dir.x);
		    m_nor = m_nor.unitize();
		    m_l = dir.length();
		}
		vec2 CLine::rebound(const vec2& v)
		{
		    float f = math::dot(v, m_nor);
		    vec2 r = m_nor * (-2.0f * f) + v;   // f = 2n + i
		    return r;
		}
        static float dis(const vec2& a, const vec2& b)
        {
            return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
        }
		float CLine::mindist(const vec2& v)
		{
		    float d1 = dis(v, m_s);
            float d2 = dis(v, m_e);
            if (m_l <= 0.00001f)
                return d1;
            if (d1 <= 0.00001f || d2 <= 0.00001f)
                return 0.0f;
            if (d1 * d1 >= d2 * d2 + m_l * m_l)
                return d2;
            if (d2 * d2 >= d1 * d1 + m_l * m_l)
                return d1;
            float l = (d1 + d2 + m_l) / 2;
            float area = sqrtf(l * (l - d1) * (l - d2) * (l - m_l));   // 海伦公式求面积
            return 2 * area / m_l;   // 面积 = 底 * 高 / 2
		}
    }
}
