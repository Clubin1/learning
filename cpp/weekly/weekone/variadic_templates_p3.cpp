#include <initializer_list>
#include <iostream>

template <typename ... T>
void print(const T& ... t){
    (void)std::initializer_list<int>{ (std::cout << t << '\n', 0) ...};
}

int main() 
{
    print("Hello", "world", 12, 5.03f);
}