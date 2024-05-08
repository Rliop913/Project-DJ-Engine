#include <catch2/catch_test_macros.hpp>
#include "data_loader.hpp"
#include "default_functions.h"
#include <iostream>

TEST_CASE("data_loader is available", "[data_loader]")
{
    function_pointers fp;
    fp.init_fileitr=default_file_init;
    fp.line_getter=default_line_getter;
    fp.uninit_fileitr=default_file_uninit;
    data_manager cm(fp);
    auto dm = cm.clone_self();
    KV key_value;
    //std::string test_string="_tf-123_ts-432_offset-1442_testtemp-4423_";
    (*dm)["G://Dropbox/Dropbox/box_inside/git_branch/cpps/Disk_Jockey_Engine_Mini/testdata.txt"];
    (*dm)>>=key_value;
    //dm.line_decoder(test_string, key_value);
    KV vl;
    vl["tf"] = "123";
    vl["ts"] = "432";
    vl["offset"] = "1442";
    vl["testtemp"] = "4423";

    //std::cout<<key_value["tf"];
    //REQUIRE(test_string.substr(1,6) == "tf-123");
    REQUIRE(key_value["tf"] == vl["tf"]);
    REQUIRE(key_value["ts"] == vl["ts"]);
    REQUIRE(key_value["offset"] == vl["offset"]);
    REQUIRE(key_value["testtemp"] == vl["testtemp"]);


}