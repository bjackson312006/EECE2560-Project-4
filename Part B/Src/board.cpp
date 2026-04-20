//
// Brian Mack, Juan Ipina, Blake Jackson, James Su
// Project #4 Sudoku (Part B)
// Implementation file for the board class
//
#include "board.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

/* Constructors. */
board::board(void)
{
    return;
} // end board constructor

board::board(const char *filename)
{
    init(filename);
} // end board constructor

/* Helper converters used by conflict arrays. */
static int _digit_toIndex(char digit)
/* Converts a digit character into 0-based index
 * parameters - digit: character expected in range '1'..'9'
 * assumptions - caller passes a valid digit character
 */
{
    return (digit - '0') - 1;
} // end _digit_toIndex

static int _square_toIndex(int row, int col)
/* Converts row and col into 0-based 3x3 square index
 * parameters - row: board row index
 *              col: board col index
 * assumptions - row and col are in range 0..8
 */
{
    return (row / 3) * 3 + (col / 3);
} // end _square_toIndex

static bool _cell_position_isInBounds(int row, int col)
/* Checks whether board position indices are in bounds
 * parameters - row: board row index
 *              col: board col index
 * assumptions - row and col may be any int values
 */
{
    return ((row >= 0) && (row < 9) && (col >= 0) && (col < 9));
} // end _cell_position_isInBounds

board::Error board::init(const char *filename)
{
    /* Make sure board isn't already initialized. */
    if (this->_is_initialized)
    {
        std::cout << "[board::init()] ERROR: Tried initializing a board that's "
                  << "already initialized." << std::endl;
        return BOARD_ERROR;
    }

    /* Open file. */
    std::ifstream file(filename);
    if (!file)
    {
        std::cout << "[board::init()] ERROR: Failed to open "
                  << "`std::ifstream file`." << std::endl;
        return BOARD_ERROR;
    }

    /* Read in characters. */
    char ch;  // Buffer for reading in character
    int i = 0;  // Index for reading in file.
    while (file.get(ch) && i < 81)
    {

        /* Find current row and col. */
        int row = i / 9;
        int col = i % 9;

        /* If it's an empty, record that in the board and continue. */
        if (ch == '.')
        {
            this->_v[row][col] = '.';
            i++;
            continue;
        }

        /* At this point, skip anything that's not a valid digit char. */
        if (ch < '1' || ch > '9')
        {
            continue;
        }

        /* Make sure there's not a row conflict. */
        int ch_i = _digit_toIndex(ch);
        if (this->_valid_row[row][ch_i])
        {
            std::cout << "[board::init()] ERROR: Invalid board due to row "
                      << "conflict." << std::endl;
            return BOARD_CONFLICT;
        }

        /* Make sure there's not a col conflict. */
        if (this->_valid_col[col][ch_i])
        {
            std::cout << "[board::init()] ERROR: Invalid board due to col "
                      << "conflict." << std::endl;
            return BOARD_CONFLICT;
        }

        /* Make sure there's not a col conflict. */
        int square_i = _square_toIndex(row, col);
        if (this->_valid_square[square_i][ch_i])
        {
            std::cout << "[board::init()] ERROR: Invalid board due to square "
                      << "conflict." << std::endl;
            return BOARD_CONFLICT;
        }

        /* If everything's valid, update the board and conflict arrays. */
        this->_v[row][col] = ch;
        this->_valid_row[row][ch_i] = true;
        this->_valid_col[col][ch_i] = true;
        this->_valid_square[square_i][ch_i] = true;
        i++;
    }

    /* Make sure input file is the right length. */
    if (i != 81)
    {
        std::cout << "[board::init()] ERROR: Invalid infile size." << std::endl;
        return BOARD_ERROR;
    }

    /* Return successful. */
    this->_is_initialized = true;
    return BOARD_OK;
} // end init

board::Error board::init_from_serialized_board_line(
    const std::string &serialized_board_line)
/* Initializes a board from one serialized input line */
{
    Error validation_status =
        _validate_uninitialized_for_serialized_line_init();
    if (validation_status != BOARD_OK)
    {
        return validation_status;
    }

    int parsed_cell_count = 0;
    Error parse_status = _parse_serialized_line_and_update_board_state(
        serialized_board_line,
        parsed_cell_count);
    if (parse_status != BOARD_OK)
    {
        return parse_status;
    }

    if (parsed_cell_count != 81)
    {
        std::cout << "[board::init_from_serialized_board_line()] ERROR: "
                  << "Invalid infile size." << std::endl;
        return BOARD_ERROR;
    }

    this->_is_initialized = true;
    return BOARD_OK;
} // end init_from_serialized_board_line

board::Error board::_validate_uninitialized_for_serialized_line_init(void)
/* Returns BOARD_OK only when board is not already initialized */
{
    if (this->_is_initialized)
    {
        std::cout << "[board::init_from_serialized_board_line()] ERROR: "
                  << "Tried initializing a board that's already initialized."
                  << std::endl;
        return BOARD_ERROR;
    }

    return BOARD_OK;
} // end _validate_uninitialized_for_serialized_line_init

bool board::_serialized_character_is_blank(char ch)
/* Returns true when ch is '.' */
{
    return (ch == '.');
} // end _serialized_character_is_blank

bool board::_serialized_character_is_digit(char ch)
/* Returns true when ch is between '1' and '9' */
{
    return (ch >= '1' && ch <= '9');
} // end _serialized_character_is_digit

void board::_compute_row_and_col_from_serialized_index(
    int serialized_index,
    int &row,
    int &col)
/* Converts serialized index into row and col values */
{
    row = serialized_index / 9;
    col = serialized_index % 9;
} // end _compute_row_and_col_from_serialized_index

board::Error board::_validate_serialized_digit_conflicts(
    int row,
    int col,
    char digit)
/* Returns BOARD_CONFLICT if digit conflicts at row/col */
{
    int digit_i = _digit_toIndex(digit);
    if (this->_valid_row[row][digit_i])
    {
        std::cout << "[board::init()] ERROR: Invalid board due to row "
                  << "conflict." << std::endl;
        return BOARD_CONFLICT;
    }

    if (this->_valid_col[col][digit_i])
    {
        std::cout << "[board::init()] ERROR: Invalid board due to col "
                  << "conflict." << std::endl;
        return BOARD_CONFLICT;
    }

    int square_i = _square_toIndex(row, col);
    if (this->_valid_square[square_i][digit_i])
    {
        std::cout << "[board::init()] ERROR: Invalid board due to square "
                  << "conflict." << std::endl;
        return BOARD_CONFLICT;
    }

    return BOARD_OK;
} // end _validate_serialized_digit_conflicts

void board::_write_serialized_blank_to_board(int row, int col)
/* Writes a blank marker to board cell row/col */
{
    this->_v[row][col] = '.';
} // end _write_serialized_blank_to_board

void board::_write_serialized_digit_to_board_and_conflict_arrays(
    int row,
    int col,
    char digit)
/* Writes digit to board and updates conflict arrays */
{
    int digit_i = _digit_toIndex(digit);
    int square_i = _square_toIndex(row, col);
    this->_v[row][col] = digit;
    this->_valid_row[row][digit_i] = true;
    this->_valid_col[col][digit_i] = true;
    this->_valid_square[square_i][digit_i] = true;
} // end _write_serialized_digit_to_board_and_conflict_arrays

board::Error board::_process_single_serialized_character_and_advance_count(
    char ch,
    int &parsed_cell_count)
/* Processes one serialized character and updates parsed count */
{
    int row = 0;
    int col = 0;
    _compute_row_and_col_from_serialized_index(parsed_cell_count, row, col);

    if (_serialized_character_is_blank(ch))
    {
        _write_serialized_blank_to_board(row, col);
        parsed_cell_count++;
        return BOARD_OK;
    }

    if (!_serialized_character_is_digit(ch))
    {
        return BOARD_OK;
    }

    Error conflict_status = _validate_serialized_digit_conflicts(row, col, ch);
    if (conflict_status != BOARD_OK)
    {
        return conflict_status;
    }

    _write_serialized_digit_to_board_and_conflict_arrays(row, col, ch);
    parsed_cell_count++;
    return BOARD_OK;
} // end _process_single_serialized_character_and_advance_count

board::Error board::_parse_serialized_line_and_update_board_state(
    const std::string &serialized_board_line,
    int &parsed_cell_count)
/* Parses serialized line and updates board state */
{
    std::istringstream line_stream(serialized_board_line);
    char ch;
    parsed_cell_count = 0;
    while (line_stream.get(ch) && parsed_cell_count < 81)
    {
        Error process_status =
            _process_single_serialized_character_and_advance_count(
                ch,
                parsed_cell_count);
        if (process_status != BOARD_OK)
        {
            return process_status;
        }
    }

    return BOARD_OK;
} // end _parse_serialized_line_and_update_board_state

void board::printGrid(void) const
{
    const char *hline = "+-------+-------+-------+";
    std::cout << hline << "\n";
    for (int row = 0; row < 9; row++)
    {
        std::cout << "| ";
        for (int col = 0; col < 9; col++)
        {
            char cell = this->_v[row][col];
            std::cout << (cell ? cell : '.');
            if (col == 2 || col == 5)
            {
                std::cout << " | ";
            }
            else if (col < 8)
            {
                std::cout << " ";
            }
        }
        std::cout << " |\n";
        if (row == 2 || row == 5 || row == 8)
        {
            std::cout << hline << "\n";
        }
    }
} // end printGrid

void board::print(void)
{
    /* Print the board grid. */
    this->printGrid();

    /* Print conflict arrays (rows, cols, squares). */
    const char *labels[3] = { "Row", "Col", "Square" };
    bool (*arrays[3])[9] = { _valid_row, _valid_col, _valid_square };

    std::cout << "\n";
    for (int a = 0; a < 3; a++)
    {
        std::cout << labels[a] << " conflicts:\n";
        std::cout << "      1  2  3  4  5  6  7  8  9\n";
        for (int i = 0; i < 9; i++)
        {
            std::cout << "  " << i << ":  ";
            for (int d = 0; d < 9; d++)
            {
                std::cout << (arrays[a][i][d] ? 'x' : '.') << "  ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
} // end print

board::Error board::add(int row, int col, char digit)
{
    /* Make sure digit is valid. */
    if (digit < '1' || digit > '9')
    {
        std::cout << "[board::add()] ERROR: Invalid character passed into "
                  << "`digit`." << std::endl;
        return BOARD_ERROR;
    }

    /* Make sure board is initialized. */
    if (!this->_is_initialized)
    {
        std::cout << "[board::add()] ERROR: Board is not initialized."
                  << std::endl;
        return BOARD_ERROR;
    }

    /* Make sure row and col are in bounds */
    if (!_cell_position_isInBounds(row, col))
    {
        std::cout << "[board::add()] ERROR: Cell position is out of bounds."
                  << std::endl;
        return BOARD_ERROR;
    }

    /* Make sure the cell is empty */
    if (this->_v[row][col] != '.')
    {
        std::cout << "[board::add()] ERROR: Cell is not empty." << std::endl;
        return BOARD_ERROR;
    }

    /* Make sure adding the value wouldn't produce any conflicts. */
    int digit_i = _digit_toIndex(digit);
    int square_i = _square_toIndex(row, col);
    if (this->_valid_row[row][digit_i])
    {
        std::cout << "[board::add()] ERROR: Adding this digit would create a "
                  << "row conflict." << std::endl;
        return BOARD_CONFLICT;
    }
    if (this->_valid_col[col][digit_i])
    {
        std::cout << "[board::add()] ERROR: Adding this digit would create a "
                  << "col conflict." << std::endl;
        return BOARD_CONFLICT;
    }
    if (this->_valid_square[square_i][digit_i])
    {
        std::cout << "[board::add()] ERROR: Adding this digit would create a "
                  << "square conflict." << std::endl;
        return BOARD_CONFLICT;
    }

    /* If we're all good, then add in the digit and update conflicts. */
    this->_v[row][col] = digit;
    this->_valid_row[row][digit_i] = true;
    this->_valid_col[col][digit_i] = true;
    this->_valid_square[square_i][digit_i] = true;
    return BOARD_OK;
} // end add

bool board::isSolved(void)
{
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            if (this->_v[row][col] < '1' || this->_v[row][col] > '9')
            {
                std::cout << "Board is not solved." << std::endl;
                return false;
            }
        }
    }
    std::cout << "Board is solved!" << std::endl;
    return true;
} // end isSolved

board::Error board::clear(int row, int col)
{
    /* Make sure board is initialized. */
    if (!this->_is_initialized)
    {
        std::cout << "[board::clear()] ERROR: Board is not initialized."
                  << std::endl;
        return BOARD_ERROR;
    }

    /* Make sure row and col are in bounds */
    if (!_cell_position_isInBounds(row, col))
    {
        std::cout << "[board::clear()] ERROR: Cell position is out of bounds."
                  << std::endl;
        return BOARD_ERROR;
    }

    /* Make sure existing digit is valid and isn't already cleared. */
    char digit = this->_v[row][col];
    if (digit < '1' || digit > '9')
    {
        std::cout << "[board::clear()] ERROR: Cell is already empty, or "
                  << "otherwise not clearable." << std::endl;
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
} // end clear

bool board::isLegal(int row, int col, char digit) const
/* Returns true when digit can be placed at row,col */
{
    if (!_cell_position_isInBounds(row, col))
    {
        return false;
    }

    if (digit < '1' || digit > '9')
    {
        return false;
    }

    if (this->_v[row][col] != '.')
    {
        return false;
    }

    int digit_i = _digit_toIndex(digit);
    int square_i = _square_toIndex(row, col);
    if (this->_valid_row[row][digit_i])
    {
        return false;
    }
    if (this->_valid_col[col][digit_i])
    {
        return false;
    }
    if (this->_valid_square[square_i][digit_i])
    {
        return false;
    }

    return true;
} // end isLegal

bool board::findNextBlank(int &row, int &col) const
/* Picks an empty cell using minimum remaining values (MRV) ordering */
{
    int best_row = -1;
    int best_col = -1;
    int best_choice_count = 10;

    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            if (this->_v[r][c] != '.')
            {
                continue;
            }

            int choice_count = 0;
            for (char d = '1'; d <= '9'; d++)
            {
                if (this->isLegal(r, c, d))
                {
                    choice_count++;
                }
            }

            if (choice_count < best_choice_count)
            {
                best_choice_count = choice_count;
                best_row = r;
                best_col = c;
            }
        }
    }

    if (best_row < 0)
    {
        return false;
    }

    row = best_row;
    col = best_col;
    return true;
} // end findNextBlank

void board::_solver_place_digit(int row, int col, char digit)
/* Updates board state for one solver placement */
{
    int digit_i = _digit_toIndex(digit);
    int square_i = _square_toIndex(row, col);
    this->_v[row][col] = digit;
    this->_valid_row[row][digit_i] = true;
    this->_valid_col[col][digit_i] = true;
    this->_valid_square[square_i][digit_i] = true;
} // end _solver_place_digit

void board::_solver_clear_cell(int row, int col)
/* Reverts one solver placement */
{
    char digit = this->_v[row][col];
    int digit_i = _digit_toIndex(digit);
    int square_i = _square_toIndex(row, col);
    this->_valid_row[row][digit_i] = false;
    this->_valid_col[col][digit_i] = false;
    this->_valid_square[square_i][digit_i] = false;
    this->_v[row][col] = '.';
} // end _solver_clear_cell

bool board::_solve_recursive(unsigned long long &recursive_call_count)
/* Recursion with backtracking; counts each entry */
{
    recursive_call_count++;

    int row = 0;
    int col = 0;
    if (!this->findNextBlank(row, col))
    {
        return true;
    }

    for (char digit = '1'; digit <= '9'; digit++)
    {
        if (!this->isLegal(row, col, digit))
        {
            continue;
        }

        this->_solver_place_digit(row, col, digit);
        if (this->_solve_recursive(recursive_call_count))
        {
            return true;
        }

        this->_solver_clear_cell(row, col);
    }

    return false;
} // end _solve_recursive

bool board::solve(unsigned long long &recursive_call_count)
{
    if (!this->_is_initialized)
    {
        std::cout << "[board::solve()] ERROR: Board is not initialized."
                  << std::endl;
        return false;
    }

    recursive_call_count = 0;
    return this->_solve_recursive(recursive_call_count);
} // end solve
