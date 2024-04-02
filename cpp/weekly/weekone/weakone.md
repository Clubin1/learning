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
```
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

```
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