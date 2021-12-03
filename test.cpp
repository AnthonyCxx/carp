#include <iostream>
#include "parser.hpp"

int main(int argc, char* argv[])
{
    Parser parser;
    parser.parse(argc, argv)
    parser.printMap();

    return 0;
}
