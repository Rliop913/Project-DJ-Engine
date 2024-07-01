#include "data_loader.hpp"
#include "default_functions.h"
#include "PDJS.pb.h"
#include <iostream>
int main()
{
    function_pointers fp;
    fp.init_fileitr=default_file_init;
    fp.line_getter=default_line_getter;
    fp.uninit_fileitr=default_file_uninit;
    dj_base djb;
    std::cout<<djb.IsInitialized();
    
    
}