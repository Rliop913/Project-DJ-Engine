/**
 * @file BlameController.hpp
 * @brief Perform git blame operations.
 */
#pragma once
#include <filesystem>
#include <git2.h>
#include <optional>
#include <string>
namespace fs = std::filesystem;

/// Result entry for a blamed line
struct BlameResult {

    git_oid commitID;
    size_t  editStartLine = 0;
    size_t  editedLines   = 0;

    git_oid originID;
    size_t  originStartLine = 0;
};

using MAYBE_BLAME_RESULT = std::optional<BlameResult>;

/**
 * @brief Wrapper for libgit2 blame API.
 */
class BlameController {
  private:
    git_blame   *blame       = nullptr;
    unsigned int blameAmount = 0;

  public:
    BlameController() = default;

    /**
     * @brief Open a blame object for the given file.
     */
    bool
    BlameOpen(git_repository    *repo,
              const fs::path    &path,
              git_blame_options *options = nullptr);

    /// Retrieve blame information by index
    MAYBE_BLAME_RESULT
    operator[](unsigned int idx);
    ~BlameController();
};
