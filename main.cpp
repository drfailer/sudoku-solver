#include "sudoku.hpp"
#include <iostream>

int main(int, char**)
{
    Sudoku sudoku;

    sudoku.loadFromFile("./tests/test2.txt");
    sudoku.resolve();

    /* sudoku.loadFromFile("./test-solution"); */
    /* sudoku.print(); */
    /* std::cout << sudoku.check() << std::endl; */
    return 0;
}
