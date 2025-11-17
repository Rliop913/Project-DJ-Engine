#include "PDJE_interface.hpp"
#include <iostream>

int
main()
{
    auto eg = PDJE("./gittesterRoot");
    eg.InitEditor("dev", "email", "pdje_gittest_sandbox");
    // auto gw = GitWrapper();
    // git_signature* sign;
    // git_signature_now(&sign, "dev", "email");
    // gw.open("./pdje_gittest_sandbox/Mixes",
    // "./pdje_gittest_sandbox/Mixes/mixmetadata.PDJE", sign);
    // //see branch
    // auto brc = gitwrap::branch(gw.repo);
    // std::cout << "head branch: " << gw.handleBranch->branchName << std::endl;
    // std::cout << "head branch: " << brc.branchName << std::endl;
    // if(!brc.MakeNewFromHEAD("testnew")){
    //     std::cout << "failed to make test new branch from head." <<
    //     std::endl;
    // }
    // std::cout << "now branch" << brc.branchName << std::endl;
    // for(auto i : brc.ShowExistBranch()){
    //     std::cout << "exist branches: " << i << std::endl;
    // }
    // if(!brc.DeleteBranch("master")){
    //     std::cout << "failed to delete branch master." << std::endl;
    // }
    // for(auto i : brc.ShowExistBranch()){
    //     std::cout << "after delete exist branches: " << i << std::endl;
    // }
    // if(!brc.MakeNewFromHEAD("testnew_second")){
    //     std::cout << "failed to make test new branch from head." <<
    //     std::endl;
    // }
    // if(!brc.SetBranch("testnew")){
    //     std::cout << "failed to set branch to testnew" << std::endl;
    // }
    // if(!brc.CheckoutThisHEAD()){
    //     std::cout << "failed to checkout to head." << std::endl;
    // }
    // auto chead = brc.GetHEAD();
    // if(!chead){
    //     std::cout << "failed to get head commit." << std::endl;
    // }
    // if(!brc.MakeNewFromCommit(chead.value(), "headnew")){
    //     std::cout << "failed to make branch from head commit." << std::endl;
    // }
    // if(!brc.CheckoutCommitTemp(chead.value())){
    //     std::cout << "failed to checkout to commit temp." << std::endl;
    // }
    // for(auto i : brc.ShowExistBranch()){
    //     std::cout << "after checkout exist branches: " << i << std::endl;
    // }

    // //see commit
    // auto hc = brc.GetHEAD();
    // auto oidstr = git_oid_tostr_s(&hc->commitID);

    // std::cout << "commit msg: " << oidstr << std::endl;
    // std::cout << "commit msg: " << hc->msg << std::endl;

    // gitwrap::commitList cl;
    // if(!cl.UpdateCommits(gw.repo)){
    //     std::cout << "failed to update commit." << std::endl;
    // }
    // if(!cl.OkToAdd(hc->commitID)){
    //     std::cout << "this commit is not ok to add." << std::endl;
    // }

    // see log

    // destroy sandbox
    //  eg.editor->DESTROY_PROJECT();
    //  return 0;
    EDIT_ARG_MIX ma;
    ma.type    = TypeEnum::FILTER;
    ma.details = DetailEnum::HIGH;
    eg.editor->AddLine(ma);
    if (!eg.editor->Undo<EDIT_ARG_MIX>()) {
        std::cout << "UNDO failed" << std::endl;
    }
    if (eg.editor->Redo<EDIT_ARG_MIX>()) {
        std::cout << "UNDO failed" << std::endl;
    }
    eg.editor->DESTROY_PROJECT();

    return 0;
}
