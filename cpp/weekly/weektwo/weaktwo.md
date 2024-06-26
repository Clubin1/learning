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
an object that holds any type in c++17

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

## std::next std::exchange
std::next takes iterator and increments it forward the most optimal way possible. Works for both lists and vectors.

if using something like

```C++
if(std::is_sorted(std::begin(v), std::end(v));
```

however we want to exclude n items

```C++
if(std::is_sorted(std::begin(v) + n, std::end(v));
```
works if v is a vector however not if v is a list.
```C++
if(std::is_sorted(std::next(std::begin(v)), std::end(v));
```
can be used as a way to increment the iterator without making an assumption the iterator can support integer operations

**std::exchange**
replaces the old value with a new value and the new value with the old value. It avoids copying of data.
```c++
    int main() {
        for (int i = 0; i < 1000; i++) {
            last = std::exchange(i, new_value);
        }
    }
```
```c++
#include <iostream>
#include <vector>
#include <utility>

void cyclicSort(std::vector<int>& nums) {
    int n = nums.size();
    for (int i = 0; i < n; ++i) {
        int j = i;
        while (nums[j] != i + 1) {
            int correctPos = nums[j] - 1;
            std::exchange(nums[j], nums[correctPos]);
        }
    }
}

int main() {
    std::vector<int> nums = {3, 1, 5, 4, 2};
    cyclicSort(nums);
    for (int num : nums) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    return 0;
}
```
example of std::exchange in a cyclic sort algorithim

## std::bind 
can bind parameters from anything callable into said object. If we have a function that prints 2 values we can craete a bind object of the print function with example parameters.
```c++
auto f = std::bind(&print, arg1, arg2);
f();
f();
```
std::bind is not optimal to use as it has a lot of overhead and the same tasks can be completed using lambdas
```c++
const auto f = [](auto &&arg1, auto &&arg2) {
    print(arg1, arg2);
};
f("s", 2);
```
there is a heavy performance difference using std::bind over lambdas.

## std::invoke()
a universal interface to access callable objects and invoke them
```c++
#include <iostream>
#include <functional>

void printValue(int value) {
    std::cout << "Value: " << value << std::endl;
}

struct Functor {
    void operator()(const std::string& str) {
        std::cout << "String: " << str << std::endl;
    }
};

int main() {
    // Invoking a free function
    std::invoke(printValue, 42);

    // Invoking a lambda function
    auto lambda = [](double d) {
        std::cout << "Double: " << d << std::endl;
    };
    std::invoke(lambda, 3.14);

    // Invoking a function object (functor)
    Functor functor;
    std::invoke(functor, "Hello, world!");

    // Invoking a member function
    std::string str = "Hello";
    std::invoke(&std::string::size, str);

    // Invoking a member object pointer
    int x = 10;
    int* ptr = &x;
    std::invoke(ptr);

    return 0;
}
```
use cases are for writting generic code where we can avoid having to write specific function calls for different types and abstracts it away. 

## constexpr if 
essentially a way to preform compile-time branching. A common use case is for checking things like typing at compile time

```c++
#include <type_traits>
#include <limits>
#include <iostream>

template<typename T>
constexpr bool is_both() {
    if constexpr(std::is_integral<T>::value && !std::is_same<bool, T>::value) {
        if constexpr(std::numeric_limits<T>::max() < 1000) {
            return true
        }
    }
    return false;
}
template<typename T>
auto print_type_info(const T&t) {
    if constexpr(is_both<T>()) {
        return t + 1;
    } else if constexpr(std::is_floating_point<T>::value) {
        return t + 0.1;
    } else {
        return t;
    }
}
int main () {
    print_type_info(1);
    print_type_info(100000);
}
```
with constexpr if the main thing to remember is how branches are removed out from the compiler. Some behavior and assumptions are made at compile time that require extra handling then if it wasnt at compile time. 

In this example, to add 1 to T T has to be a integral, be the same as bool and also be < 1000. The nested if branch is needed so the compiler knows that if it passes the first branch it has access to `std::numeric_limits`. 

