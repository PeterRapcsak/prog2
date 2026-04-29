#include <iostream>
#include <string>

int main() {
    std::string s1 = "Hello";
    const char *p = (s1 + " World").c_str();    // p a Hello World-re mutat
    std::cout << p << std::endl;

    std::string s2 = "C++";
    s2 = s2 + " 0x11";

    std::cout << p << std::endl;                // ismét kiírjuk: Hello World. Vagy mégse?
    std::cout << s2 <<  std::endl;
    return 0;
}
