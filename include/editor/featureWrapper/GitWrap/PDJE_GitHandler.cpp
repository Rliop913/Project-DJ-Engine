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

SaveDatas
PDJE_GitHandler::GetCommits()
{
    return gw.GetCommits();
}

bool
PDJE_GitHandler::Checkout(const std::string& branch_name, const std::string& timeStamp)
{
    return gw.checkout(branch_name, timeStamp);
}

DiffResult
PDJE_GitHandler::GetDiff(const GitCommit& oldTimeStamp, const GitCommit& newTimeStamp)
{
    return gw.diff(oldTimeStamp, newTimeStamp);
}

std::string
PDJE_GitHandler::GetLogWithMermaidGraph()
{
    std::string log = gw.log();
    std::istringstream iss(log);
    std::string line;
    std::string result = "gitGraph\n";
    std::string lastHash;
    while (std::getline(iss, line)) {
        size_t found = line.find_first_of(" ");
        if (found != std::string::npos) {
            std::string hash = line.substr(0, found);
            std::string subject = line.substr(found + 1);
            if (lastHash.size() > 0) {
                result += "  " + lastHash + " --> " + hash + "\n";
            }
            result += "  " + hash + "[\"" + subject + "\"]\n";
            lastHash = hash;
        }
    }
    return result;

}
