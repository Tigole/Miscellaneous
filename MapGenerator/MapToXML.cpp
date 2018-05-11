#include "MapToXML.hpp"
#include "Map.hpp"

#include <fstream>

void MapToXML::mt_Generate(const Map& rccl_Map, const MapToXMLData& rcst_Data)
{
    Vector2 l_st_It(0, 0);
    Vector2 l_st_Size(rccl_Map.mt_Get_Size());
    std::ofstream l_cl_Output_Stream(rcst_Data.m_Resource_Path + '/' + rcst_Data.m_Map_Id + ".xml");

    l_cl_Output_Stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    l_cl_Output_Stream << "<BasicMap id=\"" << rcst_Data.m_Map_Id << "\" tileset_id=\"" << rcst_Data.m_Tileset_Id << "\">\n\t<Layer number=\"1\">\n";

    for (l_st_It.m_y = 0; l_st_It.m_y < l_st_Size.m_y; l_st_It.m_y++)
    {
        for (l_st_It.m_x = 0; l_st_It.m_x < l_st_Size.m_x; l_st_It.m_x++)
        {
            int l_tile_id;
            Vector2 l_st_Tileset_Coord;

            if (rccl_Map[l_st_It].m_state == 0)
            {
                l_tile_id = 1;
            }
            else
            {
                l_tile_id = 0;
            }

            l_cl_Output_Stream << "\t\t<Cell cell_col=\"" << l_st_It.m_x << "\" cell_row=\"" << l_st_It.m_y
                                << "\" tile_id=\"" << l_tile_id << "\"/>\n";
        }
    }

	l_cl_Output_Stream << "\t</Layer>\n\t<LinkedMaps>\n\t\t<LinkedMap cell_col=\"\" cell_row=\"\" map_id=\"\"/>\n\t</LinkedMaps>\n";
	l_cl_Output_Stream << "\t<AmbiantMusic id=\"" << rcst_Data.m_Music_Id << "\"/>\n";
	l_cl_Output_Stream << "\t<CharacterLayer number=\"1\"/>\n</BasicMap>";
}

