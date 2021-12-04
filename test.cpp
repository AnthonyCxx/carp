#include <iostream>
#include "parser.hpp"

int main(int argc, char* argv[])
{
    arg_parser::Parser parser(
                    arg_parser::Argument("--out-file", true),       //required=true
                    arg_parser::Argument("--increment", false),    //required=false
                    arg_parser::Argument("--student-id", false)   //required=false
                );

    parser.parse(argc, argv);




    parser.print_map();

    return 0;
}
