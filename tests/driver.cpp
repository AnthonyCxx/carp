#ifndef CARP_DEBUG
    #error Cannot run tests without the "CARP_DEBUG" flag.
#endif

#include "parser-tests.hh"
#include "argument-tests.hh"

//oh my god unit tests without a framework is so bad
//why is c/c++'s infrastructure so bad
int main()
{
    tests::ArgumentTests::driver();
    tests::ParserTests::driver();
    std::cout << "All tests passed successfully!\n";

    return 0;
}