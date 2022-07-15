# C.A.R.P.: A Commandline Argument Parser
A light-weight commandline argument parser, originally built to support my [password cracker](https://github.com/EthanC2/password-cracker). This parser was designed to make
processing commandline arguments easier by allowing the programmer to directly query whether or not an argument was provided; in addition to querying arguments, the parser
supports querying the parameters of and argument. Being light-weight, the parser makes no unncessary allocations (using C++17's [string_view](https://docs.microsoft.com/en-us/cpp/standard-library/string-view?view=msvc-170)) and only supports the necessary
features for a parser: 

# Planned Features
- Special handling for '--help'