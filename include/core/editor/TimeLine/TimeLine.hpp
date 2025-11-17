#pragma once
#include "EventMarker.hpp"
#include "GitDatas.hpp"
#include "LineVersion.hpp"
#include "PDJE_LOG_SETTER.hpp"
#include <filesystem>
#include <string>
namespace PDJE_TIMELINE {
namespace fs = std::filesystem;
template <typename CapnpType> class TimeLine {
  private:
    std::shared_ptr<GitData> git;

    EventMarker<CapnpType> mark;

  public:
    bool
    Save()
    {
        try {
            if (!mark.Activate()) {
                return false;
            }
            return mark.Append();
        } catch (const std::exception &e) {
            critlog("failed to save. error occurred. What: ");
            critlog(e.what());
            return false;
        }
    }
    bool
    Undo()
    {
        try {
            return mark.MoveBack();
        } catch (const std::exception &e) {
            critlog("failed to undo. error occurred. What: ");
            critlog(e.what());
            return false;
        }
    }
    bool
    Redo()
    {
        try {
            return mark.MoveFront();
        } catch (const std::exception &e) {
            critlog("failed to redo. error occurred. What: ");
            critlog(e.what());
            return false;
        }
    }
    bool
    Go(const std::string &OID)
    {
        try {
            return mark.Move(OID);
        } catch (const std::exception &e) {
            critlog("failed to Go. error occurred. What: ");
            critlog(e.what());
            return false;
        }
    }
    void
    Diff(const OID &origin, const OID &compare) // todo-impl
    {
    }
    TimeLine(const fs::path    &git_repo_root,
             const std::string &file_name,
             const std::string &auth_name,
             const std::string &auth_email)
        : git(std::make_shared<GitData>(
              git_repo_root, file_name, auth_name, auth_email)),
          mark(git)
    {
        startlog();
    }
    ~TimeLine() = default;
};
}; // namespace PDJE_TIMELINE