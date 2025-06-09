#pragma once

#include <git2.h>
#include <string>

class AddController {
public:
    git_index* index = nullptr;
    AddController() = default;
    // 파일을 인덱스에 추가. 성공 시 true 반환
    bool open(git_repository* repo);
    bool addFile(const std::u8string& path);
    ~AddController();
};
