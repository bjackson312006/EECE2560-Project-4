#include "board.hpp"

int main(void) {
    /* first one */
    board b1("sudoku1.txt");
    b1.print();
    b1.isSolved();

    /* second one */
    board b2("sudoku2.txt");
    b2.print();
    b2.isSolved();

    /* third one */
    board b3("sudoku3.txt");
    b3.print();
    b3.isSolved();
    return 0;
}