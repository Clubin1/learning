## Objects
objects contain data on
- size via sizeof
- alignment requirement via alignof
- storage duration (auto, static, dynamic)
- lifetime
- type
- value
  
## Scope
the area that a decleration is valid. Inside of a scope a name look up is preformed for declerations

Each program has a global scope, other scopes are created by

- a decleration
- a parameter in parameter list
- a statement
- exception handler
  
```
//                global  scope  scope
//                scope     S      T
int x;         //   ─┐                 // program point X
               //    │
{              //    │     ─┐
    {          //    │      │     ─┐
        int y; //    │      │      │   // program point Y
    }          //    │      │     ─┘
}              //   ─┘     ─┘
```

block scopes are basically scopes created by things like
- if statements
- iteration
- exception handlers
- compound statements

```
int i = 42;
int a[10];
 
for (int i = 0; i < 10; i++) // inner “i” inhabits the block scope
    a[i] = i;                // introduced by the for-statement
 
int j = i; // j = 42
```

namespace scopes
```
namespace V   // the namespace definition of “V”
{             // introduces a namespace scope “S”
    // the first part of scope “S” begins here
    void f();
    // the first part of scope “S” ends here
}
 
void V::f()   // the portion after “f” is also a part of scope “S”
{
    void h(); // declares V::h
}   
```
class scopes
```
class C       // the class definition of “C”
{             // introduces a class scope “S”
    // the first part of scope “S” begins here
    void f();
    // the first part of scope “S” ends here
}
 
void C::f()   // the portion after “f” is also a part of scope “S”
{
    /* ... */
}  
```
