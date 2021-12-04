#include <iostream>
#include "parser.hpp"

int main(int argc, char* argv[])
{
    arg_parser::Parser parser(
                    arg_parser::Argument("--out-file", true),       //required=true
                    arg_parser::Argument("--increment", false),    //required=false
                    arg_parser::Argument("--student-id", true)   //required=false
                );

    parser.parse(argc, argv);


    if (parser["--increment"].set)
        std::cout << "Increment: " << parser["--increment"].params[0] << '\n';
    else
        std::cout << "--increment not set\n";

    parser.print_map();

    return 0;
}
