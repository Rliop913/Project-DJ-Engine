#pragma once

// evade lsp flag. disable red lines temporary. do not activate on build step.
// #define EVADE_LSP

#ifdef EVADE_LSP
#include "MainProcess.hpp"
#endif

namespace PDJE_IPC {


template <typename T>
bool
MainProc::SendBufferArena(const PDJE_Buffer_Arena<T> &mem)
{

    nj j;
    j["PATH"]     = mem.ID;
    j["DATATYPE"] = "input_buffer";
    j["COUNT"]    = mem.BUFFER_COUNT;
    try{
        TXRX_RESPONSE.SEND_IPC_SHMEM.emplace();
        auto resp = TXRX_RESPONSE.SEND_IPC_SHMEM->get_future();
        bool res = txrx->Send(PDJE_CRYPTO::TXRXHEADER::SEND_IPC_SHMEM, j.dump());
        
        if(res){
            res = resp.get();
        }
    
        TXRX_RESPONSE.SEND_IPC_SHMEM.reset();
        if(res){
            return true;
        }
        else{
            critlog("failed to send ipc shared memory.");
            return false;
        }
    }catch(const std::exception& e){
            critlog("failed to send ipc shared memory. Why:");
            critlog(e.what());
            return false;
        }
    
    
}


template <typename T, int MEM_PROT_FLAG>
bool
MainProc::SendIPCSharedMemory(const SharedMem<T, MEM_PROT_FLAG> &mem,
                                 const std::string                 &mem_path,
                                 const std::string                 &dataType)
{
    nj j;
    j["PATH"]     = mem_path;
    j["DATATYPE"] = dataType;
    j["COUNT"]    = mem.data_count;
    try{
        TXRX_RESPONSE.SEND_IPC_SHMEM.emplace();
        auto resp = TXRX_RESPONSE.SEND_IPC_SHMEM->get_future();
        bool res = txrx->Send(PDJE_CRYPTO::TXRXHEADER::SEND_IPC_SHMEM, j.dump());
        
        if(res){
            res = resp.get();
        }
    
        TXRX_RESPONSE.SEND_IPC_SHMEM.reset();
        if(res){
            return true;
        }
        else{
            critlog("failed to send ipc shared memory.");
            return false;
        }
    }catch(const std::exception& e){
            critlog("failed to send ipc shared memory. Why:");
            critlog(e.what());
            return false;
        }
    }
}; // namespace PDJE_IPC