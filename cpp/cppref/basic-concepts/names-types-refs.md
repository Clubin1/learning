## Basic Concepts
C++ code are essentially text files, header and source, that contain declerations and undergo translation to become an executable program. 

Keywords are special words in C++ that have a special meaning

Entities consist of,
-  values
-  objects
-  references
-  structured bindings
-  functions
-  enumerators
-  types
-  class members
-  templates
-  template specializations
-  parameter packs
-  name spaces

Declerations introduce entities, assign them values and define their properties

Declared objects and declared references that are non-static data members are variables

## Names
names are used to describe the following
- identifier
- an overloaded operator name in function notation
- user defined literal operator name
- template name ex `MyTemplate<>`
  
Names are introduced into a program by decleration. A name might also have linkage.

Linkage is when a name denotes another value that is already declared somewhere else in the scope

When the compiler encounters an unknown name, it makes an association by name lookup. This does not happen for dependent names from template declerations. 

## Name Lookup
when a name is encountered an associtation is made with the decleration that introduced it.

for the code `std::cout << std::endl;`
A name lookup for std is preformed where it finds the decleration of `namespace std` ion the header `<iostream>`

Again a lookup is ran on the name `cout` which finds a variable in the namespace `std` similarly `endl` is found as a function template decleration. 

## Types

Program defined types
- program defined types include non closure class types, enumeration types, lambda expressions

Type naming
- types can be named using a class, enum, typpedef or type alias
- For types with no name type id syntax is used

Static and dynamic types
- static type of an expression is determined at compile time
- the dynamic type of a gvalue expression referring to a polymorphic object is the type of the most derived object

incomplete types
- includes voids, forward declared
   
## Refrences
 Reference collapsing

It is permitted to form references to references through type manipulations in templates or typedefs, in which case the reference collapsing rules apply: rvalue reference to rvalue reference collapses to rvalue reference, all other combinations form lvalue reference:

```
typedef int&  lref;
typedef int&& rref;
int n;
 
lref&  r1 = n; // type of r1 is int&
lref&& r2 = n; // type of r2 is int&
rref&  r3 = n; // type of r3 is int&
rref&& r4 = 1; // type of r4 is int&&
```

**Lvalue references**
an alias to an existing object
```
std::string test = "Hello";
std::string& test_ref = test;
test_ref += "world";
std::cout << test << endl;
```

lvalue in a function, Note c++ compiler turns the string passed in and uses a references due to the function param
```
#include <iostream>
#include <string>
 
void double_string(std::string& s)
{
    s += s; // 's' is the same object as main()'s 'str'
}
 
int main()
{
    std::string str = "Test";
    double_string(str);
    std::cout << str << '\n';
}
```

rvalues are temporary values that dont have a specific memory address. They are usually things like literals or temp objects returned from functions or expressions.
```
int x = 10; // 10 is an rvalue
```
rvalue references are used in c++ or later. They are denoted by `&&` and are a way to extend lifetime using move semantivs to avoid uneeded copying of objects. Rvalue refs allow the binding to rvalues and move their contents
```
#include <iostream>
#include <string>

void printString(std::string& str) {
    std::cout << "Lvalue reference: " << str << std::endl;
}

void printString(std::string&& str) {
    std::cout << "Rvalue reference: " << str << std::endl;
}

std::string getName() {
    return "John";
}

int main() {
    std::string name = "Alice";

    printString(name);                  // Calls printString(std::string&)
    printString(std::move(name));       // Calls printString(std::string&&)
    printString(getName());             // Calls printString(std::string&&)
    printString("Hello");               // Calls printString(std::string&&)

    return 0;
```
This code has two overloaded functions of printString. Different versions are called based on the parameter. The rvalue reference function improves the efficiency of the program by not copying the resulting string. 

**Forward references**
in c++11 they are a ref type that preserves the value category (lvalue/rvalue) of a function arguement allowing it to be forwarded to another function with `std::forward`
```
template<class T>
int f(T&& x)                      // x is a forwarding reference
{
    return g(std::forward<T>(x)); // and so can be forwarded
}
```
forward refs are used a lot in generic programming like templates to ensure something like an rvalue ref isnt lost

## Values
glvalue: is an expression whos evaluation determines the identity of an object or function
prvalue: expression whose evaluation
    - compoutes the val of an operand of a built in operator
    - initializes an object
xvalue is a glvalue that denotes an object whose resources can be reused
## Templates
templates allow generic code to work with different data types. They allow function or classes to work with different types. 

Actual type is determined at compile time

Templates use placeholder types known as template parameters that are replaced when the template is instantiated

```
#include <iostream>
#include <vector>

template<typename T>
void addToVector(std::vector<T>& vec, T&& value) {
    vec.push_back(std::forward<T>(value));
}

int main() {
    std::vector<std::string> strings;
    std::string str = "Hello";

    addToVector(strings, str);                    // Lvalue, copies the string
    addToVector(strings, std::move(str));         // Rvalue, moves the string
    addToVector(strings, "World");                // Rvalue, moves the temporary string

    std::cout << "Strings: ";
    for (const auto& s : strings) {
        std::cout << s << " ";
    }
    std::cout << std::endl;

    return 0;
```
this is an example of using forward refs and templates to have a generic function that adds to a vector without copying the value that is passed in.