#include "Linker.hpp"


CoordLinker_Linear::CoordLinker_Linear()
 :  m_x_first(false)
{}

void CoordLinker_Linear::mt_Set_x_First(bool x_first)
{
    m_x_first = x_first;
}

void CoordLinker_Linear::mt_Link(Map& rcl_Map, Tile& rcl_Input, Tile& rcl_Output)
{
    Tile* l_pcl_Tmp(nullptr);
    Vector2 l_Tmp(rcl_Output.m_st_Coord);

    rcl_Input.mt_Set_Flag(TileInfo::Intermed);
    rcl_Output.mt_Set_Flag(TileInfo::Intermed);

    if (m_x_first)
    {
    	l_Tmp.m_y = rcl_Input.m_st_Coord.m_y;
    }
    else
    {
    	l_Tmp.m_x = rcl_Input.m_st_Coord.m_x;
    }

	l_pcl_Tmp = &rcl_Map[l_Tmp];
	mt_Linear_Link(rcl_Map, rcl_Input, *l_pcl_Tmp);
	mt_Linear_Link(rcl_Map, *l_pcl_Tmp, rcl_Output);
}

void CoordLinker_Linear::mt_Linear_Link(Map& rcl_Map, Tile& rcl_Input, Tile& rcl_Output)
{
	Tile* l_pcl_Current(&rcl_Input);

	while(l_pcl_Current->m_st_Coord != rcl_Output.m_st_Coord)
	{
		Vector2 l_Delta(rcl_Output.m_st_Coord - l_pcl_Current->m_st_Coord);

		if (l_Delta.m_x != 0)
		{
			l_Delta.m_y = 0;
			l_Delta.m_x /= std::abs(l_Delta.m_x);
		}
		else
		{
			l_Delta.m_y /= std::abs(l_Delta.m_y);
		}
		l_pcl_Current->mt_Set_Flag(TileInfo::Path);

		l_pcl_Current = &rcl_Map[l_pcl_Current->m_st_Coord + l_Delta];
	}

	rcl_Output.mt_Set_Flag(TileInfo::Path);
}

CoordLinker_Bezier::CoordLinker_Bezier()
 :  m_tst_Ctrl_Points(),
    m_step(0.0001f)
{}

void CoordLinker_Bezier::mt_Set_Ctrl_Points(const std::vector<Vector2>& tcst_Ctrl_Points)
{
    m_tst_Ctrl_Points = tcst_Ctrl_Points;
}

void CoordLinker_Bezier::mt_Set_Step(float step)
{
    m_step = step;
}

void CoordLinker_Bezier::mt_Link(Map& rcl_Map, Tile& rcl_Input, Tile& rcl_Output)
{
    std::vector<Vector2> l_tst_Points(m_tst_Ctrl_Points);

    rcl_Input.mt_Set_Flag(TileInfo::Intermed);
    rcl_Output.mt_Set_Flag(TileInfo::Intermed);

    l_tst_Points.insert(l_tst_Points.begin(), rcl_Input.m_st_Coord);
    l_tst_Points.push_back(rcl_Output.m_st_Coord);

    if (l_tst_Points.size() == 2)
        mt_Link_Linear(rcl_Map, l_tst_Points);
    else if (l_tst_Points.size() == 3)
        mt_Link_Quadratic(rcl_Map, l_tst_Points);
    else if (l_tst_Points.size() == 4)
        mt_Link_Cubic(rcl_Map, l_tst_Points);
}

void CoordLinker_Bezier::mt_Link_Linear(Map& rcl_Map, const std::vector<Vector2>& tcst_Ctrl_Points)
{
    Vector2 l_st_Current(0, 0);
    float l_param(0.0f), l_delta(m_step);

    do
    {
        l_st_Current.m_x = (1 - l_param) * tcst_Ctrl_Points[0].m_x + l_param * tcst_Ctrl_Points[1].m_x;
        l_st_Current.m_y = (1 - l_param) * tcst_Ctrl_Points[0].m_y + l_param * tcst_Ctrl_Points[1].m_y;

        l_param += l_delta;

        rcl_Map[l_st_Current].mt_Set_Flag(TileInfo::Path);
    } while (l_param < 1.0f);
}

void CoordLinker_Bezier::mt_Link_Quadratic(Map& rcl_Map, const std::vector<Vector2>& tcst_Ctrl_Points)
{
    Vector2 l_st_Current(0, 0);
    float l_param(0.0f), l_delta(m_step);
    float l_x, l_y;

    do
    {
        l_x =      (1 - l_param) * (1 - l_param) * tcst_Ctrl_Points[0].m_x
                +   2 * (1 - l_param) * l_param * tcst_Ctrl_Points[1].m_x;
                +   l_param * l_param * tcst_Ctrl_Points[2].m_x;
        l_y =      (1 - l_param) * (1 - l_param) * tcst_Ctrl_Points[0].m_y
                +   2 * (1 - l_param) * l_param * tcst_Ctrl_Points[1].m_y
                +   l_param * l_param * tcst_Ctrl_Points[2].m_y;

        l_st_Current.m_x = l_x;
        l_st_Current.m_y = l_y;

        l_param += l_delta;

        rcl_Map[l_st_Current].mt_Set_Flag(TileInfo::Path);
    } while (l_param < 1.0f);
}

void CoordLinker_Bezier::mt_Link_Cubic(Map& rcl_Map, const std::vector<Vector2>& tcst_Ctrl_Points)
{
    Vector2 l_st_Current(0, 0);
    float l_param(0.0f), l_delta(m_step);
    float l_x, l_y;

    do
    {
        l_x =      (1 - l_param) * (1 - l_param) * (1 - l_param) * tcst_Ctrl_Points[0].m_x
                +   3 * l_param  * (1 - l_param) * (1 - l_param) * tcst_Ctrl_Points[1].m_x
                +   3 * l_param * l_param * (1 - l_param) * tcst_Ctrl_Points[2].m_x
                +   l_param * l_param * l_param * tcst_Ctrl_Points[3].m_x;
        l_y =      (1 - l_param) * (1 - l_param) * (1 - l_param) * tcst_Ctrl_Points[0].m_y
                +   3 * l_param  * (1 - l_param) * (1 - l_param) * tcst_Ctrl_Points[1].m_y
                +   3 * l_param * l_param * (1 - l_param) * tcst_Ctrl_Points[2].m_y
                +   l_param * l_param * l_param * tcst_Ctrl_Points[3].m_y;

        l_st_Current.m_x = l_x;
        l_st_Current.m_y = l_y;

        l_param += l_delta;

        rcl_Map[rcl_Map.mt_Check_Coord(l_st_Current)].mt_Set_Flag(TileInfo::Path);
    } while (l_param < 1.0f);
}
