#ifndef SUDOKU_HPP
#define SUDOKU_HPP
#include <string>
#include <utility>
#include <vector>


/* accessors **************************************************************/

inline std::pair<int, int> subgridCoord(int subgridIdx)  {
    return std::make_pair(3 * (subgridIdx / 3), 3 * (subgridIdx % 3));
}

inline std::pair<int, int> subgridCoord(int i, int j)  {
    return std::make_pair(3 * (i / 3), 3 * (j / 3));
}

/* print ******************************************************************/
void print(int grid[9][9]);

/* load & save ************************************************************/
void loadFromFile(int grid[9][9], const std::string& fileName);
void saveToFile(int grid[9][9], const std::string& fileName);

/* check ******************************************************************/
bool check(int grid[9][9]);

/* resolve ****************************************************************/
void copyGrids(int dest[9][9], int src[9][9]);
std::vector<int> getValids(int grid[9][9], int line, int culumn);
void resolve(int grid[9][9]);

struct ValidList {
    int line;
    int column;
    std::vector<int> valids;
};
std::vector<ValidList> getValidsLists(int grid[9][9]);
void resolveImpl(int grid[9][9]);

#endif
