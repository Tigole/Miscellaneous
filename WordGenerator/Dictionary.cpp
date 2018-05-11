#include "Dictionary.hpp"
#include <fstream>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <functional>
#include <set>


Dic::Dic()
 :  m_character_markov_table(),
    m_word_length_markov_table()
{
    //
}

bool Dic::mt_Save(const std::string& output_file_path) const
{
    std::ofstream l_output_stream;

    l_output_stream.open(output_file_path);

    l_output_stream << DIC_VERSION << '\n';

    l_output_stream << m_word_length_markov_table.size() << ' ';
    for (const auto& l : m_word_length_markov_table)
        l_output_stream << l.first << ' ' << l.second << ' ';

    for (auto& a : m_character_markov_table)
    {
        l_output_stream << '\n' << a.first << ' ' << a.second.size();
        for (auto& c : a.second)
        {
            l_output_stream << ' ' << c.first << ' ' << c.second;
        }
    }

    return true;
}

bool Dic::mt_Load(const std::string& input_file_path)
{
    bool l_b_ret;
    std::ifstream l_cl_input_stream(input_file_path);
    std::string l_line;
    Char l_input_char;
    Char l_current_char;
    int l_current_char_count;
    int l_data_count;

    std::getline(l_cl_input_stream, l_line);

    if (l_line == DIC_VERSION)
    {
        l_b_ret = true;

        m_word_length_markov_table.clear();
        m_character_markov_table.clear();

        std::getline(l_cl_input_stream, l_line);
        {
            std::stringstream l_cl_line_stream;
            l_cl_line_stream.str(l_line);
            l_cl_line_stream >> l_data_count;
            for (int i = 0; i < l_data_count; i++)
            {
                int a, b;

                l_cl_line_stream >> a >> b;

                m_word_length_markov_table[a] = b;
            }
        }

        while (std::getline(l_cl_input_stream, l_line))
        {
            std::stringstream l_cl_line_stream(l_line);

            l_cl_line_stream >> l_input_char >> l_data_count;
            for (int i = 0; i < l_data_count; i++)
            {
                l_cl_line_stream >> l_current_char >> l_current_char_count;
                m_character_markov_table[l_input_char][l_current_char] = l_current_char_count;
            }
        }
    }
    else
    {
        l_b_ret = false;
    }


    return l_b_ret;
}

bool Dic::mt_Analyze(const std::string& input_file_path, FileEncoding encoding)
{
    bool l_b_ret(false);
    std::map<FileEncoding, std::function<bool(const std::string&)>> l_read_file;
    std::map<FileEncoding, std::function<bool(const std::string&)>>::iterator l_it;
    std::ifstream l_file(input_file_path);

    l_read_file.emplace(FileEncoding::ANSI, std::bind(&Dic::mt_Analyze_ANSI, this, std::placeholders::_1));

    l_it = l_read_file.find(encoding);

    if (    (l_it != l_read_file.end())
         && (l_file.is_open() == true))
    {
        l_file.close();
        l_b_ret = l_it->second(input_file_path);
    }

    return l_b_ret;
}

bool Dic::mt_Analyze(const std::vector<String>& data)
{
    bool l_b_ret(false);
    int l_size;

    m_word_length_markov_table.clear();
    m_character_markov_table.clear();


    for(const auto& s : data)
    {
        l_size = s.size();
        m_word_length_markov_table[s.size()]++;
        for (std::size_t i = 0; i < (s.size() - 1); i++)
        {
            m_character_markov_table[s[i]][s[i+1]]++;
        }
        l_b_ret = true;
    }

    mt_Normalize();

    return l_b_ret;
}

Dic::String Dic::mt_Generate(std::size_t word_size)
{
    String l_ret;
    Char l_next_char;
    Char l_current_char;

    l_ret.resize(word_size);
    word_size--;

    do
    {
        l_next_char = mt_PickFirstChar();
    }
    while (m_character_markov_table[l_next_char].size() == 0);

    l_ret[0] = l_next_char;
    for (std::size_t i = 0; i < word_size; i++)
    {
        l_current_char = l_next_char;
        do
        {
            l_next_char = mt_PickChar(l_current_char);
        }
        while (m_character_markov_table[l_next_char].size() == 0);
        l_ret[i+1] = l_next_char;
    }

    return l_ret;
}

Dic::String Dic::mt_Generate(std::size_t word_min_size, std::size_t word_max_size)
{
    return mt_Generate((std::rand() % (word_max_size - word_min_size)) + word_min_size);
}

Dic::String Dic::mt_Generate(void)
{
    std::vector<int> l_size;
    int l_index;

    for (auto s : m_word_length_markov_table)
    {
        for (int i = 0; i < s.second; i++)
            l_size.push_back(s.first);
    }

    l_index = std::rand();
    l_index %= l_size.size();

    return mt_Generate(l_size[l_index]);
}

Dic::Char Dic::mt_PickChar(Dic::Char c)
{
    std::vector<Dic::Char> l_vec;
    Dic::Char l_ret;

    for (auto& a : m_character_markov_table[c])
    {
        for (int i = 0; i < a.second; i++)
            l_vec.push_back(a.first);
    }

    l_ret = l_vec[std::rand() % l_vec.size()];

    return l_ret;
}

Dic::Char Dic::mt_PickFirstChar(void)
{
    std::vector<Dic::Char> l_vec;
    Dic::Char l_ret;

    for (auto& a : m_character_markov_table)
        l_vec.push_back(a.first);

    l_ret = l_vec[std::rand() % l_vec.size()];

    return l_ret;
}

bool Dic::mt_Analyze_ANSI(const std::string& input_file_path)
{
    bool l_b_ret(false);
    std::vector<String> l_data;
    std::ifstream l_cl_input_stream(input_file_path);
    std::string l_line, l_line_bis;
    char l_char;
    String l_string;
    std::set<char> l_non_char;

    while(std::getline(l_cl_input_stream, l_line))
    {
        l_line_bis.resize(0);
        for (std::size_t i = 0; (i < l_line.size()) && (std::isalpha(l_line[i]) || (l_line[i] & 0x80)); i++)
        {
            l_line_bis += l_line[i];
        }
        if (l_line_bis.size() != 0)
        {
            for (std::size_t i = 0; i < l_line_bis.size(); i++)
            {
                l_string.push_back(l_line_bis[i]);
            }
            l_data.push_back(l_string);
            l_string.resize(0);
        }
    }

    /*l_non_char.emplace(' ');
    l_non_char.emplace(',');
    l_non_char.emplace('.');
    l_non_char.emplace('<');
    l_non_char.emplace('>');
    l_non_char.emplace('=');
    l_non_char.emplace('_');
    l_non_char.emplace('\r');
    l_non_char.emplace('\n');
    l_non_char.emplace('\t');
    l_non_char.emplace('0');
    l_non_char.emplace('1');
    l_non_char.emplace('2');
    l_non_char.emplace('3');
    l_non_char.emplace('4');
    l_non_char.emplace('5');
    l_non_char.emplace('6');
    l_non_char.emplace('7');
    l_non_char.emplace('8');
    l_non_char.emplace('9');
    l_non_char.emplace('@');
    l_non_char.emplace('#');
    l_non_char.emplace('\"');
    l_non_char.emplace('\'');
    l_non_char.emplace('~');
    l_non_char.emplace('&');
    l_non_char.emplace('|');
    l_non_char.emplace('(');
    l_non_char.emplace(')');
    l_non_char.emplace('[');
    l_non_char.emplace(']');
    l_non_char.emplace('{');
    l_non_char.emplace('}');
    l_non_char.emplace(':');
    l_non_char.emplace('/');
    l_non_char.emplace('\\');*/

    /*while(l_cl_input_stream.eof() == false)
    {
        l_cl_input_stream.read(&l_char, 1);

        if (l_non_char.find(l_char) != l_non_char.end())
        {
            if (l_string.size() != 0)
            {
                l_data.push_back(l_string);
                l_string.resize(0);
            }
        }
        else
        {
            l_string.push_back((Char)l_char);
        }
    }*/

    l_b_ret = mt_Analyze(l_data);

    return l_b_ret;
}

bool Dic::mt_Analyze_UTF8(const std::string& input_file_path)
{
    bool l_b_ret(false);

    return l_b_ret;
}

void Dic::mt_Normalize(void)
{
    int l_size;

    for (auto& l_current_char : m_character_markov_table)
    {
        l_size = 0;
        for (auto l_it = l_current_char.second.begin(); l_it != l_current_char.second.end(); l_it++)
        {
            l_size += l_it->second;
        }
        if (l_size > 1000)
        {
            for (auto l_it = l_current_char.second.begin(); l_it != l_current_char.second.end(); l_it++)
            {
                l_it->second = (l_it->second * 1000) / l_size;
            }
            for (auto l_it = l_current_char.second.begin(); l_it != l_current_char.second.end(); l_it++)
            {
                if (l_it->second == 0)
                {
                    l_current_char.second.erase(l_it);
                    l_it = l_current_char.second.begin();
                }
            }
        }
    }

    l_size = 0;
    for (auto& l_current_size : m_word_length_markov_table)
    {
        l_size += l_current_size.second;
    }
    for (auto& l_current_size : m_word_length_markov_table)
    {
        l_current_size.second = (l_current_size.second * 1000) / l_size;
    }
    for (auto l_it = m_word_length_markov_table.begin(); l_it != m_word_length_markov_table.end(); l_it++)
    {
        if (l_it->second == 0)
        {
            m_word_length_markov_table.erase(l_it);
            l_it = m_word_length_markov_table.begin();
        }
    }
}
