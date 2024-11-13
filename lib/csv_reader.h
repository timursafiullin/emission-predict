#ifndef CSV_READER_H
#define CSV_READER_H 1

#include <stdexcept>
#include <vector>
#include <fstream>

class FileNotExistsError : public std::exception
{
};

class FileIsClosedError : public std::exception
{
};

class CsvReader
{
public:
    CsvReader(std::string filename) : filename(filename) {}
    CsvReader(std::string filename, std::string delimiter) : filename(filename), delimiter(delimiter) {}

    void open_file()
    {
        file.open(filename);
        if (!file.is_open())
            throw FileNotExistsError();
    }

    void close_file()
    {
        file.close();
    }

    std::vector<std::string> read_row();
    std::vector<std::vector<std::string>> read_file();

private:
    std::string delimiter = ";";
    std::string filename;
    std::ifstream file;
};

#endif