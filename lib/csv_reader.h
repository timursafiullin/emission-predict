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

class FileIsNotSpecifiedError : public std::exception
{
};

class CsvReader
{
public:
    CsvReader() = default;
    CsvReader(std::string filename) : filename(filename) {}
    CsvReader(std::string filename, std::string delimiter) : delimiter(delimiter), filename(filename) {}

    bool is_open()
    {
        return file.is_open();
    }

    void open_file(std::string fn = "")
    {
        if (fn != "")
        {
            filename = fn;
        }
        if (filename == "" and filename == "")
        {
            throw FileIsNotSpecifiedError();
        }
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
    std::string delimiter = ",";
    std::ifstream file;
    std::string filename = "";
};

#endif