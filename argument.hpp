#pragma once

//Native C++ Libraries
#include <string_view>
#include <vector>

//Class 'Argument' represents a commandline argument
class Argument final
{
    bool required;
}


//
struct Properties final 
{
    //Data members
    std::vector<std::string_view> params;      //List of the parameters (view of the strings in char* argv[])
    bool set;                                 //Whether or not the argument was used in the cmd args

    //Constructor
    ArgumentProperties()
    {
        set = false;
    }
};
