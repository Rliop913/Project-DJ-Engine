constexpr DWORD mapAccess =
    (MEM_PROT_FLAG == 1) ? FILE_MAP_READ : FILE_MAP_ALL_ACCESS;

memory_handle =
    OpenFileMappingW(mapAccess, FALSE, memfd_name.wstring().c_str());
if (!memory_handle) {
    return false;
}
ptr = MapViewOfFile(memory_handle, mapAccess, 0, 0, 0);
if (!ptr) {
    CloseHandle(memory_handle);
    return false;
}
return true;