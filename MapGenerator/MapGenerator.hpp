#ifndef _MAP_GENERATOR_HPP
#define _MAP_GENERATOR_HPP 1

#include "Map.hpp"
#include <unordered_set>

class RandomGenerator;

struct MapGeneratorData_Bezier
{
    int m_delta_deg;
};

struct MapGeneratorData_Intermed
{
	int m_modulus_max;
	int m_modulus_min;
    int m_delta_deg;
};

struct MapGeneratorData_SignifiantPoints
{
    /*int m_children_low;
    int m_count;
    int m_modulus_max;
    int m_modulus_min;*/
    std::vector<Vector2> m_tst_Points;
};

struct MapGeneratorData
{
    MapGeneratorData_SignifiantPoints m_st_Sign_Points;
    MapGeneratorData_Intermed m_st_Intermed;
    MapGeneratorData_Bezier m_st_Bezier;
};

class MapGenerator : public Map
{
public:

    void mt_Generate(int x_size, int y_size,
                     const std::vector<Vector2>& rctst_Nodes,
                     const MapGeneratorData& rcst_Data,
                     RandomGenerator& rccl_Randomizer);
    bool mt_Generate(int x_size, int y_size,
					const Vector2& rccl_Input,
					const Vector2& rccl_End,
					const MapGeneratorData& rcst_Data,
					RandomGenerator& rccl_Randomizer);

private:
    void mt_Link_Coords_Linear(Tile& rcl_Current, Tile& rcl_Next, bool x_first);
    void mt_Link_Coords_Bezier(Tile& rcl_Current, Tile& rcl_Next, const MapGeneratorData_Bezier& rcst_Bezier, RandomGenerator& rccl_Randomizer);

    void mt_Get_Intermed(std::vector<Vector2>& rtst_Intermed) const;
    void mt_Manage_Additional_Points(const MapGeneratorData& rcst_Data, RandomGenerator& rccl_Randomizer);
};

#endif // _MAP_GENERATOR_HPP
