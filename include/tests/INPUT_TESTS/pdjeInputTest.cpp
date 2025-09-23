#include "PDJE_Input.hpp"
#include <iostream>
int 
main()
{
    auto pin = PDJE_Input();
    auto dev = pin.GetDevs();
    for(auto& i : dev){
        std::cout << i.Type << ", ";
        std::wcout << i.Name << std::endl;
    }
    return 0;
}