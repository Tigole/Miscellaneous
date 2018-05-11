#include <iostream>
#include <fstream>
#include <algorithm>

#include "Map.hpp"
#include "MapGenerator.hpp"

#include "RandomGenerator.hpp"

#include "Linker.hpp"

#include "MapToXML.hpp"


using namespace std;

void fn_Get_Points_Occupation(const Map& rccl_Map,
                              const Vector2& rcst_Point,
                              int radius,
                              std::vector<Vector2>& rtst_Non_Path_Points,
                              std::vector<Vector2>& rtst_Path_Points)
{
    for (int i = 1; i < radius; i++)
    {
        int l_angle_deg_step(45);
        for (int angle_deg = 0; angle_deg < 360; angle_deg += l_angle_deg_step)
        {
            Vector2 l_st_Point(rcst_Point + PolarCoord(i, angle_deg));

            /*if (    (rccl_Map[l_st_Point].m_state == 0)
                &&  (std::find(rtst_Free_Points.begin(), rtst_Free_Points.end(), l_st_Point) == rtst_Free_Points.end()))
            {
                rtst_Free_Points.push_back(l_st_Point);
            }*/
        }
    }
}

void brol()
{
    MapGenerator l_cl_Generator;
    MapGeneratorData l_st_Data;
    RandomGenerator l_cl_Randomizer(0);
    Vector2 l_st_Map_Size(38, 24);
    std::vector<Vector2> l_tst_Points;

    l_tst_Points.push_back(Vector2(2, 24));
    l_tst_Points.push_back(Vector2(35, 0));
}

int main()
{
    MapGenerator l_cl_Generator;
    MapGeneratorData l_st_Data;
    RandomGenerator l_cl_Randomizer(0);
    MapToXML l_cl_To_XML;
    MapToXMLData l_cl_To_XML_Data;

    l_st_Data.m_st_Intermed.m_delta_deg = 90;
    l_st_Data.m_st_Intermed.m_modulus_max = 10;
    l_st_Data.m_st_Intermed.m_modulus_min = 5;
    l_st_Data.m_st_Bezier.m_delta_deg = 90;
    l_st_Data.m_st_Sign_Points.m_tst_Points.push_back(Vector2(2, 1));
    /*l_st_Data.m_st_Sign_Points.m_count = 1;
    l_st_Data.m_st_Sign_Points.m_modulus_min = 5;
    l_st_Data.m_st_Sign_Points.m_modulus_max = 10;*/

    for (int i = 0; i < 57; i++)
    {
        l_cl_Randomizer.mt_Generate();
    }

    for (int i = 0; i < 1; i++)
	{
		l_cl_Generator.mt_Generate(26, 14, Vector2(0, 13), Vector2(25, 0), l_st_Data, l_cl_Randomizer);
		l_cl_Generator.mt_Print(cout);
		cout << std::dec << l_cl_Randomizer.mt_Get_Generation_Count() << '\n';
	}

	l_cl_To_XML_Data.m_Map_Id = "GeneratedMap";
	l_cl_To_XML_Data.m_Tileset_Id = "GeneratedTileset";
	l_cl_To_XML_Data.m_Music_Id = "Lost In Fairyland";
	l_cl_To_XML_Data.m_Resource_Path = "C:/Users/Janniere Sylvain/Dropbox/01-Resources_rpg_engine/Resources/Data/Maps";

	l_cl_To_XML.mt_Generate(l_cl_Generator, l_cl_To_XML_Data);

    return 0;
}
