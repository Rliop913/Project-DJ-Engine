#include <iostream>
#include "gitWrapper.hpp"
// #include "gitWrapper.hpp"
int main()
{
   auto gh = PDJE_GitHandler("rrop",  "temp@email.com");
   gh.Open("./test_storage");
   std::cout << "loaded ok" << std::endl;
   
   // getchar();

   if(gh.Save("test.txt", "test_second")){
      std::cout << "saved ok" << std::endl;
   }
   else{
      
      // auto commitres = gh.gw.handleBranch->ShowExistCommitsOnBranch("master");
      // for( auto i : commitres){
      //    // std::cout << "branch: " << i. << std::endl;
      //    // for(auto j : i.second){
      //       std::cout << "commit: " << i.msg << std::endl;
      //    // }
      // }
      // std::cout << "save error" << std::endl;
   }
   // getchar();
   gh.Close();
   return 0;
}