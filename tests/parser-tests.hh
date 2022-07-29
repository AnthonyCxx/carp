#pragma once
#pragma GCC diagnostic ignored "-Wwrite-strings"

#ifdef CARP_DEBUG

#include <iostream>
#include <cassert>
#include <regex>

#include "test-utils.hh"
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
            char* argv[] { "program_name", "--foo", "-b", "--str1", "not an argument", "32", "-32" };
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

            char* argv[] { "program_name", "--foo", "-b", "--help", "not an argument", "32", "-32" };
            int argc = 7;

            parser.parse(argc, argv);
        }

        static void action_set_true()
        {
            carp::Parser parser(
                carp::CmdArg("foo")
                        .abbreviation("f")
                        .action(carp::ArgAction::SetTrue)
                        .build(),

                carp::CmdArg("bar")
                        .abbreviation("b")
                        .action(carp::ArgAction::SetTrue)
                        .build()
            );

            char* argv[] { "program_name", "--foo", "-b" };
            int argc = 3;

            parser.parse(argc, argv);
            assert(parser.get_arg("foo")->values[0] == "true");
            assert(parser.get_arg("bar")->values[0] == "true");
        }

        static void action_set_false()
        {
            carp::Parser parser(
                carp::CmdArg("foo")
                        .abbreviation("f")
                        .action(carp::ArgAction::SetFalse)
                        .build(),

                carp::CmdArg("bar")
                        .abbreviation("b")
                        .action(carp::ArgAction::SetFalse)
                        .build()
            );

            char* argv[] { "program_name", "--foo", "-b" };
            int argc = 3;

            parser.parse(argc, argv);
            assert(parser.get_arg("bar")->values[0] == "false");
            assert(parser.get_arg("bar")->values[0] == "false");
        }

        static void action_store_single()
        {
            carp::Parser parser(
                carp::CmdArg("foo")
                        .abbreviation("f")
                        .action(carp::ArgAction::StoreSingle)
                        .build(),

                carp::CmdArg("bar")
                        .abbreviation("b")
                        .action(carp::ArgAction::StoreSingle)
                        .build()
            );

            char* argv[] { "program_name", "--foo", "a", "b", "c", "-b", "this is a param", "12" };
            int argc = 8;

            parser.parse(argc, argv);
            auto arg = parser.get_arg("foo");

            assert(parser.get_arg("foo")->values[0] == "c");
            assert(parser.get_arg("bar")->values[0] == "12");
        }

        static void action_store_many()
        {
            carp::Parser parser(
                carp::CmdArg("foo")
                        .abbreviation("f")
                        .action(carp::ArgAction::StoreMany)
                        .build(),

                carp::CmdArg("bar")
                        .abbreviation("b")
                        .action(carp::ArgAction::StoreMany)
                        .build()
            );

            char* argv[] { "program_name", "--foo", "a", "b", "c", "-b", "this is a param", "1" };
            int argc = 8;

            parser.parse(argc, argv);
            auto foo = parser.get_arg("foo");
            auto bar = parser.get_arg("bar");

            assert(are_equal_vectors(foo->values, {"a", "b", "c"}));
            assert(are_equal_vectors(bar->values, {"this is a param", "1"}));
        }

        static void action_count()
        {
            carp::Parser parser(
                carp::CmdArg("flag")
                        .abbreviation("f")
                        .action(carp::ArgAction::Count)
                        .build(),

                carp::CmdArg("count-me")
                        .abbreviation("c")
                        .action(carp::ArgAction::Count)
                        .build()
            );

            char* argv[] { "program_name", "--flag", "-f", "--flag", "--count-me", "-c", "--flag" };
            int argc = 7;

            parser.parse(argc, argv);
            assert(parser.get_arg("flag")->count == 4);
            assert(parser.get_arg("count-me")->count == 2);
        }

        static void driver() 
        {
            test(__FILE__, stringify(default_constructor), default_constructor);
            test(__FILE__, stringify(parameterized_constructor), parameterized_constructor);
            test(__FILE__, stringify(cmdarg_regex), cmdarg_regex);
            test(__FILE__, stringify(get_cmdarg), get_cmdarg);
            test(__FILE__, stringify(parse_flags), parse_flags);
            test(__FILE__, stringify(action_set_true), action_set_true);
            test(__FILE__, stringify(action_set_false), action_set_false);
            test(__FILE__, stringify(action_store_single), action_store_single);
            test(__FILE__, stringify(action_store_many), action_store_many);
            test(__FILE__, stringify(action_count), action_count);
            //test(__FILE__, stringify(help), help);
            std::cout << '\n';
        }
    };
}
#endif