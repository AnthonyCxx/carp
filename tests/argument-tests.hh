#pragma once

#ifdef CARP_DEBUG

#include <memory>
#include <cassert>

#include "test-utils.hh"
#include "../src/argument.hh"

namespace tests 
{
    class ArgumentTests
    {
        public:
        static void default_constructor()
        {
            std::shared_ptr<carp::CmdArg> arg = carp::CmdArg("default")
                                                        .build();
            assert(arg->identifier == "default");
            assert(arg->long_name == "--default");
            assert(arg->short_name == "-default");
            assert(arg->enforced == false);
            assert(arg->set == false);
            assert(arg->on_parse = carp::ArgAction::SetTrue);
            assert(arg->values.size() == 1);
            assert(arg->count == 0);
        }

        static void parameterized_constructor()
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
        }

        static void driver() 
        {
            test(__FILE__, stringify(default_constructor), default_constructor);
            test(__FILE__, stringify(parameterized_constructor), parameterized_constructor);
            std::cout << '\n';
        }
    };
}
#endif