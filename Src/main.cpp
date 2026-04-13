//
// Brian Mack, Juan Ipina, Blake Jackson, James Su
// Project #4 Sudoku (Part A)
// Main program file for reading and printing Sudoku boards
//
#include "board.hpp"

#include <fstream>
#include <iostream>
#include <string>

static void process_single_serialized_board_line(
   const std::string &serialized_board_line)
/* Initializes, prints, and checks one board from serialized_board_line
 * parameters - serialized_board_line: one board string from input file
 * assumptions - serialized_board_line may include non-digit separators
 */
{
   board sudoku_board;

   board::Error initialization_status =
      sudoku_board.init_from_serialized_board_line(serialized_board_line);

   if (initialization_status != board::BOARD_OK)
   {
      return;
   }

   sudoku_board.print();
   sudoku_board.isSolved();
   std::cout << std::endl;
} // end process_single_serialized_board_line

static void process_all_serialized_board_lines_from_input_file(
   std::ifstream &boards_input_file)
/* Reads each line from boards_input_file and processes each board line
 * parameters - boards_input_file: open input stream for board lines
 * assumptions - stream is already opened and readable
 */
{
   std::string serialized_board_line;

   while (std::getline(boards_input_file, serialized_board_line))
   {
      if (serialized_board_line.empty())
      {
         continue;
      }

      process_single_serialized_board_line(serialized_board_line);
   } // end while
} // end process_all_serialized_board_lines_from_input_file

int main(int argc, char *argv[])
/* Reads a board file and prints each board and conflict state
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

   process_all_serialized_board_lines_from_input_file(boards_input_file);

   return 0;
} // end main
