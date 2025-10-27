#pragma once

#include "PDJE_LOG_SETTER.hpp"
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <ios>
#include <limits>
#include <algorithm>
#include <cctype>
#include <picosha2.h>
#include "PDJE_INPUT_PROCESS_HASH.hpp"

#include <string>

#ifdef WIN32
#define NOMINMAX
#include <Windows.h>

#else
#include <sys/mman.h>
#include <unistd.h>
#endif
namespace PDJE_IPC_UTILS {

#ifdef WIN32
#else
constexpr int PDJE_IPC_SHARE_MEM_RW = PROT_READ | PROT_WRITE;
constexpr int PDJE_IPC_SHARE_MEM_R  = PROT_READ;
constexpr int PDJE_IPC_SHARE_MEM_W  = PROT_WRITE;
#endif

namespace fs = std::filesystem;
constexpr int PDJE_IPC_R = 1;
constexpr int PDJE_IPC_RW = 0;
template <typename T, int MEM_PROT_FLAG> class IPCSharedMem {
  public:
    IPCSharedMem()
    {
        startlog();
    }

    IPCSharedMem(const IPCSharedMem &) = delete;
    IPCSharedMem &
    operator=(const IPCSharedMem &) = delete;

    T     *ptr   = nullptr;

#ifdef WIN32
    HANDLE memory_handle = nullptr;
#else
    int    FD    = -1;
    uint64_t count = 0;
    uint64_t bytes = 0;
#endif

    bool
    GetIPCSharedMemory(const fs::path& memfd_name)
    {
        static_assert(
            std::is_trivially_copyable_v<T>,
            "Shared mem should contain trivially copyable types only.");
        #ifdef WIN32
            constexpr DWORD mapAccess = (MEM_PROT_FLAG == 1) ? FILE_MAP_READ : FILE_MAP_ALL_ACCESS;

            memory_handle = OpenFileMappingW(mapAccess, FALSE, memfd_name.wstring().c_str());
            if(!memory_handle){
                return false;
            }
            ptr = MapViewOfFile(memory_handle, mapAccess, 0,0,0);
            if(!ptr){
                CloseHandle(memory_handle);
                return false;
            }
            return true;
        #else
        return false;//todo -impl
        #endif

    }


    bool
    MakeIPCSharedMemory(const fs::path &memfd_name, const uint64_t count)
    #ifdef WIN32
    {
        if(count == 0){
            return false;
        }
        SECURITY_ATTRIBUTES sa{};
        sa.nLength = sizeof(sa);
        sa.bInheritHandle = FALSE;
        sa.lpSecurityDescriptor = nullptr;

        const uint64_t bsize = sizeof(T);
        if (count > ((std::numeric_limits<uint64_t>::max)() / bsize)) {
            return false;
        }
        constexpr DWORD flProtect = (MEM_PROT_FLAG == 1) ? PAGE_READONLY : PAGE_READWRITE;
        constexpr DWORD mapAccess = (MEM_PROT_FLAG == 1) ? FILE_MAP_READ : FILE_MAP_ALL_ACCESS;

        const uint64_t data_max_length = bsize * count;
        memory_handle = CreateFileMappingW(
            INVALID_HANDLE_VALUE, &sa, flProtect,
            (DWORD)(data_max_length >> 32), (DWORD)(data_max_length & 0xFFFFFFFF), memfd_name.wstring().c_str());
        if (!memory_handle) {
            return false; 
        }

        ptr = reinterpret_cast<T*>( 
            MapViewOfFile(memory_handle, mapAccess, 0, 0, data_max_length)
        );

        
        if (!ptr) {
            CloseHandle(memory_handle);
            memory_handle = nullptr;
            return false; 
        }

        return true;

    }
    #else
    {

        if (data_length == 0) {
            critlog("data_length is zero");
            return false;
        }
        constexpr uint64_t Tsize    = sizeof(T);
        constexpr auto Max_length = std::numeric_limits<uint64_t>::max() / Tsize;
        if (Tsize != 0 && data_length > Max_length) {
            critlog("byte size exceeded uint64_t maximum value");
            return false;
        }

        FD = memfd_create(memfd_name.c_str(), MFD_CLOEXEC);
        if (FD < 0) {
            critlog("memfd not created");
            return false;
        }

        uint64_t byte_length = Tsize * data_length;
        if (ftruncate(FD, static_cast<off_t>(byte_length)) != 0) {
            close(FD);
            FD = -1;
            critlog("ftruncate failed");
            return false;
        }

        void *p = mmap(nullptr, byte_length, MEM_PROT_FLAG, MAP_SHARED, FD, 0);
        if (p == MAP_FAILED) {
            close(FD);
            FD = -1;
            critlog("map failed");
            return false;
        }

        ptr   = reinterpret_cast<T *>(p);
        count = data_length;
        bytes = byte_length;
        return true;
    }
    #endif

    ~IPCSharedMem()
    #ifdef WIN32
    {
        if(ptr){
            UnmapViewOfFile(ptr);
        }
        if(memory_handle){
            CloseHandle(memory_handle); 
        }
    }
    #else
    {
        if (ptr && bytes > 0) {
            if (munmap(ptr, bytes) == -1) {
                critlog("failed to munmap");
            }
        }
        if (FD != -1) {
            if (close(FD) == -1) {
                critlog("failed to close FD");
            }
        }
    }
    #endif
};

static inline fs::path
GetValidProcessExecutor()
{
    fs::path              current = fs::current_path();
    auto lower = [](std::string s){
        std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c){
            return std::tolower(c);});
        return s;
    };
    auto target_hash = lower(EMBEDDED_INPUT_PROCESS_SHA256);
    for (auto &fp : fs::recursive_directory_iterator(
             current, fs::directory_options::skip_permission_denied)) {
        if (fs::is_regular_file(fp) &&
            fp.path().filename().string().find("PDJE_MODULE_INPUT_PROCESS") !=
                std::string::npos) {

                    std::ifstream exfile(fp.path(), std::ios::binary);
                    if(!exfile){
                        continue;
                    }
                    std::vector<unsigned char> hash(picosha2::k_digest_size);
                    picosha2::hash256(exfile, hash.begin(), hash.end());
                    auto file_hash = picosha2::bytes_to_hex_string(hash);
                    if(lower(file_hash) == target_hash){
                        return fp.path();
                    }
            
        }
    }
    
    return {};
}

}; // namespace PDJE_IPC_UTILS