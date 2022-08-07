#pragma once

#include <iostream>
#include <string>
#include <optional>

/*
    <program_name> <version> [by <author>] [, <license> license]
    <description>
*/

namespace carp
{
    struct ProgramInfo
    {
        std::optional<std::string> author;
        std::optional<std::string> version;
        std::optional<std::string> program_name;
        std::optional<std::string> description;
        std::optional<std::string> license;

        ProgramInfo(std::optional<std::string>, std::optional<std::string>, std::optional<std::string>, std::optional<std::string>, std::optional<std::string>);

        void details() const;
    };

    ProgramInfo::ProgramInfo(std::optional<std::string> _author = std::nullopt, std::optional<std::string> _version = std::nullopt, std::optional<std::string> _program_name = std::nullopt, std::optional<std::string> _description = std::nullopt, std::optional<std::string> _license = std::nullopt)
    {
        author = _author;
        version = _version;
        program_name = _program_name;
        description = _description;
        license = _license;
    }

    void ProgramInfo::details() const
    {
        if (program_name) std::cout << program_name.value() << ' ';
        if (version) std::cout << version.value() << ' ';

        if (author) std::cout << "by " << author.value();
        if (license) std::cout << ", " << license.value() << " license";

        if (description) std::cout << '\n' << description.value() << "\n\n";
    }
}