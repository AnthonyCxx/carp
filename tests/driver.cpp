#ifndef CARP_DEBUG
    #error Can\'t run tests without the "CARP_DEBUG" flag.
#endif

#include "parser-tests.hh"
#include "argument-tests.hh"

//oh my god unit tests without a framework is hell
//why is c/c++'s infrastructure so bad
int main()
{
    ArgumentTests::driver();
    ParserTests::driver();
    std::cout << "All tests passed successfully!\n";

    return 0;
}