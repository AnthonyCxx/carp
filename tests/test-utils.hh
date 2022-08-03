#pragma once

#ifdef CARP_DEBUG

#include <iostream>
#include <iomanip>
#include <functional>
#include <chrono>
#include <type_traits>
#include <exception>

#define stringify(x) #x

template <typename T>
bool are_equal_vectors(std::vector<T> v1, std::vector<T> v2)
{
    if (v1.size() != v2.size())
        return false;

    for(int i=0; i < v1.size(); ++i)
    {
        if (v1[i] != v2[i])
            return false;
    }

    return true;
}

template <typename Function, typename... Args>
void test(const char* file, const char* function_name, const Function& func, Args&&... args)
{
    using fpms = std::chrono::duration<double, std::milli>;
    std::cout << std::fixed << std::setprecision(4);    

    std::cout << '[' << file << "] " << function_name << "...";
    auto start = std::chrono::high_resolution_clock::now();

    func(std::forward<Args>(args)...);

    double elapsed_time = std::chrono::duration_cast<fpms>(std::chrono::high_resolution_clock::now() - start).count();
    std::cout << "SUCCESS (" << elapsed_time << "ms)\n";
}

template <typename Function, typename... Args>
bool throws_exception(const Function& func, Args&&... args)
{
    try
    {
        func(std::forward<Args>(args)...);
    }
    catch(...)
    {
        return true;
    }

    return false;
}

template <typename Function, typename... Args, typename Exception, 
typename = std::enable_if_t<std::is_base_of_v<std::exception, Exception>>>
bool throws_exception(const Function& func, Args&&... args)
{
    try
    {
        func(std::forward<Args>(args)...);
    }
    catch(Exception)
    {
        return true;
    }
    catch(...)
    {
        return false;
    }

    return false;
}

#endif