#pragma once

#ifdef CARP_DEBUG

#include <iostream>
#include <iomanip>
#include <functional>
#include <chrono>

#define stringify(x) #x

template <typename Function, typename... Args>
void test(const char* file, const char* function_name, Function&& func, Args&&... args)
{
    using fpms = std::chrono::duration<double, std::milli>;
    std::cout << std::fixed << std::setprecision(4);    

    std::cout << '[' << file << "] " << function_name << "...";
    auto start = std::chrono::high_resolution_clock::now();

    std::invoke(std::forward<Function>(func), std::forward<Args>(args)...);

    double elapsed_time = std::chrono::duration_cast<fpms>(std::chrono::high_resolution_clock::now() - start).count();
    std::cout << "SUCCESS (" << elapsed_time << "ms)\n";
}

#endif