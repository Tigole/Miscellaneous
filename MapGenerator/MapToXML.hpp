#ifndef _MAP_TO_XML_HPP
#define _MAP_TO_XML_HPP 1

#include <string>

class Map;

struct MapToXMLData
{
    std::string m_Resource_Path;
    std::string m_Map_Id;
    std::string m_Tileset_Id;
    std::string m_Music_Id;
};

class MapToXML
{
public:

    void mt_Generate(const Map& rccl_Map, const MapToXMLData& rcst_Data);
};

#endif // _MAP_TO_XML_HPP
