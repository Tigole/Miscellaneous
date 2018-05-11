#include "Map.hpp"
#include <cmath>
#include <iomanip>

Tile::Tile()
 :	m_state(0),
	m_st_Coord(-1, -1)
{}

Map::Map()
 :  m_ttst_Map()
{}

Map::~Map()
{}

void Map::mt_Set_Size(std::size_t x, std::size_t y)
{
    m_ttst_Map.resize(y);
    for (std::size_t i = 0; i < y; i++)
    {
    	m_ttst_Map[i].resize(x);
        for (std::size_t j = 0; j < x; j++)
        {
            m_ttst_Map[i][j].m_state = 0;
            m_ttst_Map[i][j].m_st_Coord.m_x = j;
            m_ttst_Map[i][j].m_st_Coord.m_y = i;
        }
    }
}

void Map::mt_Set_Size(const Vector2& rcst_Size)
{
    mt_Set_Size(rcst_Size.m_x, rcst_Size.m_y);
}

void Map::mt_Reset(void)
{
    for (auto& toto : m_ttst_Map)
    {
        for (auto& brol : toto)
        {
            brol.m_state = 0;
        }
    }
}

Vector2 Map::mt_Get_Size(void) const
{
    Vector2 l_st_Ret;

    l_st_Ret.m_y = m_ttst_Map.size();
    l_st_Ret.m_x = m_ttst_Map.front().size();

    return l_st_Ret;
}

void Map::mt_Print(std::ostream& rcl_Stream)
{
    rcl_Stream << std::hex;
    for (auto& col : m_ttst_Map)
    {
        for (auto& cell : col)
        {
            rcl_Stream << std::setw(2) << cell.m_state;
        }
        rcl_Stream << '\n';
    }
    rcl_Stream << '\n';
}

Tile& Map::operator[](const Vector2& rcst_Coord)
{
    return mt_Get(rcst_Coord);
}

const Tile& Map::operator[](const Vector2& rcst_Coord) const
{
    return mt_Get(rcst_Coord);
}

Tile& Map::mt_Get(const Vector2& rcst_Coord)
{
    return m_ttst_Map[rcst_Coord.m_y][rcst_Coord.m_x];
}

const Tile& Map::mt_Get(const Vector2& rcst_Coord) const
{
    return m_ttst_Map[rcst_Coord.m_y][rcst_Coord.m_x];
}

Vector2 Map::mt_Check_Coord(Vector2 rst_Coord) const
{
    if (rst_Coord.m_x < 0)
        rst_Coord.m_x = 0;
    if (rst_Coord.m_x >= m_ttst_Map.front().size())
        rst_Coord.m_x = m_ttst_Map.front().size()-1;
    if (rst_Coord.m_y < 0)
        rst_Coord.m_y = 0;
    if (rst_Coord.m_y >= m_ttst_Map.size())
        rst_Coord.m_y = m_ttst_Map.size()-1;

    return rst_Coord;
}
