#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <string_view>
#include <memory>
#include <unordered_map>
#include <regex>
#include <type_traits>
#include <cstdlib>
#include <stdexcept>

#include "argument.hh"

namespace carp
{
    class Parser final
    {
        public:
            template <typename ...Args>
            Parser(Args...);

            void parse(int, char*[]);
            void validate_required_args() const;
            const std::shared_ptr<CmdArg> get_arg(std::string) const;  //TODO: change to 'std::shared_ptr<const CmdArg>' (const in wrong spot)
            void help() const;

            #ifdef CARP_DEBUG
            void print_all_arguments() const;
            #endif

        private:
            std::unordered_map<std::string, std::shared_ptr<CmdArg>> arguments;
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
        std::shared_ptr<CmdArg> arg = nullptr;  

        for(int i=1; i < argc; ++i)
        {
            std::string cmdarg = argv[i];

            if (std::regex_match(cmdarg, arg_pattern) and arguments.find(cmdarg) != arguments.end())
            {
                arg = arguments.at(cmdarg);
                arg->set = true;

                switch (arg->on_parse)
                {
                    case ArgAction::SetTrue:
                        arg->values[0] = "true";
                        break;

                    case ArgAction::SetFalse:
                        arg->values[0] = "false";
                        break;

                    case ArgAction::Count:
                        arg->count++;
                        break;
                }
            }
            else if (arg != nullptr)
            {
                switch (arg->on_parse)
                {
                    case ArgAction::StoreSingle:
                        arg->values[0] = cmdarg;
                        break;

                    case ArgAction::StoreMany:
                        if (arg->values.size() > 0 and not arg->values[0].empty()) //TODO: refactor this mess
                        {
                            arg->values.push_back(cmdarg);
                        }
                        else
                        {
                            arg->values[0] = cmdarg;
                        }
                        break;
                }
            }
        }

        validate_required_args();
    }

    void Parser::validate_required_args() const
    {
        std::string argument_errors;

        for(const auto& [identifier, cmdarg] : arguments)
        {
            if (identifier[0] != '-')
            {
                if (cmdarg->enforced and not cmdarg->set)
                {
                    if (not argument_errors.empty())
                        argument_errors += ", ";

                    argument_errors += cmdarg->long_name + " (" + cmdarg->short_name + ")";
                }
            }
        }

        if (not argument_errors.empty())
        {
            throw std::runtime_error("the following required arguments were not provided: " + argument_errors);
        }
    }

    const std::shared_ptr<CmdArg> Parser::get_arg(std::string argname) const
    {
        return arguments.at(argname);
    }

    void Parser::help() const
    {

        for(const auto& [name, cmdarg] : arguments)
        {
            if (name[0] != '-') //if a raw identifier, not a long/short_name
            {
                std::cout << cmdarg->summary() << '\n';
            }
        }
        exit(1);
    }

    #ifdef CARP_DEBUG
        void Parser::print_all_arguments() const
        {
            std::cout << std::boolalpha;
            for(const auto& [name, cmdarg] : arguments)
            {
                if (name[0] != '-') //if a raw identifier, not a long/short_name
                {
                    std::cout << "Identifier: " << cmdarg->identifier << '\n'
                              << "Long name: " << cmdarg->long_name << '\n'
                              << "Short name: " << cmdarg->short_name << '\n'
                              << "Description: " << cmdarg->description << '\n'
                              << "Required?: " << cmdarg->enforced << '\n'
                              << "Set?: " << cmdarg->set << "\n\n";
                }
            }
        }
    #endif
}

