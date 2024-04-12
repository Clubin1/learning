Week 2 4/8

OS: 
https://pages.cs.wisc.edu/~remzi/OSTEP/
Ch 5 - 7 (~40 pages)

C++ Weekly: 11-20 (not all) (~3hrs)

Features:
Ep 12 C++17 std::any 20M
Ep 14 std library gems 10M
Ep 15 std::bind 10M
Ep 16 Avoiding std::bind 10M
Ep 17 C++17 std::invoke 10M
Ep 18 C++17 constexpr if + special edition ep 15M
Ep 20 C++ 17 fold expressions 10M
Ep 21 C++17 if and switch init statements 10M
Ep 22 C++17 Nested Namespaces 5M
Ep 23 C++17 __has_include 5M
Ep 24 C++17 Structured bindings 5M
Ep 25 C++17 Aggregate Inits 5M
Ep 26 Language Features Removed in C++17 10M
Ep 27 Sequence Containers 10M
Ep 28 C++17 [[fallthrough]] Attribute 5M
Ep 29 C++17 [[maybe_unused]] Attribute 5M 
Ep 30 [[nodiscard attribute]] 5MEp 12 C++17 std::any 20M


## std::any
an object that holds any type

if an object is no throw move constructible and is 16 bytes or less, the std::any object does not have to pay dynamic allocation costs. 

This is good to avoid since dynamic allocation involves overhead by requesting memory from the heap and other book keeping costs. 
```c++
#include <expiremental/any>
#include <vector>
#include <string>
#include <iostream>
#include <type_traits>
struct T {
    S(const S &s) = default;
    s() = default;
}

int main() {
    std::vector<std::expiremental::fundamentals_v1::any> v{5,3.4, std::string("Hello World"), s()};

    static assert(std::is_nothrow_move_constructible<S>::value, "no throew");
}
```
an example of ensuring that something is no throw move constructible and std any
```c++
#include <any>
#include <iostream>
#include <string>

int main() {
    std::any value = std::string("Hello, world!");

    try {
        std::string& str = std::any_cast<std::string&>(value);
        std::cout << str << std::endl;  // Output: Hello, world!

        int& num = std::any_cast<int&>(value);  // Throws std::bad_any_cast
    } catch (const std::bad_any_cast& e) {
        std::cout << "Bad any_cast: " << e.what() << std::endl;
    }

    return 0;
}
```
std any and anycast.
anycast must be used if reading a std::any value. The type has to be known