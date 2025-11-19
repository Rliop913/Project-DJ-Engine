/**
 * @file AddController.hpp
 * @brief Utility for staging files in Git repositories.
 */
#pragma once

#include <git2.h>
#include <string>

#include <filesystem>
namespace fs = std::filesystem;

/**
 * @brief Provides a simple interface to add files to a Git index.
 */
class AddController {
  public:
    git_index *index = nullptr;
    AddController()  = default;
    /**
     * @brief Open the index from a repository.
     */
    bool
    open(git_repository *repo);

    /**
     * @brief Add a file to the staging index.
     */
    bool
    addFile(const fs::path &path);
    ~AddController();
};
