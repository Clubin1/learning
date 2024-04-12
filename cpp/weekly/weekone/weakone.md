Week 1 4/1

OS: 
https://pages.cs.wisc.edu/~remzi/OSTEP/
Ch 1 - 4 (~40 pages) Half is just intro

C++ Weekly: 1-10 (not all) (~3hrs)

Variadic Templates:
Ep 6 Intro to Variadic Templates 20M
Ep 4 Variadic Template Refactor 30M
Ep 10 Variadic Expansion Wrap up 15M

Specifics:
Ep 2 Cost of using statics 20M
Ep 7 Stop using std::endl 10M
Ep 8 C++ name demangling 15M

std::future:
Ep 9 part 1 std::future 15M
Ep 11 part 2 15M

## Variadic Templates 
**part one**
```c++
#include <iostream> 
#include <sstream>
#include <vector>

template<typename T>
std::string to_string_impl(const T& t) 
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

template<typename ... Param>
std::vector<std::string> to_string(const Param& ... param)
{
    return {to_string_impl(param)...};
}
int main() 
{
    const auto vec = to_string("hello", 1, 5.3);
    for(const auto &o : vec) {
        std::cout << o << '\n';
    }
    return 0;
}
```
Variadic templates are ways to handle writting generic code for multiple parameters. An example of usage for one is writting a type safe print function. Variadic templates can be done with paremeter pack expansion or recursively. 

Variadic templates are defined by using an elipsis in the function signature and template decleration. The code above uses parameter pack expansion to wring a function that converts any type to a string on the entire list of parameters and ouputs to a vector.

The same code can be done recursively by creating a base case and modifying the function to appending to the list recursively. A recursive approach does increase overhead. 

```c++
#include <iostream> 
#include <sstream>
#include <vector>

template<typename T>
std::string to_string_impl(const T& t) 
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

// no parameters to pass, do nothing
std::vector<std::string> to_string()
{
    return {};
}
template<typename ... Param>
std::vector<std::string> to_string(const P1 &p1, const Param& ... param)
{
    std::vector<std::string> s;
    s.pushback(to_string_impl(p1));
    
    const auto remainder = to_string(param...);
    s.insert(s.end(), remainder.begin(), remainder.end());
    return s;
}
int main() 
{
    const auto vec = to_string("hello", 1, 5.3);
    for(const auto &o : vec) {
        std::cout << o << '\n';
    }
    return 0;
}
```

**Variadic template refactoring**

The existing code in the codebase here had multiple different templates to handle all of the different types and testing functions. Variadic templates are used here toa void having duplicate templating code. 

key notes
- Variadic templates can be recursive by nature. When working with overloaded functions the instructor opted to simply code out the first case so the compiler doesn't have to guess which overloaded function to call.
- In the video a `std::initializer_list<>` is used with pack expansion to preform an action and handle all types in order. It can be used in other cases to avoid recursive intialization of templating functions. Some compilers might complain about the return type, `void()<std::initializer_list>` can be used.

**Variadic template review**

key notes
- different compilers evaluate the order of functionc alls differently. This is wht its important to do `void()<std::initializer_list><int>`. Braces are also needed. For example `void()<std::initializer_list><int>{recursive_template(t), 0)...};`

## cost of using statics
```c++
struct C {
    static const std::string &magic_static()
    {
        static const std::string s = "bob";
        return s;
    }
    const std::string &s = magic_static();

    const std::string &magic_static_ref()
    {
        return s;
    }
}
```

```c++
auto main() -> int
{
    C::magic_static().size();
    C::magic_static().size();
    C::magic_static().size();
    return C::magic_static().size();
}
```

```c++
auto main() -> int
{
    C c;
    c.magic_static_ref().size();
    c.magic_static_ref().size();
    c.magic_static_ref().size();
    return c.magic_static_ref().size();
}
```
The class C also has a member variable s which is assigned a const reference to the value returned by the magic_static() function during object construction.

The magic_static_ref() function returns a reference to the member variable s, not the static variable inside magic_static().

In the first main() function, the compiler has to make a call to the size() function every time magic_static() is called. There is a thread-safe comparison (cmpb with a guard variable) to check if the static variable has been initialized, which happens for each function call.

In the second main() function, the compiler eliminates all calls to magic_static_ref().size() because it knows there are no side effects to those calls. The cached reference to the variable s is used, avoiding the overhead of accessing the static variable created by magic_static().

## Std Future
```c++
#include <random>
#include <set>
#include <iostream>
#include <algorithm>
#include <future>

std::set<int> make_sorted_random(const size_t num_elems) {
    std::set<int> retval;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, num_elems - 1);
    std::generate_n(std::inserter(retval, retval.end()), num_elems, [&](){ return dis(gen); });
    return retval;
}

int main() {
    auto f1 = std::async(std::launch::async, make_sorted_random, 100000);
    auto f2 = std::async(std::launch::async, make_sorted_random, 100000);

    std::cout << f1.get().size() << ' ' << f2.get().size() << '\n';
}
```
used to preform things in parallelism. `std::async` takes a function and its parameters and runs it asyncronously. `std::async` also can take in `std::launch::async` which is a launch policy that gurantees the function will run async on another thread.

`std::async` returns a callable object of `std::future`

`std::future.get()` is a blocking call so in order to ensure async add async policy.

`std::launch:deferred` is another launch policy that uses lazy evaluation.

## std::endl vs '\n'
main takeaways
- std::endl preforms a flush after every call. If you don't need a flush use \n as it improves performance. 
- If a flush was needed a manual flush at the end is also a possible approach.
