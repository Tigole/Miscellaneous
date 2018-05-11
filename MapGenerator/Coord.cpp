#include "Coord.hpp"


#include <cmath>

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif // M_PI

PolarCoord::PolarCoord(float modulus, float angle_deg)
 :	m_modulus(modulus), m_angle_rad(0.0f)
{
    mt_Set_Angle_Deg(angle_deg);
}

PolarCoord::PolarCoord(const Vector2& cartesian_coord)
 :	m_modulus(0.0f), m_angle_rad(0.0f)
{
    mt_Set(cartesian_coord);
}
void PolarCoord::mt_Set(const Vector2& cartesian_coord)
{
    Vector2 l_Origin(0, 0);
    m_modulus = l_Origin.mt_Get_Euclidean_Distance(cartesian_coord);
    m_angle_rad = std::atan((float)cartesian_coord.m_y / (float)cartesian_coord.m_x);
}

float PolarCoord::mt_Get_Angle_Deg(void) const
{
    return (m_angle_rad * 180 / M_PI);
}

void PolarCoord::mt_Set_Angle_Deg(float angle_deg)
{
    m_angle_rad = (angle_deg * M_PI) / 180;
}

Vector2 PolarCoord::mt_To_Cartesian(void) const
{
    return Vector2(	m_modulus * std::cos(m_angle_rad),
                    m_modulus * std::sin(m_angle_rad));
}

Vector2 operator+(const Vector2& cartesian, const PolarCoord& polar)
{
    return cartesian + polar.mt_To_Cartesian();
}
