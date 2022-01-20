# cmdline-arg-parser
A light-weight commandline argument parser, originally built to support my [password cracker](https://github.com/EthanC2/password-cracker). Being light-weight, this parser
uses C++17's [string_view](https://docs.microsoft.com/en-us/cpp/standard-library/string-view?view=msvc-170) to avoid unnecessary allocations. The only allocations made are to
store the `arg_parser::Argument()` objects, storing the arugment to parse for, the mininum number of arguments, whether or not the argument is required, and its description.
