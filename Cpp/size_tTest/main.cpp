#include <array>
#include <cstddef>
#include <iostream>

int main()
{
    std::array<std::size_t, 10> a;

    // Example with C++23 std::size_t literal
    //for (auto i = 0uz; i != a.size(); ++i)
    //    std::cout << (a[i] = i) << ' ';
    //std::cout << '\n';

    for(size_t i = 0; i != a.size(); i++)
        std::cout << (a[i] = i) << ' ';
    std::cout << '\n';

    // Example of decrementing loop
    for (std::size_t i = a.size(); i--;)//i == 0时 false跳出循环
        std::cout << a[i] << ' ';
    std::cout << '\n';

    // Note the naive decrementing loop:
    //  for (std::size_t i = a.size() - 1; i >= 0; --i) ... //i>=0恒为真
    // is an infinite loop, because unsigned numbers are always non-negative
}