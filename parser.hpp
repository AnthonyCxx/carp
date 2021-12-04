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
#include "properties.hpp"

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
    static_assert((std::is_same_v<Args&&, Argument> && ...), "whoops, not all the arguments were \'Argument\' options");

    //Insert all the arguments into the map
    (arg_map.insert({args.name, args}), ...);
}

//Parse the commandline arguments (toggle proper values in the map)
void Parser::parse(int argc, char* argv[])
{
    //Regex for commandline arguments: matches with any alphabetic string that starts with '-' or '--' (also allows -'s)
    const static std::regex arg_pattern(R"((-|--)[a-zA-Z\-]+)");
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
