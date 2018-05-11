#ifndef _WORD_GENERATOR_HPP
#define _WORD_GENERATOR_HPP 1

#include "Dictionary.hpp"

class WordGenerator
{
public:
    WordGenerator();

    bool mt_Save(const std::string& dir_path, const std::string& file_name_without_ext) const;
    bool mt_Load(const std::string& dir_path, const std::string& file_name_without_ext);

    bool mt_Set_Lang(const std::string& lang, const std::string& input_file, FileEncoding enc);
    bool mt_Set_Lang(const std::string& lang, const std::vector<Dic::String>& data);

    Dic::String mt_Generate(const std::string& lang);
    Dic::String mt_Generate(const std::string& lang, std::size_t word_size);
    Dic::String mt_Generate(const std::string& lang, std::size_t min_word_size, std::size_t max_word_size);

private:
    std::map<std::string, Dic> m_languages_dic;
};

#endif // _WORD_GENERATOR_HPP
