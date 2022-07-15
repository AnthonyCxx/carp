#pragma once

#if CARP_DEBUG

#include <memory>
#include <cassert>

#include "../argument.hh"

class ArgumentTests
{
    public:
    static void test_default_constructor()
    {
        std::shared_ptr<carp::CmdArg> arg = carp::CmdArg("default")
                                                    .build();
        assert(arg->long_name == "--default");
        assert(arg->short_name == "-default");
        assert(arg->enforced == false);
        assert(arg->set == false);

        std::cout << __FUNCTION__ << " in " << __FILE__ << " passed successfully\n";
    }

    static void test_parameterized_constructor()
    {
        std::shared_ptr<carp::CmdArg> arg = carp::CmdArg("foo")
                                                    .abbreviation("f")
                                                    .required(true)
                                                    .help("this argument does stuff")
                                                    .build();

        assert(arg->identifier == "foo");
        assert(arg->long_name == "--foo");
        assert(arg->short_name == "-f");
        assert(arg->enforced == true);
        assert(arg->description == "this argument does stuff");

        std::cout << __FUNCTION__ << " in " << __FILE__ << " passed successfully\n";
    }

    static void driver() 
    {
        test_default_constructor();
        test_parameterized_constructor();
        std::cout << '\n';
    }
};

#endif