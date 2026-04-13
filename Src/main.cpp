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
/* Initializes, prints, and checks one board from serialized_board_line */
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
}

static void process_all_serialized_board_lines_from_input_file(
   std::ifstream &boards_input_file)
/* Reads each line from boards_input_file and processes each board line */
{
   std::string serialized_board_line;

   while (std::getline(boards_input_file, serialized_board_line))
   {
      if (serialized_board_line.empty())
      {
         continue;
      }

      process_single_serialized_board_line(serialized_board_line);
   }
}

int main(int argc, char *argv[])
/* Reads a board file and prints each board and conflict state */
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
}