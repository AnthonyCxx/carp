#pragma once

#ifdef CARP_DEBUG

#include <iostream>
#include <cassert>
#include <regex>

#include "../parser.hh"

class ParserTests
{
    public:
    static void test_default_constructor()
    {
        carp::Parser parser();

        std::cout << __FUNCTION__ << " in " << __FILE__ << " passed successfully\n";
    }

    static void test_parameterized_constructor()
    {
        carp::Parser parser(
            carp::CmdArg("foo")
                    .abbreviation("f")
                    .required(true)
                    .help("description for arg foo")
                    .build(),

            carp::CmdArg("bar")
                    .abbreviation("b")
                    .help("description for arg bar")
                    .build()
        );

        std::cout << __FUNCTION__ << " in " << __FILE__ << " passed successfully\n";
    }

    static void test_cmdarg_regex()
    {
        const static std::regex arg_pattern(R"(^(-|--)[a-zA-Z]+[a-zA-Z-]*$)");
        
        assert(std::regex_match("--flag", arg_pattern));
        assert(std::regex_match("--long-flag", arg_pattern));
        assert(std::regex_match("-short", arg_pattern));
        assert(std::regex_match("-s", arg_pattern));
        
        assert(not std::regex_match("abc--flag", arg_pattern));
        assert(not std::regex_match("--flag12", arg_pattern));
        assert(not std::regex_match(" --flag  ", arg_pattern));
        assert(not std::regex_match(" --flag", arg_pattern));
        assert(not std::regex_match("--flag  ", arg_pattern));
        assert(not std::regex_match("--", arg_pattern));
        assert(not std::regex_match("-", arg_pattern));

        std::cout << __FUNCTION__ << " in " << __FILE__ << " passed successfully\n";
    }

    static void test_get_cmdarg()
    {
        carp::Parser parser(
            carp::CmdArg("foo")
                    .abbreviation("f")
                    .required(true)
                    .help("description for arg foo")
                    .build(),

            carp::CmdArg("bar")
                    .abbreviation("b")
                    .help("description for arg bar")
                    .build()
        );

        assert(parser.get_arg("foo") == parser.get_arg("--foo") and parser.get_arg("--foo") == parser.get_arg("-f"));
        assert(parser.get_arg("bar") == parser.get_arg("--bar") and parser.get_arg("--bar") == parser.get_arg("-b"));
        std::cout << __FUNCTION__ << " in " << __FILE__ << " passed successfully\n";
    }

    static void test_parse_flags()
    {
        char* argv[] { "progname", "--foo", "-b", "--str1", "not an argument", "32", "-32" };
        int argc = 7;

        carp::Parser parser(
            carp::CmdArg("foo")
                    .abbreviation("f")
                    .required(true)
                    .help("description for arg foo")
                    .build(),

            carp::CmdArg("bar")
                    .abbreviation("b")
                    .help("description for arg bar")
                    .build()
        );

        parser.parse(argc, argv);
        assert(parser.get_arg("foo")->set == true);
        assert(parser.get_arg("bar")->set == true);

        std::cout << __FUNCTION__ << " in " << __FILE__ << " passed successfully\n";
    }

    static void driver() 
    {
        test_default_constructor();
        test_parameterized_constructor();
        test_cmdarg_regex();
        test_get_cmdarg();
        test_parse_flags();
        std::cout << '\n';
    }
};

#endif