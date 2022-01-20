# cmdline-arg-parser
A light-weight commandline argument parser, originally built to support my [password cracker](https://github.com/EthanC2/password-cracker). Being light-weight, the parser
makes no unncessary allocations (using C++17's [string_view](https://docs.microsoft.com/en-us/cpp/standard-library/string-view?view=msvc-170)) and only supports the necessary
features for a parser: enforcing required arguments, enforcing a minimum number of arguments, and including descriptions of each argument.

Here is a short example of how to use the parser:
```C++
//Commandline argument parser + argument list
arg_parser::Parser parser(
                            //Format:  cmd arg=string, # of parameters=uint, is_required=bool, description=string
                            arg_parser::Argument("--help", 0, false, "displays the help screen"),            
                            arg_parser::Argument("-h", 0, false, "displays the help screen"),                 
                            arg_parser::Argument("--hashfile", 1, true, "takes the list of hashed passwords"),   
                            arg_parser::Argument("--dict", 1, false, "source dictionary of passwords"),
                            arg_parser::Argument("--brute", 1, false, "runs the brute force algorithm. 1 arg: size of password")
                         );

//Parse the commandline arguments
parser.parse(argc, argv);

if (not parser["--hashfile"].is_set())
{
    std::clog << "usage: ./a.out <password_hashlist>\n";
    exit(1);
}
```
> Note, the `Parser.validate_args()` method is not automatically called by the parser's constructor to allow the programmer to intervene, allowing them to create help screens
> and other logic.
