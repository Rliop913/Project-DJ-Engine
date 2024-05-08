#include "data_loader.hpp"
#include "default_functions.h"

int main()
{
    function_pointers fp;
    fp.init_fileitr=default_file_init;
    fp.line_getter=default_line_getter;
    fp.uninit_fileitr=default_file_uninit;
    data_manager dm(fp);
    KV key_value;
    dm["G://Dropbox/Dropbox/box_inside/git_branch/cpps/Disk_Jockey_Engine_Mini/testdata.txt"];
    dm>>=key_value;
    // std::string test_string="_tf-123_ts-432_offset-1442_testtemp-4423_";
    // dm.line_decoder(test_string, key_value);
}