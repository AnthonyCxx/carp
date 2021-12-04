#pragma once

//Native C++ Libraries
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <regex>
#include <type_traits>

//Custom Header files
#include "argument.hpp"

namespace arg_parser
{
    //Class 'Parser' represents a cmdline argument parser and its data
    class Parser final
    {
        private:
            //A map of all the commandline arguments, mapped to their values
            std::unordered_map<std::string, Argument> arg_map;

        public:
            template <typename ...Args>
            Parser(Args&&...);

            void parse(int, char*[]);
            [[nodiscard]] const Argument& operator[](const std::string&) const;

            // *** TEMP: DEBUGGING *** //
            void print_map() const;
    };



    // ***** FUNCTION IMPLEMENTATION ***** //

    //Constructor
    template <typename ...Args>
    Parser::Parser(Args&&... args)
    {
        //Make sure all the arguments are 'Argument' objects (if using C++20, change this to a 'requires' clause)
        static_assert((std::is_same_v<Args, Argument> and ...), "whoops, not all the arguments were \'Argument\' objects :P");

        //Insert all the arguments into the map
        (arg_map.insert({args.name, std::move(args)}), ...);   //move arguments since they own resources
    }

    //Parse the commandline arguments (toggle proper values in the map)
    void Parser::parse(int argc, char* argv[])
    {
        //Regex for commandline arguments: matches with any alphabetic string that starts with '-' or '--' (also allows hyphens)
        const static std::regex arg_pattern(R"((-|--)[a-zA-Z\-]+)");
        std::string current_arg;

        for(int i=1; i < argc; ++i)   //safe to use 'int' over 'std::size_t' because max is INT_MAX
        {
            if (std::regex_match(argv[i], arg_pattern) and arg_map.find(argv[i]) != arg_map.end())  //second condition prevents injection of new args
            {
                current_arg = argv[i];                  //Set the current argument
                arg_map[current_arg].set = true;       //Mark the argument as seen
            }
            else  //add whatever follows to the list of current arguments
            {
                if (arg_map.find(current_arg) != arg_map.end())      //Protect against stray input (parameters without arguments)
                    arg_map[current_arg].params.push_back(argv[i]);
            }
        }
    }


    //Return an argument object
    [[nodiscard]] const Argument& Parser::operator[](const std::string& arg) const
    {
        //Return the Argument object associated with the string -- CAN THROW std::out_of_range
        return arg_map.at(arg);
    }


    void Parser::print_map() const
    {
        for(const auto& map_entry : arg_map)
        {
            std::cout << map_entry.first << " -> ";

            //Print all the parameters
            for(const std::string_view param : map_entry.second.params)
            {
                std::cout << param << " ";
            }
            std::cout << '\n';
        }
    }
}
