#include <iostream>
#include "gui.h"
#include "graph_lib.h"
#include "dataset_reader.h"

int main()
{
    std::vector<DatasetCell> cells = get_all_cells();
    
    for (size_t i = 0; i < cells.size(); ++i) {
        std::cout << cells[i].acceleration << std::endl;
    }

    //window();
    return 0;
}