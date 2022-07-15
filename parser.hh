#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <string_view>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <regex>
#include <type_traits>
#include <cstdlib>

#include "argument.hh"

namespace carp
{
    class Parser final
    {
        public:
            std::unordered_map<std::string, std::shared_ptr<CmdArg>> arguments;

        public:
            template <typename ...Args>
            Parser(Args...);

            void parse(int, char*[]);
            const std::shared_ptr<CmdArg> get_arg(std::string) const;
            void help() const;

            #ifdef CARP_DEBUG
                void print_all_arguments() const;
            #endif
    };

    template <typename ...Args>
    Parser::Parser(Args... args)
    {
        static_assert((std::is_same_v<Args, std::shared_ptr<CmdArg>> and ...), "[CARP] Error: Parser constructor only accepts std::shared_ptr<carp::CmdArg> objects! Did you forget the '.build()' on the end of any CmdArgs?");
        (arguments.insert({args->identifier, args}), ...);
        (arguments.insert({args->long_name, args}), ...);
        (arguments.insert({args->short_name, args}), ...);
        

        std::shared_ptr<carp::CmdArg> help = carp::CmdArg("help")
                                                .abbreviation("h")
                                                .help("displays this help screen")
                                                .build();

        arguments.insert({help->identifier, help});
        arguments.insert({help->long_name, help});
        arguments.insert({help->short_name, help});
    }

    void Parser::parse(int argc, char* argv[])
    {
        //NOTE: the simpler regex "^(-|--)[a-zA-Z-]+$" causes the corner case
        // of '--' to be recognized as a valid commandline argument.
        const static std::regex arg_pattern(R"(^(-|--)[a-zA-Z]+[a-zA-Z-]*$)");

        for(int i=1; i < argc; ++i)
        {
            std::string str = argv[i];
            if (std::regex_match(str, arg_pattern) and arguments.find(str) != arguments.end())
            {
                arguments[str]->set = true;
            }
        }
    }

    const std::shared_ptr<CmdArg> Parser::get_arg(std::string argname) const
    {
        return arguments.at(argname);
    }

    void Parser::help() const
    {
        std::unordered_set<std::string> seen;
        for(const auto& [_, cmdarg] : arguments)
        {
            if (seen.find(cmdarg->identifier) == seen.end())
            {
                std::cout << cmdarg->summary() << '\n';
                seen.insert(cmdarg->identifier);
            }
        }
    }

    #ifdef CARP_DEBUG
        void Parser::print_all_arguments() const
        {
            std::cout << std::boolalpha;
            std::unordered_set<std::string> seen;
            for(const auto& [_, cmdarg] : arguments)
            {
                if (seen.find(cmdarg->identifier) == seen.end())
                {
                    std::cout << "Identifier: " << cmdarg->identifier << '\n'
                              << "Long name: " << cmdarg->long_name << '\n'
                              << "Short name: " << cmdarg->short_name << '\n'
                              << "Description: " << cmdarg->description << '\n'
                              << "Required?: " << cmdarg->enforced << '\n'
                              << "Set?: " << cmdarg->set << "\n\n";

                    seen.insert(cmdarg->identifier);
                }
            }
        }
    #endif
}

