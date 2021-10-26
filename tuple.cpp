// CODE FROM https://github.com/eliben/code-for-blog/blob/master/2014/variadic-tuple.cpp

// To compile with dumping record layout:
// $ clang++ -o variadic-tuple -Xclang -fdump-record-layouts variadic-tuple.cpp
//           -Wall -g -std=c++11
#include <cstdint>
#include <iostream>
#include <string>
#include <typeinfo>
#include <cstddef>

template <class... Ts> struct tuple {};

template <class T, class... Ts>
struct tuple<T, Ts...> : tuple<Ts...> {
  tuple(T t, Ts... ts) : tuple<Ts...>(ts...), tail(t) {}

  T tail;
};

template <std::size_t, class> struct elem_type_holder;

template <class T, class... Ts>
struct elem_type_holder<0, tuple<T, Ts...>> {
  typedef T type;
};

template <std::size_t k, class T, class... Ts>
struct elem_type_holder<k, tuple<T, Ts...>> {
  typedef typename elem_type_holder<k - 1, tuple<Ts...>>::type type;
};

template <std::size_t k, class... Ts>
typename std::enable_if<
    k == 0, typename elem_type_holder<0, tuple<Ts...>>::type&>::type
get(tuple<Ts...>& t) {
  return t.tail;
}

template <std::size_t k, class T, class... Ts>
typename std::enable_if<
    k != 0, typename elem_type_holder<k, tuple<T, Ts...>>::type&>::type
get(tuple<T, Ts...>& t) {
  tuple<Ts...>& base = t;
  return get<k - 1>(base);
}

int main(int argc, char** argv) {
  tuple<double, uint64_t, const char*> t1(12.2, 42, "big");

  std::cout << "0th elem is " << get<0>(t1) << "\n";
  std::cout << "1th elem is " << get<1>(t1) << "\n";
  std::cout << "2th elem is " << get<2>(t1) << "\n";

  get<1>(t1) = 103;

  std::cout << "1th elem is " << get<1>(t1) << "\n";

  typename elem_type_holder<1, tuple<double, int, const char*>>::type foo;
  std::cout << typeid(foo).name() << "\n";
  return 0;
}
