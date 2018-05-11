#include "MapGenerator.hpp"
#include "Map.hpp"
#include "RandomGenerator.hpp"
#include "Linker.hpp"

#include <cstdlib>

bool MapGenerator::mt_Generate(int x_size, int y_size,
								const Vector2& rccl_Input,
								const Vector2& rccl_End,
								const MapGeneratorData& rcst_Data,
								RandomGenerator& rccl_Randomizer)
{
    bool l_b_Ret(false);
    Tile* l_pcl_Current(nullptr);
    Tile* l_pcl_Next(nullptr);
    PolarCoord l_st_Polar(0.0f, 0.0f);
    Vector2 l_Tmp(0, 0);

    mt_Set_Size(x_size, y_size);
    mt_Reset();

    l_pcl_Current = &mt_Get(rccl_Input);
    l_pcl_Next = &mt_Get(rccl_End);

    l_pcl_Current->mt_Set_Flag(TileInfo::Major);
    l_pcl_Next->mt_Set_Flag(TileInfo::Major);

    do
	{
		l_st_Polar.mt_Set(rccl_End - l_pcl_Current->m_st_Coord);

		if (l_st_Polar.m_modulus > rcst_Data.m_st_Intermed.m_modulus_max)
		{
			l_st_Polar.m_modulus = rccl_Randomizer.mt_Generate(rcst_Data.m_st_Intermed.m_modulus_min, rcst_Data.m_st_Intermed.m_modulus_max);
			l_st_Polar.mt_Set_Angle_Deg(l_st_Polar.mt_Get_Angle_Deg() + rccl_Randomizer.mt_Generate(-rcst_Data.m_st_Intermed.m_delta_deg, rcst_Data.m_st_Intermed.m_delta_deg));
			l_Tmp = l_st_Polar.mt_To_Cartesian() + l_pcl_Current->m_st_Coord;

            l_Tmp = mt_Check_Coord(l_Tmp);
		}
		else
		{
			l_Tmp = rccl_End;
		}
		l_pcl_Next = &mt_Get(l_Tmp);

		mt_Link_Coords_Bezier(*l_pcl_Current, *l_pcl_Next, rcst_Data.m_st_Bezier, rccl_Randomizer);

		l_pcl_Current = l_pcl_Next;
	} while(l_pcl_Current->m_st_Coord != rccl_End);

	mt_Manage_Additional_Points(rcst_Data, rccl_Randomizer);

    return l_b_Ret;
}

void MapGenerator::mt_Link_Coords_Linear(Tile& rcl_Current, Tile& rcl_Next, bool x_first)
{
    CoordLinker_Linear l_cl_Linker;

    l_cl_Linker.mt_Set_x_First(x_first);

    l_cl_Linker.mt_Link(*this, rcl_Current, rcl_Next);
}

void MapGenerator::mt_Link_Coords_Bezier(Tile& rcl_Current, Tile& rcl_Next, const MapGeneratorData_Bezier& rcst_Bezier, RandomGenerator& rccl_Randomizer)
{
    CoordLinker_Bezier l_cl_Bezier;
    std::vector<Vector2> l_tst_Ctrl_Points;
    PolarCoord l_st_Polar(0, 0);
    int l_distance;
    int l_angle_deg;

    l_distance = rcl_Current.m_st_Coord.mt_Get_Euclidean_Distance(rcl_Next.m_st_Coord);

    l_st_Polar.mt_Set(rcl_Next.m_st_Coord - rcl_Current.m_st_Coord);

    l_st_Polar.mt_Set_Angle_Deg(l_st_Polar.mt_Get_Angle_Deg() + rccl_Randomizer.mt_Generate(-rcst_Bezier.m_delta_deg, rcst_Bezier.m_delta_deg));
    l_st_Polar.m_modulus = rccl_Randomizer.mt_Generate(2, l_distance);

    l_tst_Ctrl_Points.push_back(l_st_Polar.mt_To_Cartesian() + rcl_Current.m_st_Coord);
    l_tst_Ctrl_Points.push_back(l_tst_Ctrl_Points.back());

    l_cl_Bezier.mt_Set_Ctrl_Points(l_tst_Ctrl_Points);

    l_cl_Bezier.mt_Link(*this, rcl_Current, rcl_Next);
}

void MapGenerator::mt_Get_Intermed(std::vector<Vector2>& rtst_Intermed) const
{
    rtst_Intermed.resize(0);
    for (const auto& data : m_ttst_Map)
    {
        for (const auto& cell : data)
        {
            if (cell.mt_Is_Set(TileInfo::Intermed) || cell.mt_Is_Set(TileInfo::Major))
            {
                rtst_Intermed.push_back(cell.m_st_Coord);
            }
        }
    }
}

void MapGenerator::mt_Manage_Additional_Points(const MapGeneratorData& rcst_Data, RandomGenerator& rccl_Randomizer)
{
    std::vector<Vector2> l_tst_Intermed;
    PolarCoord l_st_Polar(0, 0);
    std::size_t l_index;
    Vector2 l_st_Cartesian;
    Tile* l_pst_Start(nullptr);
    Tile* l_pst_End(nullptr);

    mt_Get_Intermed(l_tst_Intermed);

    for (std::size_t i = 0; (i < rcst_Data.m_st_Sign_Points.m_tst_Points.size()) && (l_tst_Intermed.size() != 0); i++)
    {
        l_index = rccl_Randomizer.mt_Generate(0, l_tst_Intermed.size() - 1);

        l_st_Cartesian = l_tst_Intermed[l_index];
        l_tst_Intermed.erase(l_tst_Intermed.begin() + l_index);
        l_pst_Start = &mt_Get(l_st_Cartesian);

        l_st_Polar.mt_Set_Angle_Deg(rccl_Randomizer.mt_Generate(-180, 180));
        l_st_Polar.m_modulus = rccl_Randomizer.mt_Generate(rcst_Data.m_st_Intermed.m_modulus_min, rcst_Data.m_st_Intermed.m_modulus_max);

        l_st_Cartesian = mt_Check_Coord(l_st_Cartesian + l_st_Polar.mt_To_Cartesian());
        l_pst_End = &mt_Get(rcst_Data.m_st_Sign_Points.m_tst_Points[i]);
        l_pst_End->mt_Set_Flag(TileInfo::Additional);

        mt_Link_Coords_Bezier(*l_pst_Start, *l_pst_End, rcst_Data.m_st_Bezier, rccl_Randomizer);
    }

    /*for (std::size_t i = 0; (i < rcst_Data.m_st_Sign_Points.m_count) && (l_tst_Intermed.size() != 0); i++)
    {
        l_index = rccl_Randomizer.mt_Generate(0, l_tst_Intermed.size() - 1);

        l_st_Cartesian = l_tst_Intermed[l_index];
        l_tst_Intermed.erase(l_tst_Intermed.begin() + l_index);
        l_pst_Start = &mt_Get(l_st_Cartesian);

        l_st_Polar.mt_Set_Angle_Deg(rccl_Randomizer.mt_Generate(-180, 180));
        l_st_Polar.m_modulus = rccl_Randomizer.mt_Generate(rcst_Data.m_st_Sign_Points.m_modulus_min, rcst_Data.m_st_Sign_Points.m_modulus_max);

        l_st_Cartesian = mt_Check_Coord(l_st_Cartesian + l_st_Polar.mt_To_Cartesian());
        l_pst_End = &mt_Get(l_st_Cartesian);
        l_pst_End->mt_Set_Flag(TileInfo::Additional);

        mt_Link_Coords_Bezier(*l_pst_Start, *l_pst_End, rcst_Data.m_st_Bezier, rccl_Randomizer);
    }*/
}
