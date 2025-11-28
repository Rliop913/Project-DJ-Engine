#pragma once
#ifdef PDJE_BENCHMARK_ON
#include "PDJE_Highres_Clock.hpp"
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
namespace PDJE_BENCH{
    struct timeData{
        std::string meta;
        uint64_t time;
    };

    class Bench{
        private:
        std::vector<timeData> td;
        PDJE_HIGHRES_CLOCK::CLOCK clk;
        public:
        void Write(const std::string& metadata){
            td.emplace_back(timeData{metadata, clk.Get_MicroSecond()});
        }
        Bench(){
            td.reserve(16384);
        }
        ~Bench(){
            std::ofstream ofs("benchRes.txt");
            if(!ofs.is_open()){
                return;
            }
            for(const auto& log : td){
                ofs << log.meta << "," << log.time << "\n";
            }
            return;
        }
    };
}

inline PDJE_BENCH::Bench bench;

#define WBCH(M) bench.Write(M);

#endif

#ifndef WBCH
#define WBCH(M) 
#endif