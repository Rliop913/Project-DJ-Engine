SECURITY_ATTRIBUTES sa{};
sa.nLength              = sizeof(sa);
sa.bInheritHandle       = FALSE;
sa.lpSecurityDescriptor = nullptr;

constexpr DWORD flProtect =
    (MEM_PROT_FLAG == 1) ? PAGE_READONLY : PAGE_READWRITE;
constexpr DWORD mapAccess =
    (MEM_PROT_FLAG == 1) ? FILE_MAP_READ : FILE_MAP_ALL_ACCESS;

const uint64_t data_max_length = bsize * count;
memory_handle                  = CreateFileMappingW(INVALID_HANDLE_VALUE,
                                   &sa,
                                   flProtect,
                                   (DWORD)(data_max_length >> 32),
                                   (DWORD)(data_max_length & 0xFFFFFFFF),
                                   memfd_name.wstring().c_str());
if (!memory_handle) {
    return false;
}

ptr = reinterpret_cast<T *>(
    MapViewOfFile(memory_handle, mapAccess, 0, 0, data_max_length));

if (!ptr) {
    CloseHandle(memory_handle);
    memory_handle = nullptr;
    return false;
}

return true;