#pragma once

//Native C++ Libraries
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>

//Custom Header files
#include "argument.hpp"

class Parser
{
    private:
        //A map of all the commandline arguments, mapped to their values
        std::unordered_map<std::string, Argument> arg_map;

    public:
        template <typename ...Args>
        Argument(Args&&...)
        

        void parse(int, char*[]);
        [[nodiscard]] const Argument& operator[](std::string) const;

        // *** TEMP: DEBUGGING *** //
        void printMap() const;
};



// ***** FUNCTION IMPLEMENTATION ***** //

//Constructor
template <typename ...Args>
Argument::Argument(Args&&... args)
{
    
}


//Parse the commandline arguments (toggle proper values in the map)
void Parser::parse(int argc, char* argv[])
{
    //for(std::size_t i=0; i < argc; i;
}


//Return an argument object
const Argument& Parser::operator[](const std::string& arg) const
{
    //Return the Argument object associated with the string -- CAN THROW std::out_of_range
    return arg_map.at(arg);
}


void Parser::printMap() const
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
