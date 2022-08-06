#pragma once

#ifdef CARP_DEBUG

#include <iostream>
#include <iomanip>
#include <functional>
#include <chrono>
#include <type_traits>
#include <exception>
#include <cmath>
#include <limits>
#include <algorithm>

#define stringify(a) #a

template <typename T>
bool are_equal_vectors(std::vector<T> v1, std::vector<T> v2)
{
    return v1.size() == v2.size() and std::equal(v1.begin(), v1.end(), v2.begin(), [](T a, T b) {return a == b;});
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

template <typename Class, typename Function, typename... Args>
bool member_throws_exception(Class& object, const Function& member_func, Args&&... args)
{
    try
    {
        (object.*member_func)(std::forward<Args>(args)...);
    }
    catch(...)
    {
        return true;
    }

    return false;
}

template <typename Exception, typename Class, typename Function, typename... Args, 
typename = std::enable_if_t<std::is_base_of_v<std::exception, Exception>>>
bool member_throws_exception(Class& object, const Function& member_func, Args&&... args)
{
    try
    {
        (object.*member_func)(std::forward<Args>(args)...);
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

//The following code comes directly from the C++ documentation: https://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
template<class T,
typename std::enable_if_t<std::is_floating_point_v<T>>>
bool almost_equal(T a, T b)
{
    return std::fabs(a-b) <= std::numeric_limits<T>::epsilon() * std::fabs(a+b) 
    || std::fabs(a-b) < std::numeric_limits<T>::min();
}

#endif