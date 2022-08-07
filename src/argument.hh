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

            template <typename T,
            typename = std::enable_if_t<std::is_integral_v<T>>>
            std::optional<T> try_parse_integer(int radix = 10) const;

            template <typename T, 
            typename = std::enable_if_t<std::is_floating_point_v<T>>>
            std::optional<T> try_parse_floating_point() const;

            std::optional<bool> try_parse_bool() const;

            template <typename R, typename ...Args>
            std::optional<R> try_parse_user_defined(const std::function<bool(const std::vector<std::string>&,R&)>&, Args&&...) const;

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

            ArgAction on_parse;
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
        on_parse = ArgAction::SetTrue;
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
        on_parse = action;
        return *this;
    }

    std::shared_ptr<CmdArg> CmdArg::build()
    {
        return std::make_shared<CmdArg>(*this);
    }

    template <typename T,
    typename = std::enable_if_t<std::is_integral_v<T>>>
    std::optional<T> CmdArg::try_parse_integer(int radix) const
    {
        T value;
        std::from_chars_result parse_result = std::from_chars(values[0].data(), values[0].data() + values[0].size(), /*out*/ value, radix);

        if (parse_result.ec == std::errc{})
            return value;

        return std::nullopt;
    }

    /*
        Although std::from_chars does have an overload that accepts floating-point numbers, GCC libstdc++ does not
        yet support it (see P0067R5, "Elementary String Conversions*", at https://en.cppreference.com/w/cpp/compiler_support/17).
        When GCC gets support for floating-point std::from_chars, 'try_parse_integer' and 'try_parse_floating_point' should
        be merged into two different overloads of a function 'try_parse_number', which would use the different parameters
        (the int radix parameter for integers and std::chars_format for floating-point) to differentiate between overloads.

        Original paper: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0067r5.html
    */
    template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
    std::optional<T> CmdArg::try_parse_floating_point() const
    {
        //As defined by the standard, std::is_floating_point<T>::value only returns true for
        //float, double, and long double

        try
        {
            if constexpr (std::is_same_v<T, float>)
            {
                return stof(values[0]);
            }
            else if (std::is_same_v<T, double>)
            {
                return stod(values[0]);
            }
            else
            {
                return stold(values[0]);
            }
        }
        catch(...)
        {
            return std::nullopt;
        }
    }

    std::optional<bool> CmdArg::try_parse_bool() const
    {
        //Algorithm from https://learning.oreilly.com/library/view/c-cookbook/0596007612/ch04s14.html#cplusplusckbk-CHP-4-SECT-13.3
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

    /*
        This callback function allows users to parse a CmdArg's values as any struct, class, enum, etc using 
        their own function. The function provided must be of the same format as std::from_chars, i.e.:
        1. The function must return bool (true if parsing succeeded, false is failed)
        2. The second parameter should be of type `const std::vector<std::string>&`, which will give the parsing function
            the user passes in access to the CmdArg's `values` member
        3. The second parameter to the function must be an output parameter, which will contain the parsed value
           if the parsing succeeds. Accessing the output parameter when parsing fails is undefined behavior.

        IMP: return type deduction often fails; always explicitly specify the return type with `try_parse_user_defined<foo>`

        C++ out parameters (subsection 'Out-parameters'): http://www.cs.ecu.edu/karl/2530/spr18/Notes/lec21A.html#:~:text=An%20out%2Dparameter%20represents%20information,parameters%20and%20two%20out%2Dparameters.
    */

    template <typename R, typename ...Args>
    std::optional<R> CmdArg::try_parse_user_defined(const std::function<bool(const std::vector<std::string>&,R&)>& parser_func, Args&&... args) const
    {
        try
        {
            R parsed_value;
            if (parser_func(values, /*out*/ parsed_value, std::forward<Args>(args)...))
                return parsed_value;
        }
        catch (...)
        {
            return std::nullopt;
        }

        return std::nullopt;
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
