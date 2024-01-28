#ifndef SUDOKU_HPP
#define SUDOKU_HPP
#include <string>
#include <vector>
#include <set>
#include <algorithm>

/******************************************************************************/
/*                                  structs                                   */
/******************************************************************************/

struct ValidList {
    int line;
    int column;
    std::vector<int> valids;
};

/******************************************************************************/
/*                                  functors                                  */
/******************************************************************************/

struct CompSize {
    bool operator()(const ValidList& lhs, const ValidList& rhs) const {
        return lhs.valids.size() <= rhs.valids.size();
    }
};

struct DifferentThan {
    DifferentThan(int elt): elt(elt) {}
    bool operator()(int n) const {
        return elt != n;
    }
    int elt;
};

/******************************************************************************/
/*                                 functions                                  */
/******************************************************************************/

void loadFromFile(int grid[9][9], const std::string& fileName);
void saveToFile(int grid[9][9], const std::string& fileName);
void print(int grid[9][9]);
bool check(int grid[9][9]);
void solve(int grid[9][9]);

/******************************************************************************/
/*                              helper functions                              */
/******************************************************************************/

std::vector<int> getValids(int grid[9][9], int line, int culumn);
std::set<ValidList, CompSize> getValidsLists(int grid[9][9]);
void solveImpl(int grid[9][9], std::set<ValidList, CompSize>&& valids);
void copyGrids(int dest[9][9], int src[9][9]);

inline std::pair<int, int> subgridCoord(int subgridIdx)  {
    return std::make_pair(3 * (subgridIdx / 3), 3 * (subgridIdx % 3));
}

inline std::pair<int, int> subgridCoord(int i, int j)  {
    return std::make_pair(3 * (i / 3), 3 * (j / 3));
}

#endif
