#include "WordGenerator.hpp"
#include <fstream>
#include <sstream>

WordGenerator::WordGenerator()
 :  m_languages_dic()
{}

bool WordGenerator::mt_Save(const std::string& dir_path, const std::string& file_name_without_ext) const
{
    bool l_b_Ret;
    std::ofstream l_cl_input_stream(dir_path + file_name_without_ext + ".wg");
    std::string l_cl_line;

    l_b_Ret = l_cl_input_stream.is_open();

    if (l_b_Ret)
    {
        for (const auto& p : m_languages_dic)
        {
            if (l_b_Ret)
            {
                l_b_Ret = p.second.mt_Save(dir_path + p.first + ".dic");
                l_cl_input_stream << p.first << ' ' << p.first + ".dic\n";
            }
        }
    }

    return l_b_Ret;
}

bool WordGenerator::mt_Load(const std::string& dir_path, const std::string& file_name_without_ext)
{
    bool l_b_Ret;
    std::ifstream l_cl_input_stream(dir_path + file_name_without_ext + ".wg");
    std::string l_cl_line;

    l_b_Ret = l_cl_input_stream.is_open();

    if (l_b_Ret)
    {
        while(l_b_Ret && std::getline(l_cl_input_stream, l_cl_line))
        {
            std::stringstream l_cl_stream(l_cl_line);
            std::string l_cl_lang;
            std::string l_cl_file;

            l_cl_stream >> l_cl_lang >> l_cl_file;

            l_b_Ret = m_languages_dic.emplace(l_cl_line, Dic()).first->second.mt_Load(dir_path + l_cl_file);

        }
    }

    return l_b_Ret;
}

bool WordGenerator::mt_Set_Lang(const std::string& lang, const std::string& input_file, FileEncoding enc)
{
    return m_languages_dic[lang].mt_Analyze(input_file, enc);
}

bool WordGenerator::mt_Set_Lang(const std::string& lang, const std::vector<Dic::String>& data)
{
    return m_languages_dic[lang].mt_Analyze(data);
}

Dic::String WordGenerator::mt_Generate(const std::string& lang)
{
    return m_languages_dic[lang].mt_Generate();
}

Dic::String WordGenerator::mt_Generate(const std::string& lang, std::size_t word_size)
{
    return m_languages_dic[lang].mt_Generate(word_size);
}

Dic::String WordGenerator::mt_Generate(const std::string& lang, std::size_t min_word_size, std::size_t max_word_size)
{
    return m_languages_dic[lang].mt_Generate(min_word_size, max_word_size);
}
