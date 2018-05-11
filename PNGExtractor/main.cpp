#include <iostream>
#include <fstream>
#include <sstream>

#include <cstdlib>
#include <cstdio>
#include <vector>

using namespace std;

template<typename T>
std::string fn_Number_To_String(T number)
{
    std::stringstream l_ss;

    l_ss << number;

    return l_ss.str();
}

enum class enum_Reader_Mode
{
    Idle,
    Extracting
};

int fn_Extract_PNG(const std::string& file_path, const std::string& output_files_directory)
{
    const std::vector<unsigned char> l_begin = {137, 80, 78, 71, 13, 10, 26, 10};
    const std::vector<unsigned char> l_iend = {73, 69, 78, 68, 174, 66, 96, 130};
    std::size_t l_begin_index(0), l_iend_index(0);
    int l_png_count(0);
    std::string l_str_PNG_file_name(output_files_directory + "/file_" + fn_Number_To_String(l_png_count) + ".png");
    std::string l_PNG_data_stream;
    std::string l_line;
    FILE* l_p_File;
    int l_int(0);
    enum_Reader_Mode l_e_Mode(enum_Reader_Mode::Idle);

    l_p_File = fopen(file_path.c_str(), "rb");
    while((l_p_File != nullptr) && (l_int != EOF))
    {
        l_int = fgetc(l_p_File);
        unsigned char l_uc = (unsigned char)l_int;
        {
            if (l_e_Mode == enum_Reader_Mode::Extracting)
            {
                l_PNG_data_stream += l_uc;
            }
            else
            {
                l_PNG_data_stream.resize(0);
            }
            if (l_uc == l_begin[l_begin_index])
            {
                l_begin_index++;
                if (l_begin_index >= l_begin.size())
                {
                    l_begin_index = 0;
                    l_e_Mode = enum_Reader_Mode::Extracting;
                    for (auto uc : l_begin)
                        l_PNG_data_stream += uc;
                }
            }
            else
            {
                l_begin_index = 0;
            }
            if (l_uc == l_iend[l_iend_index])
            {
                l_iend_index++;
                if (l_iend_index >= l_iend.size())
                {
                    l_e_Mode = enum_Reader_Mode::Idle;
                    std::ofstream l_output_stream(l_str_PNG_file_name, std::ios::binary);
                    l_output_stream << l_PNG_data_stream;
                    l_output_stream.close();
                    l_png_count++;
                    l_str_PNG_file_name = output_files_directory + "/file_" + fn_Number_To_String(l_png_count) + ".png";
                    l_iend_index = 0;
                }
            }
            else
            {
                l_iend_index = 0;
            }
        }
    }

    return l_png_count;
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        cout << "argv[1] = file with PNG to extract\n";
        cout << "argv[2] = directory where to copy PNGs (without '/' at the end).";
    }
    else
    {
        const unsigned char l_iend[] = {73, 69, 78, 68, 0};
        const unsigned char l_begin[] = {137, 80, 78, 71, 13, 10, 26, 10, 0};
        std::string l_str(argv[1]);
        std::string l_str_output_directory(argv[2]);

        cout << "l_begin : " << l_begin << "\n";
        cout << "l_iend : " << l_iend << "\n";

        fn_Extract_PNG(l_str, l_str_output_directory);
    }

    return 0;
}
