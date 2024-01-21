#ifndef SUDOKU_HPP
#define SUDOKU_HPP
#include <string>
#include <utility>
#include <vector>

class Sudoku {
public:
    Sudoku() = default;

    /* accessors **************************************************************/
    int& at(int i, int j) { return table[i][j]; }

    // subgrids coordinates
    std::pair<int, int> subgridCoord(int subgridIdx) const {
        return std::make_pair(3 * (subgridIdx / 3), 3 * (subgridIdx % 3));
    }
    std::pair<int, int> subgridCoord(int i, int j) const {
        return std::make_pair(3 * (i / 3), 3 * (j / 3));
    }

    /* print ******************************************************************/
    void print() const;

    /* load & save ************************************************************/
    void loadFromFile(const std::string& fileName);
    void saveToFile(const std::string& fileName) const;

    /* check ******************************************************************/
    bool check() const;

    /* resolve ****************************************************************/
    void copyGrids(int dest[9][9], int src[9][9]);
    std::vector<int> getValids(int grid[9][9], int line, int culumn) const;
    void resolve();

    struct ValidList {
        int line;
        int column;
        std::vector<int> valids;
    };
    std::vector<ValidList> getValidsLists(int grid[9][9]);
    void resolveImpl(int grid[9][9]);

private:
    int table[9][9];
};

#endif
