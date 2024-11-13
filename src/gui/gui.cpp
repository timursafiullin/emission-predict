#include <iostream>
#include "gui.h"
#include "graph_lib.h"
#include "csv_reader.h"

int main()
{
    std::string filename = "test.csv";
    CsvReader csv_reader(filename, ",");
    csv_reader.open_file();
    std::vector<std::vector<std::string>> file = csv_reader.read_file();
    for (size_t i = 0; i < file.size(); ++i)
    {
        for (size_t j = 0; j < file[i].size(); ++j)
        {
            std::cout << file[i][j] << " ";
        }
        std::cout << std::endl;
    }

    create_window(window_width, window_height);
    return 0;
}