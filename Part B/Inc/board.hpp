//
// Brian Mack, Juan Ipina, Blake Jackson, James Su
// Project #4 Sudoku (Part B)
// Header file for the board class
//
#include <string>

class board
{
   private:
      /* 9x9 Sudoku Board. */
      char _v[9][9] = { 0 };

      /* Conflict Arrays. */
      bool _valid_col[9][9] = { false };
      bool _valid_row[9][9] = { false };
      bool _valid_square[9][9] = { false };

      /* Tracks whether board data has been initialized. */
      bool _is_initialized = false;

   public:
      /* Board Return Type. */
      enum Error
      {
         BOARD_OK,
         BOARD_CONFLICT,
         BOARD_ERROR
      }; // end enum Error

   private:
      Error _validate_uninitialized_for_serialized_line_init(void);
      /* Validates serialized-line init precondition.
       * parameters - none
       * assumptions - board object exists in a valid state
       */

      bool _serialized_character_is_blank(char ch);
      /* Checks whether serialized character is blank.
       * parameters - ch: character read from serialized board line
       * assumptions - ch is any ASCII character
       */

      bool _serialized_character_is_digit(char ch);
      /* Checks whether serialized character is Sudoku digit.
       * parameters - ch: character read from serialized board line
       * assumptions - ch is any ASCII character
       */

      void _compute_row_and_col_from_serialized_index(
         int serialized_index,
         int &row,
         int &col);
      /* Converts linear serialized index to board coordinates.
       * parameters - serialized_index: 0..80 cell index
       *              row: output row index
       *              col: output col index
       * assumptions - serialized_index is non-negative
       */

      Error _validate_serialized_digit_conflicts(
         int row,
         int col,
         char digit);
      /* Validates row, col, and square conflicts for one digit placement.
       * parameters - row: board row index
       *              col: board col index
       *              digit: digit character '1'..'9'
       * assumptions - row/col are in range and digit is valid
       */

      void _write_serialized_blank_to_board(int row, int col);
      /* Writes blank marker to one board cell.
       * parameters - row: board row index
       *              col: board col index
       * assumptions - row/col are in range
       */

      void _write_serialized_digit_to_board_and_conflict_arrays(
         int row,
         int col,
         char digit);
      /* Writes one digit and updates conflict arrays.
       * parameters - row: board row index
       *              col: board col index
       *              digit: digit character '1'..'9'
       * assumptions - placement has already passed conflict checks
       */

      Error _process_single_serialized_character_and_advance_count(
         char ch,
         int &parsed_cell_count);
      /* Processes one serialized character and updates parsed count.
       * parameters - ch: character from serialized board line
       *              parsed_cell_count: current parsed cell count
       * assumptions - parsed_cell_count is in range 0..81
       */

      Error _parse_serialized_line_and_update_board_state(
         const std::string &serialized_board_line,
         int &parsed_cell_count);
      /* Parses serialized board line and updates board values/conflicts.
       * parameters - serialized_board_line: board line from input
       *              parsed_cell_count: output parsed cell count
       * assumptions - board has not been initialized
       */

      bool _solve_recursive(unsigned long long &recursive_call_count);
      /* Recursive backtracking solver; increments recursive_call_count
       * on each entry.
       * parameters - recursive_call_count: running total of calls
       * assumptions - board is initialized with a valid puzzle state
       */

      void _solver_place_digit(int row, int col, char digit);
      /* Places digit on board and updates conflict arrays (solver path).
       * parameters - row, col: cell indices; digit: '1'..'9'
       * assumptions - cell is '.' and placement is legal
       */

      void _solver_clear_cell(int row, int col);
      /* Clears a filled cell and updates conflict arrays (solver path).
       * parameters - row, col: cell indices
       * assumptions - cell holds '1'..'9'
       */

   public:
      board(const char *filename);
      /* Constructor that initializes board from filename.
       * parameters - filename: input board filename
       * assumptions - filename points to readable file path
       */

      board(void);
      /* Constructor that creates an uninitialized board.
       * parameters - none
       * assumptions - object storage is valid
       */

      Error init(const char *filename);
      /* Initializes board from filename.
       * parameters - filename: input board filename
       * assumptions - board is uninitialized
       */

      Error init_from_serialized_board_line(
         const std::string &serialized_board_line);
      /* Initializes board from one serialized board line.
       * parameters - serialized_board_line: board text representation
       * assumptions - board is uninitialized
       */

      void printGrid(void) const;
      /* Prints the 9x9 Sudoku grid only (no conflict tables).
       * parameters - none
       * assumptions - board has been initialized
       */

      void print(void);
      /* Prints board and conflict arrays.
       * parameters - none
       * assumptions - board has been initialized
       */

      Error add(int row, int col, char value);
      /* Adds value to one board cell and updates conflicts.
       * parameters - row: board row index
       *              col: board col index
       *              value: digit character '1'..'9'
       * assumptions - board is initialized and row/col are in range
       */

      Error clear(int row, int col);
      /* Clears one board cell and updates conflicts.
       * parameters - row: board row index
       *              col: board col index
       * assumptions - board is initialized and row/col are in range
       */

      bool isSolved(void);
      /* Checks whether board has all cells filled with valid digits.
       * parameters - none
       * assumptions - board has been initialized
       */

      bool findNextBlank(int &row, int &col) const;
      /* Finds the next empty cell using minimum remaining values (MRV)
       * to reduce recursive work.
       * parameters - row, col: output indices when a blank exists
       * returns - false when no blank remains (puzzle complete)
       * assumptions - board is initialized
       */

      bool isLegal(int row, int col, char digit) const;
      /* Returns true if digit may be placed at row,col under Sudoku rules.
       * parameters - row, col: cell indices; digit: '1'..'9'
       * assumptions - board is initialized
       */

      bool solve(unsigned long long &recursive_call_count);
      /* Solves the puzzle by recursion and backtracking.
       * parameters - recursive_call_count: set to number of recursive calls
       * returns - true if a solution was found
       * assumptions - board is initialized from a valid puzzle line
       */
}; // end class board
