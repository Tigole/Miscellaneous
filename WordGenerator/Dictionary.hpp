#ifndef _DICTIONARY_HPP
#define _DICTIONARY_HPP 1

#include <string>
#include <map>
#include <vector>

#define DIC_VERSION std::string("0.0.1")


enum class FileEncoding
{
    ANSI,
    UTF8,
};

class Dic
{
public:

    using Char = uint32_t;
    using String = std::vector<Char>;


    Dic();

    bool mt_Save(const std::string& output_file_path) const;
    bool mt_Load(const std::string& input_file_path);

    bool mt_Analyze(const std::string& input_file_path, FileEncoding encoding);
    bool mt_Analyze(const std::vector<String>& data);

    String mt_Generate(std::size_t word_size);
    String mt_Generate(std::size_t word_min_size, std::size_t word_max_size);
    String mt_Generate(void);


private:
    std::map<Char, std::map<Char, int>> m_character_markov_table;
    std::map<int, int> m_word_length_markov_table;

    Char mt_PickChar(Char c);
    Char mt_PickFirstChar(void);

    bool mt_Analyze_ANSI(const std::string& input_file_path);
    bool mt_Analyze_UTF8(const std::string& input_file_path);

    void mt_Normalize(void);

};

#endif // _DICTIONARY_HPP
