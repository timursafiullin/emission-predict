#include <vector>
#include <fstream>
#include <regex>
#include "csv.h"

std::vector<std::string> CsvReader::read_row()
{
    if (!file.is_open())
        throw FileIsClosedError();

    std::string line;
    if (!file.eof())
    {
        std::getline(file, line);
        /*
        if (line == "")
        {
            close_file();
            return std::vector<std::string>{""};
        }
        */
    }
    else
    {
        close_file();
        return std::vector<std::string>{""};
    }

    std::vector<std::string> parsed_line;
    std::regex del(delimiter);
    std::sregex_token_iterator token_iterator(line.begin(), line.end(), del, -1);
    std::sregex_token_iterator end;

    for (; token_iterator != end; ++token_iterator)
        parsed_line.push_back(*token_iterator);

    return parsed_line;
}

std::vector<std::vector<std::string>> CsvReader::read_until_blank_line()
{
    std::vector<std::vector<std::string>> lines;
    std::vector<std::string> line;
    while (line != std::vector<std::string>{"\n"} && line != std::vector<std::string>{""})
    {
        lines.push_back(line);
        line = read_row();
    }

    return lines;
}

std::vector<std::vector<std::string>> CsvReader::read_file()
{
    std::vector<std::vector<std::string>> lines;
    std::vector<std::string> line;
    while (line != std::vector<std::string>{""})
    {
        lines.push_back(line);
        line = read_row();
    }

    return lines;
}

void CsvWriter::write_row(std::vector<std::string> &row)
{
    if (!file.is_open())
        open_file();
    for (size_t i = 0; i < row.size(); ++i)
    {
        file << row[i] << ((i != row.size() - 1) ? delimiter : "");
    }
    file << "\n";
}