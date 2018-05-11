#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include "Dictionary.hpp"
#include "WordGenerator.hpp"
#include <ctime>
#include <cstdlib>

std::ostream& operator<<(std::ostream& o, const Dic::String& str)
{
    for (auto c : str)
        o << (char)c;
    return o;
}

int main(int argc, char* argv[])
{
    WordGenerator l_generator;
    Dic d;
    Dic::String generated_string;
    std::vector<std::string> l_langs;
    std::ofstream l_output_file("output.txt");

    std::srand(std::time(0));

    l_generator.mt_Set_Lang("en", "data/en.txt", FileEncoding::ANSI);
    l_generator.mt_Set_Lang("es", "data/es.txt", FileEncoding::ANSI);
    l_generator.mt_Set_Lang("fr", "data/fr.txt", FileEncoding::ANSI);
    l_generator.mt_Set_Lang("hu", "data/hu.txt", FileEncoding::ANSI);
    l_generator.mt_Set_Lang("it", "data/it.txt", FileEncoding::ANSI);
    l_generator.mt_Set_Lang("se", "data/se.txt", FileEncoding::ANSI);
    l_generator.mt_Set_Lang("fi", "data/fi.txt", FileEncoding::ANSI);

    l_langs.push_back("en");
    l_langs.push_back("es");
    l_langs.push_back("fr");
    l_langs.push_back("hu");
    l_langs.push_back("it");
    l_langs.push_back("se");
    l_langs.push_back("fi");

    l_generator.mt_Save("Res/", "generator");

    for (const auto& l : l_langs)
    {
        l_output_file << "Lang : " << l;
        for (std::size_t i = 0; i < 15; i++)
            l_output_file << "\n" << l_generator.mt_Generate(l, 4, 8);

        l_output_file << "\n\n";
    }

    return 0;
}

