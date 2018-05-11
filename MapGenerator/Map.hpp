#ifndef _MAP_HPP
#define _MAP_HPP 1

#include "Vector2.hpp"
#include "Coord.hpp"

#include <vector>
#include <ostream>

enum class TileInfo
{
    Path,
    Intermed,
    Major,
    Additional,
};

struct Tile
{
	Tile();
    int m_state;
    Vector2 m_st_Coord;
    void mt_Set_Flag(TileInfo flag)
    {
        m_state |= (1 << (int)flag);
    }
    bool mt_Is_Set(TileInfo flag) const
    {
        return ((m_state >> (int)flag) == 1);
    }
};

class Map
{
public:
    Map();
    virtual ~Map();
    void mt_Print(std::ostream& rcl_Stream);

    Tile& operator[](const Vector2& rcst_Coord);
    const Tile& operator[](const Vector2& rcst_Coord) const;
    Tile& mt_Get(const Vector2& rcst_Coord);
    const Tile& mt_Get(const Vector2& rcst_Coord) const;

    void mt_Set_Size(std::size_t x, std::size_t y);
    void mt_Set_Size(const Vector2& rcst_Size);
    void mt_Reset(void);
    Vector2 mt_Get_Size(void) const;

    Vector2 mt_Check_Coord(Vector2 rst_Coord) const;

protected:

    std::vector<std::vector<Tile>> m_ttst_Map;

};

#endif // _MAP_HPP
