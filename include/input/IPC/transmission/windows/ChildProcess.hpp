#pragma once

#include "ipc_shared_memory.hpp"
#include <unordered_map>
#include <string>
#include <httplib.h>
#include <optional>
namespace PDJE_IPC{
    class ChildProcess {
        private:
        std::unordered_map<std::string, std::function<void()>> callables;
        httplib::Server server;
        void
        EndTransmission();
        
        
        public:
        ChildProcess();

        void RunServer(const int port);
        ~ChildProcess() = default;
    };
};