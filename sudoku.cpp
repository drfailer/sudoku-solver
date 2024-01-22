#include "sudoku.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <algorithm>

/******************************************************************************/
/*                                load & save                                 */
/******************************************************************************/

/* load a grid from a file */
void loadFromFile(int grid[9][9], const std::string& fileName) {
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "Error: can't open file." << std::endl;
        return;
    }

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            file >> grid[i][j];
        }
    }
}

/* save a grid to a file */
void saveToFile(int grid[9][9], const std::string& fileName) {
    std::ofstream file(fileName);

    for (int i = 0; i < 9; ++i) {
        file << grid[i][0];
        for (int j = 1; j < 9; ++j) {
            file << " " << grid[i][j];
        }
        file << std::endl;
    }
}

/******************************************************************************/
/*                                   print                                    */
/******************************************************************************/

/* print a grid on stdout */
void print(int grid[9][9]) {
    for (int i = 0; i < 9; ++i) {
        for (int j =0; j < 9; ++j) {
            std::cout << grid[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

/******************************************************************************/
/*                                   check                                    */
/******************************************************************************/

/* Check if a grid is valid */
bool check(int grid[9][9]) {
    for (int i = 0; i < 9; ++i) {
        // check lines and columns
        for (int j = 0; j < 9; ++j) {
            for (int k = 0; k < 9; ++k) {
                bool lineError = (k != j && grid[i][j] == grid[i][k]);
                bool columnError = (k != i && grid[i][j] == grid[k][j]);

                if (lineError && columnError) {
                    return false;
                }
            }
        }

        // check subgrids
        std::pair<int, int> sgc = subgridCoord(i);
        bool founded[9] = {0};

        for (int j = sgc.first; j < (sgc.first + 3); ++j) {
            for (int k = sgc.second; k < (sgc.second + 3); ++k) {
                if (founded[grid[j][k] - 1]) {
                    return false;
                } else {
                    founded[grid[j][k] - 1] = true;
                }
            }
        }
    }

    return true;
}

/******************************************************************************/
/*                             get valid numbers                              */
/******************************************************************************/

/* get the list of the valid numbers for a cell in the given grid */
std::vector<int> getValids(int grid[9][9], int line, int column) {
    std::vector<int> valids;
    bool found[9] = {0};
    std::pair<int, int> sgc = subgridCoord(line, column);

    if (grid[line][column] > 0) {
        return valids;
    }

    // lines
    for (int i = 0; i < 9; ++i) {
        if (grid[line][i] > 0) {
            found[grid[line][i] - 1] = true;
        }
        if (grid[i][column] > 0) {
            found[grid[i][column] - 1] = true;
        }
    }

    // subgrids
    for (int i = sgc.first; i < (sgc.first + 3); ++i) {
        for (int j = sgc.second; j < (sgc.second + 3); ++j) {
            if (grid[i][j] > 0) {
                found[grid[i][j] - 1] = true;
            }
        }
    }

    // get the ones that are not found
    for (int i = 0; i < 9; ++i) {
        if (!found[i]) {
            valids.push_back(i + 1);
        }
    }
    return valids;
}

/* get the list of valid numbers for all the cells in the given grid. The
 * returned list is sorted on the number of possibilities (we treat cells with
 * less possibilities first) */
std::vector<ValidList> getValidsLists(int grid[9][9]) {
    std::vector<ValidList> valids;

    // get all valids
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (grid[i][j] == 0) {
                std::vector<int> validsHere = getValids(grid, i, j);

                if (validsHere.size()) {
                    valids.push_back({
                            .line = i,
                            .column = j,
                            .valids = validsHere
                            });
                }
            }
        }
    }

    // sort valids to get the elements with less possiblities
    std::sort(valids.begin(), valids.end(),
            [](const ValidList& lhs, const ValidList rhs) {
                return lhs.valids.size() > rhs.valids.size();
            });

    return valids;
}

/******************************************************************************/
/*                                   solve                                    */
/******************************************************************************/

void solve(int grid[9][9]) {
    solveImpl(grid);
}

void solveImpl(int grid[9][9]) {
    std::vector<ValidList> valids = getValidsLists(grid);

    if (valids.size() == 0) {
        if (check(grid)) {
            std::cout << "Solution:" << std::endl;
            print(grid);
        }
        return;
    }

    ValidList lst = valids.back();
    int newGrid[9][9];
    copyGrids(newGrid, grid);

    for (int validNumber : lst.valids) {
        newGrid[lst.line][lst.column] = validNumber;
        solveImpl(newGrid);
    }
}

/******************************************************************************/
/*                                    copy                                    */
/******************************************************************************/

void copyGrids(int dest[9][9], int src[9][9]) {
    for (int i = 0; i < 9; ++i) {
        memcpy(dest[i], src[i], 9 * sizeof(int));
    }
}
