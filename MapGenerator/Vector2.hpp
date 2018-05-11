#ifndef _VECTOR2_HPP
#define _VECTOR2_HPP 1

#include <cmath>

namespace vec
{

template<typename T>
struct Vector2
{
	Vector2() : m_x(), m_y(){}
	Vector2(const T& x, const T& y) : m_x(x), m_y(y){}

	T m_x, m_y;

	T mt_Get_Euclidean_Distance(const Vector2<T>& b)
	{
		T l_x(b.m_x - m_x), l_y(b.m_y - m_y);

		return std::sqrt(l_x*l_x + l_y*l_y);
	}

	T mt_Get_Manhattan_Distance(const Vector2<T>& b)
	{
		return (std::abs(b.m_x - m_x) + std::abs(b.m_y - m_y));
	}

    Vector2<T>& operator+=(const Vector2<T>& a)
    {
        *this = ((*this) + a);
        return *this;
    }
};

template<typename T>
Vector2<T> operator+(const Vector2<T>& a, const Vector2<T>& b)
{
	return Vector2<T>(a.m_x + b.m_x, a.m_y + b.m_y);
}

template<typename T>
Vector2<T> operator-(const Vector2<T>& a, const Vector2<T>& b)
{
	return Vector2<T>(a.m_x - b.m_x, a.m_y - b.m_y);
}

template<typename T>
bool operator==(const Vector2<T>& a, const Vector2<T>& b)
{
	return ((a.m_x == b.m_x) && (a.m_y == b.m_y));
}

template<typename T>
bool operator!=(const Vector2<T>& a, const Vector2<T>& b)
{
	return !(a == b);
}

}

#endif // _VECTOR2_HPP
