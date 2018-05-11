#ifndef _LINKER_HPP
#define _LINKER_HPP 1

#include "Map.hpp"

#include <vector>

class ICoordLinker
{
public:
    virtual ~ICoordLinker(){};

    virtual void mt_Link(Map& rcl_Map, Tile& rcl_Input, Tile& rcl_Output) = 0;
};

class CoordLinker_Linear : public ICoordLinker
{
public:
    CoordLinker_Linear();

    void mt_Set_x_First(bool x_first);

    virtual void mt_Link(Map& rcl_Map, Tile& rcl_Input, Tile& rcl_Output);

protected:
    void mt_Linear_Link(Map& rcl_Map, Tile& rcl_Input, Tile& rcl_Output);
    bool m_x_first;
};

class CoordLinker_Bezier
{
public:
    CoordLinker_Bezier();

    void mt_Set_Ctrl_Points(const std::vector<Vector2>& tcst_Ctrl_Points);
    void mt_Set_Step(float step);

    virtual void mt_Link(Map& rcl_Map, Tile& rcl_Input, Tile& rcl_Output);

protected:

    void mt_Link_Linear(Map& rcl_Map, const std::vector<Vector2>& tcst_Ctrl_Points);
    void mt_Link_Quadratic(Map& rcl_Map, const std::vector<Vector2>& tcst_Ctrl_Points);
    void mt_Link_Cubic(Map& rcl_Map, const std::vector<Vector2>& tcst_Ctrl_Points);

    std::vector<Vector2> m_tst_Ctrl_Points;
    float m_step;
};

#endif // _LINKER_HPP
