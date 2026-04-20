//
// Brian Mack, Juan Ipina, Blake Jackson, James Su
// Project #4 Sudoku (Part B)
// Main program: load boards, solve with backtracking, report call counts
//
#include "board.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

static void extract_serialized_board_strings_from_input_stream(
   std::istream &boards_input_stream,
   std::vector<std::string> &serialized_board_strings)
/* Builds one string per board by taking every '.' and digit '1'..'9' in
 * order until 81 such characters are collected, then starts the next board.
 * parameters - boards_input_stream: stream positioned at start of content
 *              serialized_board_strings: output list of 81-char board lines
 * assumptions - stream may contain newlines or other separators between cells
 */
{
   std::string current_board_chunk;

   char ch = 0;
   while (boards_input_stream.get(ch))
   {
      if (ch == '.' || (ch >= '1' && ch <= '9'))
      {
         current_board_chunk.push_back(ch);
         if (current_board_chunk.size() == 81)
         {
            serialized_board_strings.push_back(current_board_chunk);
            current_board_chunk.clear();
         }
      }
   }
} // end extract_serialized_board_strings_from_input_stream

static void process_single_board_string(
   const std::string &serialized_board_line,
   unsigned long long &running_total_recursive_calls,
   unsigned long long &boards_included_in_average)
/* Initializes one board, solves it, prints the grid and per-board stats.
 * parameters - serialized_board_line: exactly 81 relevant characters
 *              running_total_recursive_calls: incremented by this board
 *              boards_included_in_average: incremented after solve() runs
 */
{
   board sudoku_board;

   board::Error initialization_status =
      sudoku_board.init_from_serialized_board_line(serialized_board_line);

   if (initialization_status != board::BOARD_OK)
   {
      return;
   }

   unsigned long long recursive_calls_for_this_board = 0;
   bool solved_ok =
      sudoku_board.solve(recursive_calls_for_this_board);

   sudoku_board.printGrid();
   std::cout << std::endl;

   if (!solved_ok)
   {
      std::cout << "No solution found for this board." << std::endl;
   }

   std::cout << "Recursive calls for this board: "
             << recursive_calls_for_this_board << std::endl;
   running_total_recursive_calls += recursive_calls_for_this_board;
   boards_included_in_average++;

   std::cout << std::endl;
} // end process_single_board_string

int main(int argc, char *argv[])
/* Reads board strings from a file, solves each, prints solutions and call
 * statistics, then prints total and average recursive calls.
 * parameters - argc: number of command-line arguments
 *              argv: command-line argument array
 * assumptions - input file path is passed as argv[1]
 */
{
   if (argc != 2)
   {
      return 1;
   }

   std::ifstream boards_input_file(argv[1]);

   if (!boards_input_file)
   {
      std::cout << "ERROR: Could not open input file \"" << argv[1]
                << "\"." << std::endl;
      return 1;
   }

   std::vector<std::string> serialized_board_strings;
   extract_serialized_board_strings_from_input_stream(
      boards_input_file,
      serialized_board_strings);

   if (serialized_board_strings.empty())
   {
      std::cout << "ERROR: No complete 81-cell boards were found in \""
                << argv[1] << "\"." << std::endl;
      return 1;
   }

   unsigned long long running_total_recursive_calls = 0;
   unsigned long long boards_included_in_average = 0;

   for (const std::string &one_board_line : serialized_board_strings)
   {
      process_single_board_string(
         one_board_line,
         running_total_recursive_calls,
         boards_included_in_average);
   }

   std::cout << "Total recursive calls: " << running_total_recursive_calls
             << std::endl;

   if (boards_included_in_average > 0)
   {
      double average_calls =
         static_cast<double>(running_total_recursive_calls) /
         static_cast<double>(boards_included_in_average);
      std::cout << "Average recursive calls per board: "
                << std::fixed << std::setprecision(2) << average_calls
                << std::endl;
   }

   return 0;
} // end main
