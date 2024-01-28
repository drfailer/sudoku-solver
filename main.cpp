#include "sudoku.hpp"
#include <iostream>

int main(int, char**)
{
    int sudoku[9][9];

    /* loadFromFile(sudoku, "./tests/test3.txt"); */
    loadFromFile(sudoku, "./tests/all.txt");
    solve(sudoku);

    /* sudoku.loadFromFile("./test-solution"); */
    /* sudoku.print(); */
    /* std::cout << sudoku.check() << std::endl; */
    return 0;
}
