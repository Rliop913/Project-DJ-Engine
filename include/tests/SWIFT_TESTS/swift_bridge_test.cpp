#include <iostream>

#include "testmainbridge.h"

extern "C" void print_zero_to_ten();
extern "C" void print_hello();


int main(){

    print_hello();
    print_zero_to_ten();


    Test::print_hello_from_cpp_header();
    Test::print_zero_to_ten_from_cpp_header();
    return 0;
}
