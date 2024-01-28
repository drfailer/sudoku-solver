#include "sudoku.hpp"

int main(int, char**) {
    int sudoku[9][9];

    loadFromFile(sudoku, "./tests/test1.txt");
    solve(sudoku);
    loadFromFile(sudoku, "./tests/test2.txt");
    solve(sudoku);
    loadFromFile(sudoku, "./tests/test3.txt");
    solve(sudoku);
    /* loadFromFile(sudoku, "./tests/all.txt"); */
    /* solve(sudoku); */
    return 0;
}
