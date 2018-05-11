#ifndef _COORD_HPP
#define _COORD_HPP 1

#include "Vector2.hpp"

using Vector2 = vec::Vector2<int>;

struct PolarCoord
{
	PolarCoord(float modulus, float angle_deg);
	PolarCoord(const Vector2& cartesian_coord);
	void mt_Set(const Vector2& cartesian_coord);
	float mt_Get_Angle_Deg(void) const;
	void mt_Set_Angle_Deg(float angle_deg);
	Vector2 mt_To_Cartesian(void) const;
	float m_modulus;
	float m_angle_rad;
};

Vector2 operator+(const Vector2& cartesian, const PolarCoord& polar);


#endif // _COORD_HPP
