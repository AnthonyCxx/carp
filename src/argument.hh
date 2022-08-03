#pragma once

#include <memory>
#include <vector>
#include <string>
#include <string_view>
#include <optional>
#include <charconv>
#include <functional>
#include <system_error>

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

            template <typename T>
            std::optional<T> try_parse_number(int radix = 10) const;

            std::optional<bool> try_parse_bool() const;

            template <typename R, typename... Args>
            std::optional<R> try_parse_user_defined(const std::function<bool(R,Args...)>&, Args&&... args) const;

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

    template <typename T>
    std::optional<T> CmdArg::try_parse_number(int radix) const
    {
        T value;
        std::from_chars_result parse_result = std::from_chars(values[0].data(), values[0].data() + values[0].size(), /*out*/ value, radix);

        if (parse_result.ec == std::errc{})
            return value;

        return std::nullopt;
    }

    std::optional<bool> CmdArg::try_parse_bool() const
    {
        const static auto case_insensitive_char_comp = [](unsigned char a, unsigned char b) -> bool {return tolower(a) == tolower(b);};
        const static auto case_insensitive_str_comp = [](std::string_view a, std::string_view b) -> bool 
        {
            return a.length() == b.length() and std::equal(a.begin(), a.end(), b.begin(), case_insensitive_char_comp);
        };

        if (case_insensitive_str_comp(values[0], "true"))
            return true;

        if (case_insensitive_str_comp(values[0], "false"))
            return false;

        return std::nullopt;
    }

    template <typename R, typename... Args>
    std::optional<R> CmdArg::try_parse_user_defined(const std::function<bool(R,Args...)>& parser_func, Args&&... args) const
    {
        try
        {
            R parsed_value;
            if (parser_func(/*out*/ parsed_value, std::forward<Args>(args)...))
                return parsed_value;
        }
        catch (...)
        {
            return std::nullopt;
        }
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
