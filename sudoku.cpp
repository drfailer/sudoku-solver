#include "sudoku.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <algorithm>

/******************************************************************************/
/*                                load & save                                 */
/******************************************************************************/

void Sudoku::loadFromFile(const std::string& fileName) {
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "Error: can't open file." << std::endl;
        return;
    }

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            file >> table[i][j];
        }
    }
}

void Sudoku::saveToFile(const std::string& fileName) const {
    std::ofstream file(fileName);

    for (int i = 0; i < 9; ++i) {
        file << table[i][0];
        for (int j = 1; j < 9; ++j) {
            file << " " << table[i][j];
        }
        file << std::endl;
    }
}

/******************************************************************************/
/*                                   print                                    */
/******************************************************************************/

void Sudoku::print() const {
    for (int i = 0; i < 9; ++i) {
        for (int j =0; j < 9; ++j) {
            std::cout << table[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

/******************************************************************************/
/*                                   check                                    */
/******************************************************************************/

bool Sudoku::check() const {
    for (int i = 0; i < 9; ++i) {
        // check lines and columns
        for (int j = 0; j < 9; ++j) {
            for (int k = 0; k < 9; ++k) {
                bool lineError = (k != j && table[i][j] == table[i][k]);
                bool columnError = (k != i && table[i][j] == table[k][j]);

                if (lineError && columnError) {
                    if (lineError) {
                        std::cout << "lineError: " << i << ", " << j << ", " << k << std::endl;
                    }
                    if (columnError) {
                        std::cout << "columnError: " << i << ", " << j << ", " << k << std::endl;
                    }
                    return false;
                }
            }
        }

        // check subgrids
        std::pair<int, int> sgc = subgridCoord(i);
        bool founded[9] = {0};

        for (int j = sgc.first; j < (sgc.first + 3); ++j) {
            for (int k = sgc.second; k < (sgc.second + 3); ++k) {
                if (founded[table[j][k] - 1]) {
                    return false;
                } else {
                    founded[table[j][k] - 1] = true;
                }
            }
        }
    }

    return true;
}

/******************************************************************************/
/*                                   solve                                    */
/******************************************************************************/

std::vector<int> Sudoku::getValids(int grid[9][9], int line, int column) const {
    std::vector<int> valids;
    bool found[9] = {0};
    std::pair<int, int> sgc = subgridCoord(line, column);

    if (grid[line][column] > 0) {
        return valids;
    }

    // lines
    for (int i = 0; i < 9; ++i) {
        if (grid[line][i] > 0) {
            /* std::cout << "found: " << grid[line][i] << std::endl; */
            found[grid[line][i] - 1] = true;
        }
        if (grid[i][column] > 0) {
            /* std::cout << "found: " << grid[i][column] << std::endl; */
            found[grid[i][column] - 1] = true;
        }
    }

    // subgrids
    for (int i = sgc.first; i < (sgc.first + 3); ++i) {
        for (int j = sgc.second; j < (sgc.second + 3); ++j) {
            if (grid[i][j] > 0) {
                /* std::cout << "found: " << grid[i][j] << std::endl; */
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

std::vector<Sudoku::ValidList> Sudoku::getValidsLists(int grid[9][9]) {
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

void printValidList(const std::vector<Sudoku::ValidList>& valids) {
    for (auto elt : valids) {
        std::cout << "(" << elt.line << ", " << elt.column << ") ";

        for (auto v : elt.valids) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
}

void Sudoku::resolve() {
    std::vector<ValidList> valids = getValidsLists(table);
    // il faut copier la table
    int grid[9][9];
    copyGrids(grid, table);
    resolveImpl(grid);
}

void Sudoku::resolveImpl(int grid[9][9]) {
    std::vector<ValidList> valids = getValidsLists(grid);

    if (valids.size() == 0) {
        copyGrids(table, grid);
        if (check()) {
            std::cout << "Solution:" << std::endl;
            print();
        }
        return;
    }

    ValidList lst = valids.back();
    int newGrid[9][9];
    copyGrids(newGrid, grid);

    for (int validNumber : lst.valids) {
        newGrid[lst.line][lst.column] = validNumber;
        resolveImpl(newGrid);
    }
}

void Sudoku::copyGrids(int dest[9][9], int src[9][9]) {
    for (int i = 0; i < 9; ++i) {
        memcpy(dest[i], src[i], 9 * sizeof(int));
    }
}
