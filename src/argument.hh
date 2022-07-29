#pragma once

#include <memory>
#include <vector>
#include <string>
#include <string_view>

#ifdef CARP_DEBUG
namespace tests
{
    class ParserTests;
    class ArgumentTests;
}
#endif

namespace carp
{
    enum class ArgAction
    {
        StoreSingle,
        StoreMany,
        SetTrue,
        SetFalse,
        Count
    };

    class CmdArg
    {
        public:
            CmdArg(std::string);

            CmdArg& name(std::string);
            CmdArg& abbreviation(std::string);
            CmdArg& help(std::string);
            CmdArg& required(bool);
            CmdArg& action(ArgAction);
            std::shared_ptr<CmdArg> build();
        
            bool is_set() const;
            std::string summary() const;

            friend class Parser;

            #ifdef CARP_DEBUG
            friend class tests::ParserTests;
            friend class tests::ArgumentTests;
            #endif

        private:
            std::string identifier;
            std::string long_name;
            std::string short_name;
            std::string description;
            bool enforced;
            bool set;

            ArgAction on_encounter;
            std::vector<std::string> values;
            unsigned int count;
    };

    CmdArg::CmdArg(std::string id = "")
    {
        identifier = id;
        long_name = "--" + id;
        short_name = "-" + id;
        enforced = false;
        set = false;
        on_encounter = ArgAction::SetTrue;
        values = std::vector<std::string>(1);
        count = 0;
    }

    CmdArg& CmdArg::name(std::string name)
    {
        long_name = "--" + name;
        return *this;
    }
    
    CmdArg& CmdArg::abbreviation(std::string abbreviation)
    {
        short_name = "-" + abbreviation;
        return *this;
    }

    CmdArg& CmdArg::help(std::string help)
    {
        description = help;
        return *this;
    }

    CmdArg& CmdArg::required(bool required)
    {
        enforced = required;
        return *this;
    }

    CmdArg& CmdArg::action(ArgAction action)
    {
        on_encounter = action;
        return *this;
    }

    std::shared_ptr<CmdArg> CmdArg::build()
    {
        return std::make_shared<CmdArg>(*this);
    }

    bool CmdArg::is_set() const
    {
        return set;
    }

    std::string CmdArg::summary() const 
    {  
        //[Required] foo (--foo, -f):       foo is a placeholder argument
        std::string prefix = enforced ? "[Required] " : "[Optional] ";
        return prefix + identifier + " (" + long_name + ", " + short_name + "): " + "\t" + description;
    }
}
