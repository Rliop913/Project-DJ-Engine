#pragma once

// evade lsp flag. disable red lines temporary. do not activate on build step.
// #define EVADE_LSP

#ifdef EVADE_LSP
#include "MainProcess.hpp"
#endif

namespace PDJE_IPC {


template <typename T>
bool
MainProcess::SendBufferArena(const PDJE_Buffer_Arena<T> &mem)
{
 if(!cli){
    critlog("mainprocess is not initialized.");
    return false;
 }
 if(!aead){
    critlog("AEAD is not initialized.");
    return false;
 }
    nj j;
    j["PATH"]     = mem.ID;
    j["DATATYPE"] = "input_buffer";
    j["COUNT"]    = mem.BUFFER_COUNT;
    try{
        auto res = cli->Post("/shmem", aead->EncryptAndPack(j.dump()), "application/json");
    
    if (!res) {
        critlog("shared memory setting from main process has failed result.");
        return false;
    } else {
        if (res->status / 100 == 2) {

            return true;

        } else {
            critlog("failed to set shared memory. status: ");
            critlog(res->status);
            return false;
        }
    }}catch(const std::exception& e){
        critlog("failed to send buffer arena. Why:");
        critlog(e.what());
        return false;
    }
}


template <typename T, int MEM_PROT_FLAG>
bool
MainProcess::SendIPCSharedMemory(const SharedMem<T, MEM_PROT_FLAG> &mem,
                                 const std::string                 &mem_path,
                                 const std::string                 &dataType)
{
     if (!cli) {
        critlog("mainprocess is not initialized.");
        return false;
    }
    if(!aead){
    critlog("AEAD is not initialized.");
    return false;
 }
    nj j;
    j["PATH"]     = mem_path;
    j["DATATYPE"] = dataType;
    j["COUNT"]    = mem.data_count;
    try{
        auto res      = cli->Post("/shmem", aead->EncryptAndPack(j.dump()), "application/json");
    
    if (!res) {
        critlog("shared memory setting from main process has failed result.");
        return false;
    } else {
        if (res->status / 100 == 2) {

            return true;

        } else {
            critlog("failed to set shared memory. status: ");
            critlog(res->status);
            return false;
        }
    }
}catch(const std::exception& e){
        critlog("failed to send ipc shared memory. Why:");
        critlog(e.what());
        return false;
    }
}
}; // namespace PDJE_IPC