#pragma once
#pragma GCC diagnostic ignored "-Wwrite-strings"

#ifdef CARP_DEBUG

#include <iostream>
#include <cassert>
#include <regex>

#include "test-wrapper.hh"
#include "../src/parser.hh"

namespace tests {
    class ParserTests
    {
        public:
        static void default_constructor()
        {
            carp::Parser parser;
        }

        static void parameterized_constructor()
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
        }

        static void cmdarg_regex()
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
        }

        static void get_cmdarg()
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
        }

        static void parse_flags()
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
        }

        static void help()
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

            char* argv[] { "progname", "--foo", "-b", "--help", "not an argument", "32", "-32" };
            int argc = 7;

            parser.parse(argc, argv);
        }

        static void driver() 
        {
            test(__FILE__, stringify(default_constructor), default_constructor);
            test(__FILE__, stringify(parameterized_constructor), parameterized_constructor);
            test(__FILE__, stringify(cmdarg_regex), cmdarg_regex);
            test(__FILE__, stringify(get_cmdarg), get_cmdarg);
            test(__FILE__, stringify(parse_flags), parse_flags);
            //test(__FILE__, stringify(help), help);
            std::cout << '\n';
        }
    };
}
#endif