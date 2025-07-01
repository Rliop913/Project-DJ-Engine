#include "PDJE_interface.hpp"


audioPlayer*
PDJE::GetPlayerObject()
{
    if(player.has_value()){
        return &player.value();
    }
    else{
        return nullptr;
    }
}

std::vector<DONT_SANITIZE>
ARGSETTER_WRAPPER::GetFXArgKeys(FXList fx)
{
    if(fxp == nullptr){
        return std::vector<DONT_SANITIZE>();
    }
    auto argkey = fxp->GetArgSetter(fx);
    std::vector<DONT_SANITIZE> keylist;
    for(auto& i : argkey){
        keylist.push_back(i.first);
    }
    return keylist;
}

void
ARGSETTER_WRAPPER::SetFXArg(
    FXList fx, 
    const DONT_SANITIZE& key, 
    double arg
    )
{
    if(fxp == nullptr){
        return;
    }
    auto argsetter = fxp->GetArgSetter(fx);
    argsetter[key](arg);
    return;
}
