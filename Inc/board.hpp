//
// Brian Mack, Juan Ipina, Blake Jackson, James Su
// Project #4 Sudoku (Part A)
// Header file for the board class
//
#include <string>

class board {
    private:
        /* 9x9 Sudoku Board. */
        char _v[9][9] = { 0 };

        /* Conflict Arrays. */
        bool _valid_col[9][9] = { false };
        bool _valid_row[9][9] = { false };
        bool _valid_square[9][9] = { false };

        bool _is_initialized = false; /* Tracks whether or not a*/

    public:
        /* Board Return Type. */
        enum Error {
            BOARD_OK,
            BOARD_CONFLICT,
            BOARD_ERROR
        };

    private:
        Error _validate_uninitialized_for_serialized_line_init(void);
        // Returns BOARD_OK only when board is not already initialized

        bool _serialized_character_is_blank(char ch);
        // Returns true when ch is the blank board marker '.'

        bool _serialized_character_is_digit(char ch);
        // Returns true when ch is a valid Sudoku digit character

        void _compute_row_and_col_from_serialized_index(
            int serialized_index,
            int &row,
            int &col);
        // Converts serialized board index into row and col indices

        Error _validate_serialized_digit_conflicts(
            int row,
            int col,
            char digit);
        // Returns BOARD_CONFLICT if placing digit at (row, col) conflicts

        void _write_serialized_blank_to_board(int row, int col);
        // Writes blank marker into board cell (row, col)

        void _write_serialized_digit_to_board_and_conflict_arrays(
            int row,
            int col,
            char digit);
        // Writes digit and updates row, col, and square conflict arrays

        Error _process_single_serialized_character_and_advance_count(
            char ch,
            int &parsed_cell_count);
        // Processes one serialized character and advances parsed cell count

        Error _parse_serialized_line_and_update_board_state(
            const std::string &serialized_board_line,
            int &parsed_cell_count);
        // Parses one serialized board line and updates board state

    public:

        /* Constructors */
        board(const char* filename);
        // Constructor to automatically initialize the board

        board(void);
        // Constructor that returns an uninitialized board
        // (it needs to be manually initialized via init())

        Error init(const char* filename);
        // Initializes a board

        Error init_from_serialized_board_line(
            const std::string &serialized_board_line);
        // Initializes a board from one input line

        void print(void);
        // Prints the board and conflict arrays

        Error add(int row, int col, char value);
        // Add a value to a specific cell

        Error clear(int row, int col);
        // Clear a value from a specific cell

        bool isSolved(void);
        // Returns true if the board is fully solved, false otherwise
};