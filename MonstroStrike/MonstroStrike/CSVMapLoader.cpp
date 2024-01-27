#include "CSVMapLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

//Author: Johny Yong
//Email: j.yong\@digipen.edu

/*
\brief The function that reads the map and return if the map is successfully loaded into a vector

    Done by opening the file, check if the file is opened, then enter a for loop of row such that i am able to getline per row,
    after getting line is a success, create a stringstream to contain the line and begin to insert the value of each column. The values
    in accordance to row and column is then stored into a vector map that has been passed on as parameter.
 
\param[in] csvFilePath
Name of the file

\param[in, out] map
Reference of the map called in Main.cpp

\param[in] rows
Number of rows

\param[in] cols
Number of cols

\return Result of successful map load
*/
bool MapLoader(const char *csvFilePath, std::vector<std::vector<MapCell>>& map, int rows, int cols) {
    
    std::cout << csvFilePath << std::endl;
    std::ifstream file(csvFilePath, std::ios::in);

    std::cout << "In Loader: " << std::endl;

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << csvFilePath << std::endl;
        return false;
    }

    for (int row = 0; row < rows; ++row) {
        std::string line;
        if (!std::getline(file, line)) {
            std::cerr << "Error reading row " << row + 1 << " from file: " << csvFilePath << std::endl;
            return false;
        }

        std::istringstream lineStream(line);

        int col = 0;
        while (col < cols)
        {
            char cellValue;

            while (lineStream >> cellValue)
            {
                if (cellValue != ',')
                {
                    map[row][col].symbol = cellValue;
                    col++;
                }
            }
        }
    }
    file.close();
    return true;
}

/*
\brief A checker to print the symbol of the map in the file
*/

void PrintMap(const std::vector<std::vector<MapCell>>& map, int rows, int cols)
{
    std::cout << "In Printer: " << std::endl;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            std::cout << map[i][j].symbol << ' ';
        }
        std::cout << std::endl;
    }
}