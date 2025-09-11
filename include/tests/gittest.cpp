#include "gitWrapper.hpp"
#include <iostream>
// #include "gitWrapper.hpp"
int
main()
{
    auto gh = PDJE_GitHandler("rrop", "temp@email.com");
    gh.Open("../");
    std::cout << "loaded ok" << std::endl;
    std::cout << gh.gw.GenTimeStamp() << std::endl;
    // getchar();

    // if(gh.Save("test.txt", "test_second")){
    //    std::cout << "saved ok" << std::endl;
    // }
    // else{
    // for(auto bmsg : gh.gw.handleBranch->ShowExistBranch()){
    //     std::cout << bmsg << std::endl;
    // }
    // gh.UpdateLog();
    // auto graphs = gh.GetLogWithJSONGraph();
    // std::cout << graphs << std::endl;
    // auto commitres = gh.gw.handleBranch->ShowExistCommitsOnBranch("master");
    // for( auto i : commitres){
    //    // std::cout << "branch: " << i. << std::endl;
    //    // for(auto j : i.second){
    //       std::cout << "commit: " << i.msg << std::endl;
    //    // }
    // }
    // std::cout << "save error" << std::endl;
    // }
    // getchar();
    gh.Close();
    return 0;
}
