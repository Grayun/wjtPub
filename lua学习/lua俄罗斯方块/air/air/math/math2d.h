#ifndef math2d_h_
#define math2d_h_
#include <math.h>
namespace air
{
    namespace math
    {
        // 2d vector /////////////////////////////////////////////////////
        struct vec2
        {
            float x, y;
            vec2()
            {
                x = y = 0.0f;
            }
            vec2(float _x, float _y)
            {
                x = _x;
                y = _y;
            }
            float length() const
            {
                return sqrtf(x * x + y * y);
            }
            vec2 unitize() const
            {
                float l = length();
                return vec2(x / l, y / l);
            }
            vec2 operator-() const
            {
                return vec2(-x, -y);
            }
        };
        inline vec2 operator+(const vec2& a, const vec2& b)
        {
            return vec2(a.x + b.x, a.y + b.y);
        }
        inline vec2 operator-(const vec2& a, const vec2& b)
        {
            return vec2(a.x - b.x, a.y - b.y);
        }
        inline vec2 operator*(const vec2& v, float f)
        {
            return vec2(v.x * f, v.y * f);
        }
        inline float dot(const vec2& a, const vec2& b)
        {
            return a.x * b.x + a.y * b.y;
        }
        inline float cross(const vec2& a, const vec2& b)
        {
            return a.x * b.y - a.y * b.x;
        }
        // 3*3 matrix /////////////////////////////////////////////////////
        struct mat3x3
        {
            float m11, m12, m13;
            float m21, m22, m23;
            float m31, m32, m33;
            mat3x3()
            {
                m11 = 1.0f; m12 = 0.0f; m13 = 0.0f;
                m21 = 0.0f; m22 = 1.0f; m23 = 0.0f;
                m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
            }
        };
        inline mat3x3 bulid_rotate(float angle)
        {
            mat3x3 m;
            m.m11 = cosf(angle);	m.m12 = sinf(angle);	m.m13 = 0.0f;
            m.m21 = -sinf(angle);	m.m22 = cosf(angle);	m.m23 = 0.0f;
            m.m31 = 0.0f;			    m.m32 = 0.0f;			    m.m33 = 1.0f;
            return m;
        }
        inline mat3x3 bulid_move(float x, float y)
        {
            mat3x3 m;
            m.m11 = 1.0f;	m.m12 = 0.0f;	m.m13 = 0.0f;
            m.m21 = 0.0f;	m.m22 = 1.0f;  m.m23 = 0.0f;
            m.m31 = x;	    m.m32 = y;	    m.m33 = 1.0f;
            return m;
        }
        inline vec2 operator*(const vec2& v, const mat3x3& m)
        {
            return vec2(v.x * m.m11 + v.y * m.m21 + m.m31, v.x * m.m12 + v.y * m.m22 + m.m32);
        }
        // angle <-> radians /////////////////////////////////////////////////////
        const float PI = 3.141592654f;
        inline float to_radians(float angle)
        {
            return angle * (PI / 180.0f);
        }
        inline float to_angle(float radians)
        {
            return 180.0f * radians / PI;
        }
    }
}
#endif
