#include <iostream>
#include "bcstream.h"

int main() {
    Bcstream bcs;

    char ch;
    while (true) {
        std::cout << "get the next char?" << std::endl;
        char boolean;
        std::cin >> boolean;
        if(boolean == 'y')
        {
          ch = bcs.getChar();
          if (ch == '\0') break; // '\0' indicates end of input
          std::cout << ch << "\n";
        }
        else
        {
          break;
        }
    }

    std::cout << std::endl;
    return 0;
}
