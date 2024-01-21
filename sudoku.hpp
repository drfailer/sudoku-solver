#ifndef SUDOKU_HPP
#define SUDOKU_HPP
#include <string>
#include <vector>

/******************************************************************************/
/*                                  structs                                   */
/******************************************************************************/

struct ValidList {
    int line;
    int column;
    std::vector<int> valids;
};

/******************************************************************************/
/*                                 functions                                  */
/******************************************************************************/

void loadFromFile(int grid[9][9], const std::string& fileName);
void saveToFile(int grid[9][9], const std::string& fileName);
void print(int grid[9][9]);
bool check(int grid[9][9]);
void resolve(int grid[9][9]);

/******************************************************************************/
/*                              helper functions                              */
/******************************************************************************/

std::vector<int> getValids(int grid[9][9], int line, int culumn);
std::vector<ValidList> getValidsLists(int grid[9][9]);
void resolveImpl(int grid[9][9]);

inline std::pair<int, int> subgridCoord(int subgridIdx)  {
    return std::make_pair(3 * (subgridIdx / 3), 3 * (subgridIdx % 3));
}

inline std::pair<int, int> subgridCoord(int i, int j)  {
    return std::make_pair(3 * (i / 3), 3 * (j / 3));
}

void copyGrids(int dest[9][9], int src[9][9]);

#endif
