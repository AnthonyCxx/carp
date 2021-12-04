#include <iostream>
#include "parser.hpp"

int main(int argc, char* argv[])
{
    Parser parser;
    parser.parse(argc, argv);
    parser.print_map();

    return 0;
}
