#include "sudoku.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>

/******************************************************************************/
/*                                load & save                                 */
/******************************************************************************/

/**
 * @brief  Load a grid from a file
 *
 * @param  grid      2D array representing the grid.
 * @param  fileName  Name of the file to load.
 */
void loadFromFile(int grid[9][9], const std::string &fileName) {
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

/**
 * @brief  Save a grid to a file
 *
 * @param  grid      2D array representing the grid.
 * @param  fileName  Name of the file.
 */
void saveToFile(int grid[9][9], const std::string &fileName) {
    std::ofstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "Error: can't open file." << std::endl;
        return;
    }

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

/**
 * @brief  Display a grid on stdout.
 * @param  grid  Sudoku grid to display.
 */
void print(int grid[9][9]) {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            std::cout << grid[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

/******************************************************************************/
/*                                   check                                    */
/******************************************************************************/

/**
 * @brief  Check if a grid is valid.
 * @param  grid  Sudoku grid to check.
 * @return  true if the grid is correct, false otherwise.
 */
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

/**
 * @brief  Compute the list of the valid numbers for a cell.
 *
 * @param  grid  Sudoku grid.
 * @param  line  Line of the cell.
 * @param  column  Column of the cell.
 * @return  Vector containing the numbers.
 */
std::vector<int> getValids(int grid[9][9], int line, int column) {
    std::vector<int> valids;
    bool found[9] = {0};
    std::pair<int, int> sgc = subgridCoord(line, column);

    if (grid[line][column] > 0) {
        return valids;
    }

    // lines & columns
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

/**
 * @brief  Computes the list of valid numbers for all the cells in the given
 *         grid.
 *
 * @param  grid  Sudoku grid.
 * @return  Vector containing all the valid lists with the following format:
 *          { line: int, column: int, valids: vector<int> }
 */
std::set<ValidList, CompSize> getValidsLists(int grid[9][9]) {
    std::set<ValidList, CompSize> valids;

    // get all valids
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (grid[i][j] == 0) {
                std::vector<int> validsHere = getValids(grid, i, j);

                if (validsHere.size()) {
                    valids.insert(
                        {.line = i, .column = j, .valids = validsHere});
                }
            }
        }
    }

    return valids;
}

/**
 * @brief  Update the list of valid numbers. This function has to be used when a
 *         new number is placed on the grid.
 *
 * @param  valids  Old list of valid numbers
 * @param  line  Line where the new number has be placed.
 * @param  column  Column where the new number has be placed.
 * @param  number  New number placed.
 *
 * @return  The updated list of valid numbers where `number` has been removed in
 *          all the valid list on the same line / column / subgrid as the cell
 *          (line, column).
 */
std::set<ValidList, CompSize>
updateValids(const std::set<ValidList, CompSize> &valids, int line, int column,
             int number) {
    std::set<ValidList, CompSize> output;
    std::pair<int, int> sgc = subgridCoord(line, column);

    for (const ValidList &lst : valids) {
        bool sameLine = lst.line == line;
        bool sameColumn = lst.column == column;
        bool sameSubGrid =
            (sgc.first <= lst.line && lst.line < (sgc.first + 3)) &&
            (sgc.second <= lst.column && lst.column < (sgc.second + 3));
        ValidList updated = {lst.line, lst.column, std::vector<int>()};

        if (sameLine || sameColumn || sameSubGrid) {
            std::copy_if(lst.valids.begin(), lst.valids.end(),
                         std::back_inserter(updated.valids),
                         DifferentThan(number));
        } else {
            std::copy(lst.valids.begin(), lst.valids.end(),
                      std::back_inserter(updated.valids));
        }
        if (updated.valids.size()) {
            output.insert(updated);
        }
    }
    return output;
}

/******************************************************************************/
/*                                   solve                                    */
/******************************************************************************/

/**
 * @brief  Solve the sudoku.
 * @param  grid  Sudoku grid to solve.
 */
void solve(int grid[9][9]) {
    std::set<ValidList, CompSize> valids = getValidsLists(grid);
    solveImpl(grid, std::move(valids));
}

/**
 * @brief  Implementation of the solving algorithm.
 *
 * @param  grid    Sudoku grid to solve.
 * @param  valids  List of valid numbers.
 * @return  The solutions are printed when found. We do not return anything when
 *          a solution is found as we want to find all the possible solutions.
 */
void solveImpl(int grid[9][9], std::set<ValidList, CompSize> &&valids) {
    if (valids.size() == 0) {
        if (check(grid)) {
            std::cout << "Solution:" << std::endl;
            print(grid);
        }
        return;
    }

    ValidList lst = *valids.begin();
    valids.erase(valids.begin());

    for (int validNumber : lst.valids) {
        grid[lst.line][lst.column] = validNumber;
        solveImpl(grid, updateValids(valids, lst.line, lst.column, validNumber));
    }
    grid[lst.line][lst.column] = 0;
}
