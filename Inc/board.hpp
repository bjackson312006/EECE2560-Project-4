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

        /* Constructors */
        board(const char* filename); // Constructor to automatically initialize the board.
        board(void); // Constructor that returns an uninitialized board (it needs to be manually initialized via init()).

        Error init(const char* filename); // Initializes a board.
        void print(void); // Prints the board and conflict arrays.
        Error add(int row, int col, char value); // Add a value to a specific cell.
        Error clear(int row, int col); // Clear a value from a specific cell.
        bool isSolved(void); // Returns true if the board is fully solved, false otherwise.
};