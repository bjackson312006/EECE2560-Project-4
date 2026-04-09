#include "board.hpp"
#include <fstream>
#include <iostream>

/* Constructors. */
board::board(void) { return; }
board::board(const char* filename) { init(filename); }

/* Helpers to convert char digits to int indexes. Used to lookup values in the conflict arrays. */
static int _digit_toIndex(char digit) { return (digit - '0') - 1; }
static int _square_toIndex(int row, int col) { return (row / 3)*3 + (col/3); }

board::Error board::init(const char* filename) {
    /* Make sure board isn't already initialized. */
    if(this->_is_initialized) {
        std::cout << "[board::init()] ERROR: Tried initializing a board that's already initialized." << std::endl;
        return BOARD_ERROR;
    }

    /* Open file. */
    std::ifstream file(filename);
    if(!file) {
        std::cout << "[board::init()] ERROR: Failed to open `std::ifstream file`." << std::endl;
        return BOARD_ERROR;
    }

    /* Read in characters. */
    char ch; // Buffer for reading in character
    int i = 0; // Index for reading in file.
    while(file.get(ch) && i < 81) {

        /* Find current row and col. */
        int row = i/9;
        int col = i%9;

        /* If it's an empty, record that in the board and continue. */
        if(ch == '.') {
            this->_v[row][col] = '.';
            i++;
            continue;
        }

        /* At this point, skip anything that's not a valid digit char. */
        if(ch < '1' || ch > '9') {
            continue;
        }
        
        /* Make sure there's not a row conflict. */
        int ch_i = _digit_toIndex(ch);
        if(this->_valid_row[row][ch_i]) {
            std::cout << "[board::init()] ERROR: Invalid board due to row conflict." << std::endl;
            return BOARD_CONFLICT;
        }

        /* Make sure there's not a col conflict. */
        if(this->_valid_col[col][ch_i]) {
            std::cout << "[board::init()] ERROR: Invalid board due to col conflict." << std::endl;
            return BOARD_CONFLICT;
        }

        /* Make sure there's not a col conflict. */
        int square_i = _square_toIndex(row, col);
        if(this->_valid_square[square_i][ch_i]) {
            std::cout << "[board::init()] ERROR: Invalid board due to square conflict." << std::endl;
            return BOARD_CONFLICT;
        }

        /* If everything's valid, update the board and the conflict arrays. */
        this->_v[row][col] = ch;
        this->_valid_row[row][ch_i] = true;
        this->_valid_col[col][ch_i] = true;
        this->_valid_square[square_i][ch_i] = true;
        i++;
    }

    /* Make sure input file is the right length. */
    if(i != 81) {
        std::cout << "[board::init()] ERROR: Invalid infile size." << std::endl;
        return BOARD_ERROR;
    }

    /* Return successful. */
    this->_is_initialized = true;
    return BOARD_OK;
}

void board::print(void) {
    /* Print the board grid. */
    const char* hline = "+-------+-------+-------+";
    std::cout << hline << "\n";
    for(int row = 0; row < 9; row++) {
        std::cout << "| ";
        for(int col = 0; col < 9; col++) {
            char cell = this->_v[row][col];
            std::cout << (cell ? cell : '.');
            if     (col == 2 || col == 5) std::cout << " | ";
            else if(col < 8)              std::cout << " ";
        }
        std::cout << " |\n";
        if(row == 2 || row == 5 || row == 8)
            std::cout << hline << "\n";
    }

    /* Print conflict arrays (rows, cols, squares). */
    const char*   labels[3]  = { "Row", "Col", "Square" };
    bool        (*arrays[3])[9] = { _valid_row, _valid_col, _valid_square };

    std::cout << "\n";
    for(int a = 0; a < 3; a++) {
        std::cout << labels[a] << " conflicts:\n";
        std::cout << "      1  2  3  4  5  6  7  8  9\n";
        for(int i = 0; i < 9; i++) {
            std::cout << "  " << i << ":  ";
            for(int d = 0; d < 9; d++)
                std::cout << (arrays[a][i][d] ? 'x' : '.') << "  ";
            std::cout << "\n";
        }
        std::cout << "\n";
    }
}

board::Error board::add(int row, int col, char digit) {
    /* Make sure digit is valid. */
    if(digit < '1' || digit > '9') {
        std::cout << "[board::add()] ERROR: Invalid character passed into `digit`." << std::endl;
        return BOARD_ERROR;
    }

    /* Make sure board is initialized. */
    if(!this->_is_initialized) {
        std::cout << "[board::add()] ERROR: Board is not initialized." << std::endl;
        return BOARD_ERROR;
    }

    /* Make sure adding the value wouldn't produce any conflicts. */
    int digit_i = _digit_toIndex(digit);
    int square_i = _square_toIndex(row, col);
    if(this->_valid_row[row][digit_i]) {
        std::cout << "[board::add()] ERROR: Adding this digit would create a row conflict." << std::endl;
        return BOARD_CONFLICT;
    }
    if(this->_valid_col[col][digit_i]) {
        std::cout << "[board::add()] ERROR: Adding this digit would create a col conflict." << std::endl;
        return BOARD_CONFLICT;
    }
    if(this->_valid_square[square_i][digit_i]) {
        std::cout << "[board::add()] ERROR: Adding this digit would create a square conflict." << std::endl;
        return BOARD_CONFLICT;
    }

    /* If we're all good, then add in the digit and update the conflicts. */
    this->_v[row][col] = digit;
    this->_valid_row[row][digit_i] = true;
    this->_valid_col[col][digit_i] = true;
    this->_valid_square[square_i][digit_i] = true;
    return BOARD_OK;
}

bool board::isSolved(void) {
    for(int row = 0; row < 9; row++) {
        for(int col = 0; col < 9; col++) {
            if(this->_v[row][col] < '1' || this->_v[row][col] > '9') {
                std::cout << "Board is not solved." << std::endl;
                return false;
            }
        }
    }
    std::cout << "Board is solved!" << std::endl;
    return true;
}

board::Error board::clear(int row, int col) {
    /* Make sure board is initialized. */
    if(!this->_is_initialized) {
        std::cout << "[board::clear()] ERROR: Board is not initialized." << std::endl;
        return BOARD_ERROR;
    }

    /* Make sure the existing digit is valid and isn't already cleared. */
    char digit = this->_v[row][col];
    if(digit < '1' || digit > '9') {
        std::cout << "[board::clear()] ERROR: Cell is already empty, or otherwise not clearable." << std::endl;
        return BOARD_ERROR;
    }

    /* Clear the value and update conflicts. */
    int square_i = _square_toIndex(row, col);
    int digit_i = _digit_toIndex(digit);
    this->_valid_row[row][digit_i] = false;
    this->_valid_col[col][digit_i] = false;
    this->_valid_square[square_i][digit_i] = false;
    this->_v[row][col] = '.';
    return BOARD_OK;
}