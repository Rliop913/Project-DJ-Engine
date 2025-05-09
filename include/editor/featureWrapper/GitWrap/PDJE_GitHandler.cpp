#include <filesystem>

#include "gitWrapper.hpp"


namespace fs = std::filesystem;



PDJE_GitHandler::PDJE_GitHandler(const std::string& auth_name, const std::string& auth_email)
{
    git_signature_now(&sign, auth_name.c_str(), auth_email.c_str());

}

PDJE_GitHandler::~PDJE_GitHandler()
{
    git_signature_free(sign);
}


bool
PDJE_GitHandler::Open(const std::string& path)
{
    return gw.open(path);
}

bool
PDJE_GitHandler::DeleteGIT(const std::string& path)
{

    if( !fs::exists(path) ||
        !fs::is_directory(path) ||
        !Close())
    {
        return false;
    }
    fs::remove_all(path);
    return true;
}


bool
PDJE_GitHandler::Close()
{
    return gw.close();
}


bool
PDJE_GitHandler::Save(const std::string& tracingFile, const std::string& timeStamp)
{
    if(!gw.add(tracingFile)) return false;
    if(!gw.commit(sign, timeStamp)) return false;
    return true;
}


bool
PDJE_GitHandler::Checkout(const std::string& branch_name, const std::string& timeStamp)
{
    if(gw.handleBranch.has_value()){
        return gw.handleBranch->CheckoutThisHEAD();
    }
    else{
        return false;
    }
}

DiffResult
PDJE_GitHandler::GetDiff(const gitwrap::commit& oldTimeStamp, const gitwrap::commit& newTimeStamp)
{
    return gw.diff(oldTimeStamp, newTimeStamp);
}

struct BranchJSON{
    std::string branchname;
    std::string oid;
};

std::string
PDJE_GitHandler::GetLogWithJSONGraph()
{
    using nj = nlohmann::json;
    nj GraphRoot;
    for(auto& i : gw.log_hdl->heads){
        nj b;
        b["NAME"] = i.BranchName;
        b["OID"] = std::string(git_oid_tostr_s(&i.head));
        GraphRoot["BRANCH"].push_back(b);
    }
    for(auto& i : gw.log_hdl->logs){
        nj c;
        c["OID"] = std::string(git_oid_tostr_s(&i.first));
        c["EMAIL"] = i.second.authEmail;
        c["NAME"] = i.second.authName;
        c["PARENTID"] = std::string(git_oid_tostr_s(&i.second.parentID));
        GraphRoot["COMMIT"].push_back(c);
    }
    return GraphRoot.dump();
}
